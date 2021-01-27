#pragma once

#include <string>


class VideoProcessor
{
public:
    VideoProcessor();
    ~VideoProcessor();

    int correctVideoTimeline(std::string fileName
                             , std::string outputDir
                             , int matchesThreshold
                             , bool debug = false
                             , bool reverse_video = false);
};
