// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2024 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef YOLOV8_H
#define YOLOV8_H

#define NOMINMAX
#include <windows.h>
#include <iostream>

#include "detector_config.h"
#include <opencv2/core/core.hpp>
#include <net.h>

struct Object
{
    cv::Rect_<float> rect;
    int label;
    float prob;
};

class YOLOv8
{
public:
    YOLOv8() = default;
    ~YOLOv8();

    bool init(const DetectorConfig& config);
    int detect(const cv::Mat& rgb, std::vector<Object>& objects);
    int draw(cv::Mat& rgb, const std::vector<Object>& objects);

private:
    ncnn::Net yolo;
    ncnn::UnlockedPoolAllocator blob_pool_allocator;
    ncnn::PoolAllocator workspace_pool_allocator;

    const float mean_vals[3] = { 103.53f, 116.28f, 123.675f };
    const float norm_vals[3] = { 1 / 255.f, 1 / 255.f, 1 / 255.f };

    DetectorConfig m_config;
    bool initialized = false;
};

#endif // YOLOV8_H

