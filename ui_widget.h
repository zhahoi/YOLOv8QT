/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QFrame *inputFrame;
    QHBoxLayout *horizontalLayout_3;
    QLabel *input;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QFrame *outputFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *output;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *btnStartDetect;
    QProgressBar *progressBar;
    QPushButton *btnStopDetect;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QPushButton *btnImageDetect;
    QPushButton *btnCamDetect;
    QPushButton *btnVideoDetect;
    QPushButton *btnSaveResult;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QComboBox *modelChoose;
    QLabel *label_4;
    QComboBox *resolutionChoose;
    QLabel *label_6;
    QRadioButton *btnYes;
    QRadioButton *btnNo;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QLabel *iou_label;
    QSlider *iouThreshSlider;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_2;
    QLabel *conf_label;
    QSlider *confThreshSlider;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QPlainTextEdit *resultText;
    QHBoxLayout *horizontalLayout_5;
    QLabel *detectFrameRate;
    QLabel *detectSpend;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1113, 802);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/YOLO.png"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        gridLayout_10 = new QGridLayout(Widget);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        groupBox->setFont(font);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        inputFrame = new QFrame(groupBox);
        inputFrame->setObjectName(QString::fromUtf8("inputFrame"));
        inputFrame->setFrameShape(QFrame::StyledPanel);
        inputFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(inputFrame);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        input = new QLabel(inputFrame);
        input->setObjectName(QString::fromUtf8("input"));

        horizontalLayout_3->addWidget(input);


        gridLayout->addWidget(inputFrame, 0, 0, 1, 1);


        gridLayout_8->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        outputFrame = new QFrame(groupBox_2);
        outputFrame->setObjectName(QString::fromUtf8("outputFrame"));
        outputFrame->setFrameShape(QFrame::StyledPanel);
        outputFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(outputFrame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        output = new QLabel(outputFrame);
        output->setObjectName(QString::fromUtf8("output"));

        horizontalLayout_2->addWidget(output);


        gridLayout_2->addWidget(outputFrame, 0, 0, 1, 1);


        gridLayout_8->addWidget(groupBox_2, 1, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(11, -1, 11, -1);
        btnStartDetect = new QPushButton(Widget);
        btnStartDetect->setObjectName(QString::fromUtf8("btnStartDetect"));
        btnStartDetect->setMinimumSize(QSize(40, 40));
        btnStartDetect->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"border-style: solid;\n"
"border-width: 0px;\n"
"border-radius: 0px;\n"
"background-color: rgba(223, 223, 223, 0);\n"
"}\n"
"QPushButton::focus{outline: none;}\n"
"QPushButton::hover {\n"
"border-style: solid;\n"
"border-width: 0px;\n"
"border-radius: 0px;\n"
"background-color: rgba(223, 223, 223, 150);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnStartDetect->setIcon(icon1);
        btnStartDetect->setIconSize(QSize(20, 20));
        btnStartDetect->setCheckable(true);

        horizontalLayout_12->addWidget(btnStartDetect);

        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 5));
        progressBar->setStyleSheet(QString::fromUtf8("QProgressBar{ color: rgb(255, 255, 255); font:12pt; border-radius:2px; text-align:center; border:none; background-color: rgba(215, 215, 215,100);} \n"
"QProgressBar:chunk{ border-radius:0px; background: rgba(55, 55, 55, 200);}"));
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        horizontalLayout_12->addWidget(progressBar);

        btnStopDetect = new QPushButton(Widget);
        btnStopDetect->setObjectName(QString::fromUtf8("btnStopDetect"));
        btnStopDetect->setMinimumSize(QSize(40, 40));
        btnStopDetect->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"border-style: solid;\n"
"border-width: 0px;\n"
"border-radius: 0px;\n"
"background-color: rgba(223, 223, 223, 0);\n"
"}\n"
"QPushButton::focus{outline: none;}\n"
"QPushButton::hover {\n"
"border-style: solid;\n"
"border-width: 0px;\n"
"border-radius: 0px;\n"
"background-color: rgba(223, 223, 223, 150);}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnStopDetect->setIcon(icon2);
        btnStopDetect->setIconSize(QSize(20, 20));

        horizontalLayout_12->addWidget(btnStopDetect);


        gridLayout_8->addLayout(horizontalLayout_12, 2, 0, 1, 1);

        gridLayout_8->setRowStretch(0, 6);
        gridLayout_8->setRowStretch(1, 6);

        gridLayout_9->addLayout(gridLayout_8, 0, 0, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        groupBox_5 = new QGroupBox(Widget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        btnImageDetect = new QPushButton(groupBox_5);
        btnImageDetect->setObjectName(QString::fromUtf8("btnImageDetect"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnImageDetect->setIcon(icon3);

        gridLayout_5->addWidget(btnImageDetect, 0, 0, 1, 1);

        btnCamDetect = new QPushButton(groupBox_5);
        btnCamDetect->setObjectName(QString::fromUtf8("btnCamDetect"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/cam.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCamDetect->setIcon(icon4);

        gridLayout_5->addWidget(btnCamDetect, 1, 0, 1, 1);

        btnVideoDetect = new QPushButton(groupBox_5);
        btnVideoDetect->setObjectName(QString::fromUtf8("btnVideoDetect"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/video.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnVideoDetect->setIcon(icon5);

        gridLayout_5->addWidget(btnVideoDetect, 0, 1, 1, 1);

        btnSaveResult = new QPushButton(groupBox_5);
        btnSaveResult->setObjectName(QString::fromUtf8("btnSaveResult"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/files.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSaveResult->setIcon(icon6);

        gridLayout_5->addWidget(btnSaveResult, 1, 1, 1, 1);


        gridLayout_7->addWidget(groupBox_5, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(Widget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setFont(font);
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        modelChoose = new QComboBox(groupBox_3);
        modelChoose->setObjectName(QString::fromUtf8("modelChoose"));
        modelChoose->setEditable(true);

        gridLayout_3->addWidget(modelChoose, 0, 1, 1, 2);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        resolutionChoose = new QComboBox(groupBox_3);
        resolutionChoose->setObjectName(QString::fromUtf8("resolutionChoose"));
        resolutionChoose->setEditable(true);

        gridLayout_3->addWidget(resolutionChoose, 1, 1, 1, 2);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_6, 2, 0, 1, 1);

        btnYes = new QRadioButton(groupBox_3);
        btnYes->setObjectName(QString::fromUtf8("btnYes"));

        gridLayout_3->addWidget(btnYes, 2, 1, 1, 1);

        btnNo = new QRadioButton(groupBox_3);
        btnNo->setObjectName(QString::fromUtf8("btnNo"));
        btnNo->setChecked(true);

        gridLayout_3->addWidget(btnNo, 2, 2, 1, 1);


        gridLayout_7->addWidget(groupBox_3, 1, 0, 1, 1);

        groupBox_4 = new QGroupBox(Widget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/iou.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon7);
        pushButton->setFlat(true);

        horizontalLayout->addWidget(pushButton);

        iou_label = new QLabel(groupBox_4);
        iou_label->setObjectName(QString::fromUtf8("iou_label"));
        iou_label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(iou_label);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        iouThreshSlider = new QSlider(groupBox_4);
        iouThreshSlider->setObjectName(QString::fromUtf8("iouThreshSlider"));
        iouThreshSlider->setMaximum(100);
        iouThreshSlider->setValue(45);
        iouThreshSlider->setOrientation(Qt::Horizontal);
        iouThreshSlider->setTickPosition(QSlider::TicksAbove);
        iouThreshSlider->setTickInterval(5);

        gridLayout_4->addWidget(iouThreshSlider, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/thresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon8);
        pushButton_2->setFlat(true);

        horizontalLayout_4->addWidget(pushButton_2);

        conf_label = new QLabel(groupBox_4);
        conf_label->setObjectName(QString::fromUtf8("conf_label"));
        conf_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(conf_label);


        gridLayout_4->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        confThreshSlider = new QSlider(groupBox_4);
        confThreshSlider->setObjectName(QString::fromUtf8("confThreshSlider"));
        confThreshSlider->setMaximum(100);
        confThreshSlider->setSingleStep(1);
        confThreshSlider->setValue(25);
        confThreshSlider->setOrientation(Qt::Horizontal);
        confThreshSlider->setTickPosition(QSlider::TicksAbove);
        confThreshSlider->setTickInterval(5);

        gridLayout_4->addWidget(confThreshSlider, 3, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_4, 2, 0, 1, 1);

        groupBox_6 = new QGroupBox(Widget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        resultText = new QPlainTextEdit(groupBox_6);
        resultText->setObjectName(QString::fromUtf8("resultText"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        resultText->setFont(font1);

        gridLayout_6->addWidget(resultText, 0, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_6, 3, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_7, 0, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        detectFrameRate = new QLabel(Widget);
        detectFrameRate->setObjectName(QString::fromUtf8("detectFrameRate"));

        horizontalLayout_5->addWidget(detectFrameRate);

        detectSpend = new QLabel(Widget);
        detectSpend->setObjectName(QString::fromUtf8("detectSpend"));

        horizontalLayout_5->addWidget(detectSpend);


        gridLayout_9->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        gridLayout_9->setColumnStretch(0, 7);
        gridLayout_9->setColumnStretch(1, 3);

        gridLayout_10->addLayout(gridLayout_9, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "YOLODetect", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Widget", "\345\216\237\345\247\213\350\276\223\345\205\245", nullptr));
        input->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("Widget", "\346\243\200\346\265\213\350\276\223\345\207\272", nullptr));
        output->setText(QString());
        btnStartDetect->setText(QString());
        btnStopDetect->setText(QString());
        groupBox_5->setTitle(QCoreApplication::translate("Widget", "\344\270\273\350\246\201\345\212\237\350\203\275", nullptr));
        btnImageDetect->setText(QCoreApplication::translate("Widget", "\345\233\276\347\211\207\346\243\200\346\265\213", nullptr));
        btnCamDetect->setText(QCoreApplication::translate("Widget", "\346\221\204\345\203\217\345\244\264\346\243\200\346\265\213", nullptr));
        btnVideoDetect->setText(QCoreApplication::translate("Widget", "\350\247\206\351\242\221\346\243\200\346\265\213", nullptr));
        btnSaveResult->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230\346\243\200\346\265\213\347\273\223\346\236\234", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Widget", "\346\250\241\345\236\213\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\346\250\241\345\236\213\351\200\211\346\213\251", nullptr));
        modelChoose->setCurrentText(QCoreApplication::translate("Widget", "\351\200\211\346\213\251\346\250\241\345\236\213(n/s/m)", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\346\243\200\346\265\213\345\210\206\350\276\250\347\216\207", nullptr));
        resolutionChoose->setCurrentText(QCoreApplication::translate("Widget", "\351\200\211\346\213\251\345\210\206\350\276\250\347\216\207(320/480/640)", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "\344\275\277\347\224\250GPU", nullptr));
        btnYes->setText(QCoreApplication::translate("Widget", "\346\230\257", nullptr));
        btnNo->setText(QCoreApplication::translate("Widget", "\345\220\246", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Widget", "\346\243\200\346\265\213\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\345\275\223\345\211\215\347\232\204IOU\351\230\210\345\200\274\357\274\232", nullptr));
        iou_label->setText(QCoreApplication::translate("Widget", "0.45", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\345\275\223\345\211\215\347\232\204\347\275\256\344\277\241\345\272\246\351\230\210\345\200\274\357\274\232", nullptr));
        conf_label->setText(QCoreApplication::translate("Widget", "0.25", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("Widget", "\346\243\200\346\265\213\347\273\223\346\236\234", nullptr));
        detectFrameRate->setText(QCoreApplication::translate("Widget", "FPS:", nullptr));
        detectSpend->setText(QCoreApplication::translate("Widget", "\346\243\200\346\265\213\350\200\227\346\227\266:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
