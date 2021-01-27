#include "VideoProcessor.h"

#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/imgcodecs/legacy/constants_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Frame.h"
#include "Corrector.h"


using namespace cv;


VideoProcessor::VideoProcessor()
{
}

VideoProcessor::~VideoProcessor()
{
}

int VideoProcessor::correctVideoTimeline(std::string fileName
                                         , std::string outputDir
                                         , int matchesThreshold
                                         , bool debug
                                         , bool reverse_video)
{
    std::cout << "[correctVideoTimeline] 0. Parameters: fileName:" << fileName
              << " outputDir:" << outputDir
              << " debug:" << debug
              << " matches Threshold:" << matchesThreshold << std::endl;
    try
    {
        // Reading the video

        VideoCapture cap(fileName);
        if(!cap.isOpened())
            CV_Error(CV_StsError, "Can not open Video file");

        double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
        std::cout << "[correctVideoTimeline] 1. Getting the dimensions of the original video, width="
                  << dWidth <<", height=" << dHeight << std::endl;

        std::cout << "[correctVideoTimeline] 2. Reading the video file frame by frame. Descriptor and Smoothed Image are created." << std::endl;

        int N = cap.get(CV_CAP_PROP_FRAME_COUNT);
        std::vector<std::shared_ptr<Frame>> m_frames;
        for(int frameNum = 0; frameNum < N; frameNum++)
        {
            Mat frame;
            cap >> frame;
            m_frames.push_back(std::make_shared<Frame>(matchesThreshold, frameNum, frame));
        }

        // Processing the frames

        Corrector corrector(m_frames
                            , debug ? outputDir : "" );
        corrector.computeDistanceBetweenFrames();

        auto reorderedFrames = corrector.reOrderFrames();
        if (reverse_video)
            std::reverse(reorderedFrames.begin(), reorderedFrames.end());

        // Output the correct video.

        VideoWriter writer;
        int codec = VideoWriter::fourcc('m', 'p', '4', 'v');
        double fps = 15.0;
        std::string outputFile = outputDir + "corrected_video.mp4";

        std::cout << "[correctVideoTimeline] 6. Saving new video from selected and sorted framesa at " << outputFile << "." << std::endl;

        writer.open(outputFile, codec, fps, frameSize, true);

        if (debug)
        {
            std::cout << "[correctVideoTimeline] 7. Print ordered frames (debug mode)." << std::endl;
        }

        std::vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
        compression_params.push_back(100);
        int count = 0;
        for (std::shared_ptr<Frame> frame : reorderedFrames)
        {
            writer.write(frame->mat());

            if (debug)
            {
                std::string filePath = outputDir + "corrected_video_frame_" + std::to_string(count) +"_"+ std::to_string(frame->frame_position()) + ".jpg";
                imwrite(filePath, frame->mat(), compression_params);
            }
            ++count;
        }

        std::cout << "[correctVideoTimeline] Execution finished well." << std::endl;

    }
    catch( cv::Exception& e )
    {
        std::cerr << e.msg << std::endl;
        return -1 ;
    }

    return 0;
}
