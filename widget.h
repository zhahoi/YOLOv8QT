#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio.hpp>

#include "yolov8.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSize>
#include <QPixmap>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QMap>
#include <QVector>
#include <QTextStream>
#include <QList>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    enum class DetectionMode {
        None,
        Camera,
        Video
    };

private slots:
    void on_btnImageDetect_clicked();
    void on_modelChoose_currentIndexChanged(int index);
    void on_btnYes_clicked(bool checked);
    void on_btnNo_clicked(bool checked);
    void do_iouThreshChanged(int value);
    void do_confThreshChanged(int value);
    void on_resolutionChoose_currentIndexChanged(int index);
    void on_btnCamDetect_clicked();
    void on_btnStopDetect_clicked();
    void on_btnVideoDetect_clicked();
    void on_btnStartDetect_clicked();
    void on_btnSaveResult_clicked();

private:
    QImage cvMatToQImage(const cv::Mat& inMat);
    void resizeEvent(QResizeEvent *event);
    void detectAndDisplay();
    void updateConfigFromUI();
    bool reinitializeModel();
    void updateResultDisplay(const std::vector<Object>& objects);
    void updateImageDisplay(bool forceUpdate=false);
    bool isConfigChanged() const;
    void resetConfigChangeFlags();
    void stopVideoDetection();
    void stopCameraDetection();

private:
    cv::Mat currentImage;
    QImage inputImage;
    QImage outputImage;
    QString modelPath;
    bool isDetecting = false;

    // 细化的配置变更标志
    bool modelChanged = false;
    bool resolutionChanged = false;
    bool gpuSettingChanged = false;
    bool thresholdChanged = false;

    QString appDir, modelDir;
    QTimer* inferenceTimer;
    QMutex detectionMutex;  // 线程安全

    // 检测模式
    DetectionMode currentDetectionMode = DetectionMode::None;

    // 摄像头检测
    cv::VideoCapture cap;
    QTimer* camTimer = nullptr;
    bool isCamRunning = false;

    // 视频检测
    cv::VideoCapture videoCap;      // 视频专用捕获对象
    QTimer* videoTimer = nullptr;   // 视频播放定时器
    bool isVideoRunning = false;    // 视频是否正在运行
    bool isVideoPaused = false;     // 视频是否暂停
    int currentFrame = 0;           // 当前帧数
    int totalFrames = 0;            // 总帧数
    int frameCount = 0;             // 统计检测帧数
    double videoFPS = 30.0;         // 视频帧率

    // 保存视频
    cv::VideoWriter videoWriter;
    QString autoSaveVideoPath;

    // 性能监控
    QElapsedTimer performanceTimer, fpsTimer;

    // 缓存变量，避免重复计算
    QSize lastInputSize;
    QSize lastOutputSize;
    QString lastResultText;

private:
    Ui::Widget *ui;
    YOLOv8 yolov8;
    DetectorConfig detector_config;
    DetectorConfig lastConfig;  // 用于配置比较
};
#endif // WIDGET_H
