#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , yolov8()
{
    ui->setupUi(this);

    // 初始化分辨率
    ui->resolutionChoose->clear();
    ui->resolutionChoose->addItem("320x320", QVariant(320));
    ui->resolutionChoose->addItem("480x480", QVariant(480));
    ui->resolutionChoose->addItem("640x640", QVariant(640));
    detector_config.target_size = ui->resolutionChoose->itemData(0).toInt();

    qDebug() << "分辨率大小: " << detector_config.target_size;

    // 初始化权重选择
    ui->modelChoose->clear();
    ui->modelChoose->addItem("YOLOv8n", "/ncnn-yolov8-models/yolov8n");
    ui->modelChoose->addItem("YOLOv8s", "/ncnn-yolov8-models/yolov8s");
    ui->modelChoose->addItem("YOLOv8m", "/ncnn-yolov8-models/yolov8m");

    // 获取模型权重
    appDir = QDir::currentPath();;
    modelDir = appDir + ui->modelChoose->itemData(0).toString();

    qDebug() << "模型路径:" << modelDir;

    // 初始化阈值
    detector_config.nms_threshold = ui->iou_label->text().toFloat();
    detector_config.prob_threshold = ui->conf_label->text().toFloat();
    detector_config.model_path = modelDir.toStdString();
    detector_config.useGPU = false;  // 默认不使用GPU

    qDebug() << "IOU阈值大小: " << detector_config.nms_threshold;
    qDebug() << "置信度阈值大小: " << detector_config.prob_threshold;

    // 保存初始配置
    lastConfig = detector_config;

    // 初始化 fps 计时器
    fpsTimer.start();
    frameCount = 0;

    // 初始化模型
    if (!yolov8.init(detector_config)) {
        QMessageBox::critical(this, "Error", "Model initialization failed!");
    }

    // 设置延迟推理定时器
    inferenceTimer = new QTimer(this);
    inferenceTimer->setSingleShot(true);
    inferenceTimer->setInterval(200); // 减少延迟到150ms，提高响应速度

    connect(inferenceTimer, &QTimer::timeout, this, [this]() {
        if (!currentImage.empty() && !isDetecting) {
            detectAndDisplay();
        }
    });

    // 连接信号槽
    connect(ui->iouThreshSlider, &QSlider::valueChanged, this, &Widget::do_iouThreshChanged);
    connect(ui->confThreshSlider, &QSlider::valueChanged, this, &Widget::do_confThreshChanged);
    connect(ui->modelChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(on_modelChoose_currentIndexChanged(int)));
    connect(ui->resolutionChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(on_resolutionChoose_currentIndexChanged(int)));
    connect(ui->btnStartDetect, &QPushButton::clicked, this, &Widget::on_btnStartDetect_clicked);
    connect(ui->btnStopDetect, &QPushButton::clicked, this, &Widget::on_btnStopDetect_clicked);

    // 初始化UI显示设置
    ui->input->setAlignment(Qt::AlignCenter);
    ui->input->setScaledContents(false);
    ui->output->setAlignment(Qt::AlignCenter);
    ui->output->setScaledContents(false);
    ui->resultText->setReadOnly(true);
    ui->resultText->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    ui->btnCamDetect->setEnabled(true);
    ui->btnVideoDetect->setEnabled(true);
    ui->btnStartDetect->setEnabled(false);
    ui->btnStopDetect->setEnabled(false);
}

Widget::~Widget()
{
    if (inferenceTimer) {
        inferenceTimer->stop();
    }
    delete ui;
}

bool Widget::isConfigChanged() const
{
    bool changed = modelChanged || resolutionChanged || gpuSettingChanged || thresholdChanged;
    if (changed) {
        qDebug() << "配置变化检测: 模型=" << modelChanged << " 分辨率=" << resolutionChanged
                 << " GPU=" << gpuSettingChanged << " 阈值=" << thresholdChanged;
    }
    return changed;
}

void Widget::resetConfigChangeFlags()
{
    modelChanged = false;
    resolutionChanged = false;
    gpuSettingChanged = false;
    thresholdChanged = false;
}

void Widget::on_btnImageDetect_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(
        this,
        tr("打开图片"),
        ".",
        tr("Image Files (*.png *.jpg *.jpeg *.bmp *.gif *.tiff *.webp)")
    );

    if (image_path.isEmpty()) {
        QMessageBox::information(this, "Notice", "No file selected");
        return;
    }

    // 使用OpenCV读取图像，更高效
    currentImage = cv::imread(image_path.toStdString());
    if (currentImage.empty()) {
        QMessageBox::warning(this, "Error", "Failed to open image!");
        return;
    }

    qDebug() << "图像尺寸: " << currentImage.cols << "x" << currentImage.rows;

    // 转换并显示输入图像
    inputImage = cvMatToQImage(currentImage);
    updateImageDisplay();

    // 开始推理
    detectAndDisplay();
}

QImage Widget::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type()) {
        case CV_8UC4: {
            QImage image(inMat.data, inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step), QImage::Format_ARGB32);
            return image;
        }
        case CV_8UC3: {
            QImage image(inMat.data, inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step), QImage::Format_RGB888);
            return image.rgbSwapped();
        }
        case CV_8UC1: {
            static QVector<QRgb> sColorTable;
            if (sColorTable.isEmpty()) {
                sColorTable.reserve(256);
                for (int i = 0; i < 256; ++i) {
                    sColorTable.push_back(qRgb(i, i, i));
                }
            }
            QImage image(inMat.data, inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step), QImage::Format_Indexed8);
            image.setColorTable(sColorTable);
            return image;
        }
        default:
            break;
    }
    return QImage();
}

void Widget::updateImageDisplay(bool forceUpdate)
{
    // 只有在尺寸变化时才重新缩放
    if (!inputImage.isNull()) {
        QSize currentInputSize = ui->input->size();
        if (forceUpdate || currentInputSize != lastInputSize) {
            lastInputSize = currentInputSize;
            QPixmap scaledPixmap = QPixmap::fromImage(inputImage).scaled(
                currentInputSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->input->setPixmap(scaledPixmap);
        }
    }

    if (!outputImage.isNull()) {
        QSize currentOutputSize = ui->output->size();
        if (forceUpdate || currentOutputSize != lastOutputSize) {
            lastOutputSize = currentOutputSize;
            QPixmap scaledOut = QPixmap::fromImage(outputImage).scaled(
                currentOutputSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->output->setPixmap(scaledOut);
        }
    }

    if (forceUpdate) {
        ui->input->repaint();
        ui->output->repaint();
        // 或者使用 QApplication::processEvents() 强制处理所有待处理事件
        QApplication::processEvents();
    }
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 窗口大小变化时更新图像显示
    updateImageDisplay();
}

void Widget::detectAndDisplay()
{
    QMutexLocker locker(&detectionMutex);

    if (isDetecting || currentImage.empty()) {
        return;
    }

    isDetecting = true;
    performanceTimer.start();

    updateConfigFromUI();

    // 检查是否需要重新初始化模型
    if (isConfigChanged()) {
        qDebug() << "配置发生变化，重新初始化模型...";
        if (!reinitializeModel()) {
            isDetecting = false;
            return;
        }
        resetConfigChangeFlags();
    }

    // 执行推理
    std::vector<Object> objects;
    yolov8.detect(currentImage, objects);

    // 修复：每次都重新绘制结果图像
    cv::Mat resultImage = currentImage.clone();
    yolov8.draw(resultImage, objects);

    // 修复：强制更新输出图像
    outputImage = cvMatToQImage(resultImage);

    // 如果是视频模式且视频写入器已打开，写入检测结果帧
    if (currentDetectionMode == DetectionMode::Video &&
        videoWriter.isOpened() &&
        videoTimer && videoTimer->isActive()) {
        videoWriter.write(resultImage);
    }

    // 修复：立即更新UI显示
    if (!outputImage.isNull()) {
        updateImageDisplay(true); // 强制更新
    }

    updateResultDisplay(objects);

    qint64 elapsed = performanceTimer.elapsed(); // 本次推理耗时（ms）

    // 新增更新显示
    frameCount++;
    qint64 fpsElapsed = fpsTimer.elapsed(); // 从开始计算经过的毫秒数

    if (fpsElapsed >= 1000) {  // 每秒更新一次帧率
       float fps = frameCount * 1000.0f / fpsElapsed;
       ui->detectFrameRate->setText(QString("FPS: %1").arg(fps, 0, 'f', 2));
       frameCount = 0;
       fpsTimer.restart();
    }

    ui->detectSpend->setText(QString("检测耗时: %1 ms").arg(elapsed));

    qDebug() << "推理完成，耗时: " << elapsed << "ms，检测到" << objects.size() << "个目标.";

    isDetecting = false;
}

void Widget::updateConfigFromUI()
{
    DetectorConfig newConfig = detector_config; // 从当前配置开始

    // 读取UI配置
    newConfig.nms_threshold = ui->iou_label->text().toFloat();
    newConfig.prob_threshold = ui->conf_label->text().toFloat();

    QVariant resolutionData = ui->resolutionChoose->itemData(ui->resolutionChoose->currentIndex());
    if (resolutionData.isValid()) {
        newConfig.target_size = resolutionData.toInt();
    }

    QVariant modelData = ui->modelChoose->itemData(ui->modelChoose->currentIndex());
    if (modelData.isValid()) {
        newConfig.model_path = (appDir + modelData.toString()).toStdString();
    }

    newConfig.useGPU = ui->btnYes->isChecked();

    // 修复：精确比较配置变化
    if (std::abs(newConfig.nms_threshold - detector_config.nms_threshold) > 0.001f ||
        std::abs(newConfig.prob_threshold - detector_config.prob_threshold) > 0.001f) {
        thresholdChanged = true;
        qDebug() << "阈值变化: IOU=" << newConfig.nms_threshold << " Conf=" << newConfig.prob_threshold;
    }

    if (newConfig.target_size != detector_config.target_size) {
        resolutionChanged = true;
        qDebug() << "分辨率变化: " << newConfig.target_size;
    }

    if (newConfig.model_path != detector_config.model_path) {
        modelChanged = true;
        qDebug() << "模型变化: " << QString::fromStdString(newConfig.model_path);
    }

    if (newConfig.useGPU != detector_config.useGPU) {
        gpuSettingChanged = true;
        qDebug() << "GPU设置变化: " << newConfig.useGPU;
    }

    detector_config = newConfig;
}

bool Widget::reinitializeModel()
{
    qDebug() << "开始重新初始化模型...";
    qDebug() << "模型路径: " << QString::fromStdString(detector_config.model_path);
    qDebug() << "分辨率: " << detector_config.target_size;
    qDebug() << "IOU阈值: " << detector_config.nms_threshold;
    qDebug() << "置信度阈值: " << detector_config.prob_threshold;
    qDebug() << "使用GPU: " << detector_config.useGPU;

    if (!yolov8.init(detector_config)) {
        QMessageBox::critical(this, "Model Reinitialization Failed",
                              "Failed to reinitialize the model. Please check the configuration.");
        return false;
    }

    qDebug() << "模型重新初始化成功!";
    return true;
}

void Widget::updateResultDisplay(const std::vector<Object>& objects)
{
    // Class name mapping for COCO dataset
    static const QStringList classNames = {
        "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat",
        "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
        "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
        "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball",
        "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket",
        "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
        "chair", "sofa", "pottedplant", "bed", "diningtable", "toilet", "tvmonitor", "laptop",
        "mouse", "remote", "keyboard", "cell phone", "microwave", "oven", "toaster", "sink",
        "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
    };

    // Start building result text
    QString resultsText = QString("Detection Results (%1 objects):\n").arg(objects.size());

    if (!objects.empty()) {
        resultsText += "Format: [Index] Class: Confidence% [x, y, width, height]\n";
        resultsText += "----------------------------------------\n";
    }

    for (size_t i = 0; i < objects.size(); ++i) {
        const auto& obj = objects[i];

        // Get class name
        QString className;
        if (obj.label >= 0 && obj.label < classNames.size()) {
            className = classNames[obj.label];
        } else {
            className = QString("class_%1").arg(obj.label);
        }

        // Format detection result line
        resultsText += QString("[%1]   %2: %3%   [%4, %5, %6, %7]\n")
            .arg(i + 1)
            .arg(className)
            .arg(obj.prob * 100, 0, 'f', 1)
            .arg(obj.rect.x, 0, 'f', 1)
            .arg(obj.rect.y, 0, 'f', 1)
            .arg(obj.rect.width, 0, 'f', 1)
            .arg(obj.rect.height, 0, 'f', 1);
    }

    if (resultsText != lastResultText) {
        ui->resultText->setPlainText(resultsText);
        lastResultText = resultsText;
    }
}

void Widget::on_btnYes_clicked(bool checked)
{
    if (checked != detector_config.useGPU) {
        detector_config.useGPU = true;
        gpuSettingChanged = true;

        ui->btnNo->setChecked(false);

        if (!currentImage.empty()) {
            QTimer::singleShot(0, this, [this]() {
                detectAndDisplay();
            });
        }
    }
}

void Widget::on_btnNo_clicked(bool checked)
{
    if (checked != (!detector_config.useGPU)) {
        detector_config.useGPU = false;
        gpuSettingChanged = true;

        ui->btnYes->setChecked(false);

        if (!currentImage.empty()) {
            detectAndDisplay();
        }
    }
}

void Widget::do_iouThreshChanged(int value)
{
    float threshold = qBound(0.0, value / 100.0, 1.0);

    if (qAbs(threshold - detector_config.nms_threshold) < 0.001) {
        return;
    }

    ui->iou_label->setText(QString::number(threshold, 'f', 2));
    detector_config.nms_threshold = threshold;
    thresholdChanged = true;

    qDebug() << "IOU阈值: " << detector_config.nms_threshold;

    if (!currentImage.empty()) {
        detectAndDisplay();
    }
}

void Widget::do_confThreshChanged(int value)
{
    float threshold = qBound(0.0, value / 100.0, 1.0);

    if (qAbs(threshold - detector_config.prob_threshold) < 0.001) {
        return;
    }

    ui->conf_label->setText(QString::number(threshold, 'f', 2));
    detector_config.prob_threshold = threshold;
    thresholdChanged = true;

    qDebug() << "置信度阈值: " << detector_config.prob_threshold;

    if (!currentImage.empty()) {
        detectAndDisplay();
    }
}

void Widget::on_resolutionChoose_currentIndexChanged(int index)
{
    QVariant data = ui->resolutionChoose->itemData(index);
    if (!data.isValid() || !data.canConvert<int>()) {
        qWarning() << "分辨率数据无效!";
        return;
    }

    int newSize = data.toInt();
    if (newSize != detector_config.target_size) {
        detector_config.target_size = newSize;
        resolutionChanged = true;

        qDebug() << "分辨率设置为: " << detector_config.target_size;

        if (!currentImage.empty()) {
            detectAndDisplay();
        }
    }
}

void Widget::on_modelChoose_currentIndexChanged(int index)
{
    QVariant data = ui->modelChoose->itemData(index);
    if (!data.isValid()) {
        return;
    }

    std::string newModelPath = (appDir + data.toString()).toStdString();
    if (newModelPath != detector_config.model_path) {
        detector_config.model_path = newModelPath;
        modelChanged = true;

        qDebug() << "模型路径设置为: " << QString::fromStdString(detector_config.model_path);

        if (!currentImage.empty()) {
            detectAndDisplay();
        }
    }
}

void Widget::on_btnCamDetect_clicked()
{
    // 如果其他模式正在运行，先停止
    if (isVideoRunning) {
        stopVideoDetection();
    }

    if (isCamRunning) {
        QMessageBox::information(this, "Notice", "The camera is already running!");
        return;
    }

    if (!cap.open(0)) {
        QMessageBox::warning(this, "Error", "Failed to open the camera!");
        return;
    }

    // 设置摄像头模式
    currentDetectionMode = DetectionMode::Camera;

    if (!camTimer) {
        camTimer = new QTimer(this);
        camTimer->setInterval(50);
        connect(camTimer, &QTimer::timeout, this, [this]() {
            if (isDetecting || !cap.isOpened())
                return;

            cv::Mat frame;
            cap >> frame;

            if (frame.empty()) {
                qDebug() << "摄像头帧获取失败!!!";
                return;
            }

            currentImage = frame;
            inputImage = cvMatToQImage(currentImage);
            updateImageDisplay();

            detectAndDisplay();
        });
    }

    // camTimer->start();
    isCamRunning = true;
    ui->btnCamDetect->setEnabled(false);
    ui->btnVideoDetect->setEnabled(false);
    ui->btnStartDetect->setEnabled(true);
    ui->btnStopDetect->setEnabled(false);
    ui->progressBar->setVisible(true);

    qDebug() << "摄像头已准备就绪，点击'开始检测'按钮开始实时检测!";
}

void Widget::on_btnStopDetect_clicked()
{
    if (currentDetectionMode == DetectionMode::Camera) {
        // 摄像头模式 - 完全停止
        stopCameraDetection();

    } else if (currentDetectionMode == DetectionMode::Video) {
        // 视频模式 - 暂停
        if (videoTimer && videoTimer->isActive()) {
            videoTimer->stop();
            isVideoPaused = true;
            ui->btnStartDetect->setEnabled(true);
            ui->btnStopDetect->setEnabled(false);
            qDebug() << "视频检测已暂停";
        }
    }
    ui->resultText->clear();
}

void Widget::on_btnVideoDetect_clicked()
{
    // 如果其他模式正在运行，先停止
    if (isCamRunning) {
        stopCameraDetection();
    }

    if (isVideoRunning) {
        stopVideoDetection();
    }

    // 打开文件选择对话框
    QString video_path = QFileDialog::getOpenFileName(
        this,
        tr("选择视频文件"),
        ".",
        tr("Video Files (*.mp4 *.avi *.mov *.mkv *.wmv *.flv *.webm *.m4v *.3gp)")
    );

    if (video_path.isEmpty()) {
        QMessageBox::information(this, "Notice", "No video file selected.");
        return;
    }

    // 使用视频捕获对象打开视频
    if (!videoCap.open(video_path.toStdString())) {
        QMessageBox::warning(this, "Error", "Failed to open the video file!\nPlease check if the file format is supported.");

        return;
    }

    // 获取视频信息
    totalFrames = static_cast<int>(videoCap.get(cv::CAP_PROP_FRAME_COUNT));
    videoFPS = videoCap.get(cv::CAP_PROP_FPS);
    currentFrame = 0;

    if (totalFrames <= 0) {
        QMessageBox::warning(this, "Error", "Failed to retrieve the number of video frames!");
        videoCap.release();
        return;
    }

    // 创建自动保存视频的路径
    QFileInfo videoInfo(video_path);
    QString baseName = videoInfo.baseName();
    autoSaveVideoPath = QCoreApplication::applicationDirPath() +
                       "/" + baseName + "_detected.mp4";

    // 获取视频尺寸
    int videoWidth = static_cast<int>(videoCap.get(cv::CAP_PROP_FRAME_WIDTH));
    int videoHeight = static_cast<int>(videoCap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // 初始化视频写入器
    videoWriter.open(
        autoSaveVideoPath.toStdString(),
        cv::VideoWriter::fourcc('X', '2', '6', '4'),  // 使用 H264 编码
        videoFPS,
        cv::Size(videoWidth, videoHeight)
    );

    if (!videoWriter.isOpened()) {
        QMessageBox::warning(this, "Error", "Failed to create the output video file!");
        videoCap.release();
        return;
    }

    // 设置视频模式
    currentDetectionMode = DetectionMode::Video;

    // 设置进度条
    ui->progressBar->setRange(0, totalFrames - 1);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    // 初始化视频定时器
    if (!videoTimer) {
     videoTimer = new QTimer(this);
     connect(videoTimer, &QTimer::timeout, this, [this]() {
         if (isDetecting || !videoCap.isOpened()) {
             return;
         }

         cv::Mat frame;
         bool success = videoCap.read(frame);

         if (!success || frame.empty()) {
             // 关闭视频写入器
             if (videoWriter.isOpened()) {
                 videoWriter.release();
             }

             // 视频播放完毕
             stopVideoDetection();

             QString message = QString("Video detection completed!\nResults have been saved to: %1")
                                 .arg(autoSaveVideoPath);
             QMessageBox::information(this, "Notice", message);

             ui->resultText->clear();
             return;
         }

         currentFrame++;
         ui->progressBar->setValue(currentFrame);

         // 设置当前帧并进行检测
         currentImage = frame;
         inputImage = cvMatToQImage(currentImage);
         updateImageDisplay();

         // 执行检测
         detectAndDisplay();
     });
    }

    // 根据视频FPS设置定时器间隔
    int interval = qBound(30, static_cast<int>(1000.0 / videoFPS), 100);
    videoTimer->setInterval(interval);

    // 读取第一帧并显示
    cv::Mat firstFrame;
    if (videoCap.read(firstFrame)) {
     currentImage = firstFrame;
     inputImage = cvMatToQImage(currentImage);
     updateImageDisplay();
     currentFrame = 1;
     ui->progressBar->setValue(1);
    }

    // 更新UI状态
    isVideoRunning = true;
    isVideoPaused = false;
    ui->btnCamDetect->setEnabled(false);
    ui->btnVideoDetect->setEnabled(false);
    ui->btnStartDetect->setEnabled(true);
    ui->btnStopDetect->setEnabled(false);

    // 显示视频信息
    QString videoInfo_msg = QString("Video loaded successfully\nTotal Frames: %1\nFPS: %2\n"
                                    "Detection results will be automatically saved to: %3\n\n"
                                    "Click the 'Start Detection' button to begin processing.")
                                .arg(totalFrames)
                                .arg(videoFPS, 0, 'f', 2)
                                .arg(autoSaveVideoPath);

    QMessageBox::information(this, "视频信息", videoInfo_msg);

    qDebug() << "视频加载成功:" << video_path;
    qDebug() << "总帧数:" << totalFrames << "帧率:" << videoFPS;
    qDebug() << "输出路径:" << autoSaveVideoPath;
}

void Widget::on_btnStartDetect_clicked()
{
    if (currentDetectionMode == DetectionMode::Camera) {
        // 摄像头模式
        if (!isCamRunning) {
            QMessageBox::information(this, "Notice", "Please click the 'Camera Detection' button first.");
            return;
        }

        camTimer->start();
        ui->btnStartDetect->setEnabled(false);
        ui->btnStopDetect->setEnabled(true);
        qDebug() << "摄像头检测已开始!";

    } else if (currentDetectionMode == DetectionMode::Video) {
        // 视频模式
        if (!isVideoRunning) {
            QMessageBox::information(this, "Notice", "Please load a video file first.");
            return;
        }

        if (isVideoPaused) {
            // 恢复检测
            isVideoPaused = false;
        }

        videoTimer->start();
        ui->btnStartDetect->setEnabled(false);
        ui->btnStopDetect->setEnabled(true);
        qDebug() << "视频检测已开始!";

    } else {
        QMessageBox::information(this, "Notice", "Please select a detection mode first (Camera or Video).");
    }
}

void Widget::stopVideoDetection()
{
    if (videoTimer && videoTimer->isActive()) {
        videoTimer->stop();
    }

    // 关闭视频写入器
    if (videoWriter.isOpened()) {
        videoWriter.release();
    }

    if (videoCap.isOpened()) {
        videoCap.release();
    }

    isVideoRunning = false;
    isVideoPaused = false;
    currentFrame = 0;
    totalFrames = 0;
    currentDetectionMode = DetectionMode::None;

    // 恢复UI状态
    ui->btnCamDetect->setEnabled(true);
    ui->btnVideoDetect->setEnabled(true);
    ui->btnStartDetect->setEnabled(false);
    ui->btnStopDetect->setEnabled(false);

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

    // 清除显示
    ui->input->clear();
    ui->output->clear();

    // 清除缓存变量
    currentImage.release();

    inputImage = QImage();
    outputImage = QImage();
    lastInputSize = QSize();
    lastOutputSize = QSize();

    qDebug() << "视频检测已完全停止!";
}

void Widget::stopCameraDetection()
{
    if (camTimer && camTimer->isActive()) {
        camTimer->stop();
    }

    if (cap.isOpened()) {
        cap.release();
    }

    isCamRunning = false;
    currentDetectionMode = DetectionMode::None;

    // 恢复UI状态
    ui->btnCamDetect->setEnabled(true);
    ui->btnVideoDetect->setEnabled(true);
    ui->btnStartDetect->setEnabled(false);
    ui->btnStopDetect->setEnabled(false);

    // 清除显示
    ui->input->clear();
    ui->output->clear();

    // 清除缓存变量
    currentImage.release();
    inputImage = QImage();
    outputImage = QImage();
    lastInputSize = QSize();
    lastOutputSize = QSize();

    qDebug() << "摄像头检测已停止!";
}

void Widget::on_btnSaveResult_clicked()
{
    if (outputImage.isNull()) {
        QMessageBox::information(this, "Notice", "No detection result available to save.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "保存图像结果",
        ".",
        "图像文件 (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        if (outputImage.save(filePath)) {
            QMessageBox::information(this, "Notice", "Image saved successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save the image!");
        }
    }
}
