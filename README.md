# YOLOv8QT
使用Qt绘制YOLOv8目标检测操作界面，可以进行图像、视频和摄像头的目标检测（纯C++编写，ncnn推理框架）。

检测算法和UI界面的设计基本上是解耦状态，因此，可以替换检测代码，修改少量的UI界面便可进行新的检测算法适配。



### 测试环境

- **QT-5.14.2**
- **opencv-3.4.10**
- **ncnn-20240820-full-source**



### 主要功能

- 允许图像、视频、摄像头目标检测推理；
- 允许推理前和推理过程中，选择模型大小、检测分辨率、是否使用GPU；
- 允许推理前和推理过程中进行检测参数的修改；
- 允许视频推理过程中进行推理暂停；
- 允许显示当前推理的FPS和推理时长；
- 允许对检测的UI界面进行拉伸；



### UI界面功能面板

![ui](https://github.com/zhahoi/YOLOv8QT/blob/main/docs/ui.png)



### 图像检测

![image_detect](https://github.com/zhahoi/YOLOv8QT/blob/main/docs/image_detect.png)



### 视频推理

![video_detect](https://github.com/zhahoi/YOLOv8QT/blob/main/docs/video_detect.png)



### 摄像头推理

![cam_detect](https://github.com/zhahoi/YOLOv8QT/blob/main/docs/cam_detect.png)



### 注意事项：

(1) 在使用本项目时，需要修改"**YOLOv8QT.pro**"文件中的，ncnn和opencv的路径，将其改为你运行环境的实际路径；

```
# 配置OPENCV环境
INCLUDEPATH += C:/NCNN/opencv-3.4.10/opencv/build/include \
               C:/NCNN/opencv-3.4.10/opencv/build/include/opencv \
               C:/NCNN/opencv-3.4.10/opencv/build/include/opencv2

# 添加v15版 opencv 库文件，区分debug和release
LIBS += -LC:/NCNN/opencv-3.4.10/opencv/build/x64/vc15/lib/ -lopencv_world3410 \
                -LC:/NCNN/opencv-3.4.10/opencv/build/x64/vc15/lib/ -lopencv_world3410d

INCLUDEPATH += C:/NCNN/opencv-3.4.10/opencv/build/x64/vc15
DEPENDPATH += C:/NCNN/opencv-3.4.10/opencv/build/x64/vc15

LIBS += -LC:/NCNN/ncnn-20240820-full-source/build/install/lib -lncnn
INCLUDEPATH += C:/NCNN/ncnn-20240820-full-source/build/install/include/ncnn
DEPENDPATH += C:/NCNN/ncnn-20240820-full-source/build/install/include/ncnn
```

(2)虽然本项目可以在运行阶段修改IOU和置信度的阈值，但是如果频繁修改阈值的话会导致推理卡顿，建议在推理之前将其设置到合理的数值；如果需要在推理过程中修改，可以间隔一段时间再修改，以保证推理可以正常进行。

(3)本项目的“保存检测结果”的按钮只支持保存图片，视频推理的话，会在推理完成之后自动保存，保存的路径为项目的根目录。



### 写在最后

创作不易，如果觉得这个仓库还可以的话，麻烦给一个star，这就是对我最大的鼓励。



