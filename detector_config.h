#ifndef DETECTOR_CONFIG_H
#define DETECTOR_CONFIG_H

#include <string>

struct DetectorConfig
{
    float prob_threshold = 0.25f;
    float nms_threshold = 0.45f;
    int target_size = 640;
    std::string model_path = "./yolov8n";  // without suffix
    bool useGPU = false;
};

#endif // DETECTOR_CONFIG_H
