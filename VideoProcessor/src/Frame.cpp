#include "Frame.h"

#include <opencv2/features2d.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"


Frame::Frame()
{
}

Frame::~Frame()
{
}

Frame::Frame(int matchesThreshold, int frame_position, const cv::Mat mat)
    : m_matchesThreshold(matchesThreshold)
    , m_frame_position(frame_position)
    , m_mat(mat)
{
    // Retrieve Descriptor using ORB
    cv::Ptr<cv::ORB> myBlobDetector = cv::ORB::create( );
    std::vector<cv::KeyPoint> features_points;
    myBlobDetector->detectAndCompute( m_mat, cv::noArray(), features_points, m_descriptor );

    // Transform image into a grey smooth one
    cv::Mat grey;
    cv::cvtColor( m_mat, grey, cv::COLOR_BGR2GRAY );
    cv::GaussianBlur( grey, m_smooth, cv::Size(21,21), 0 );
}

void Frame::addNeighbor(Neighbor potential_neighbor)
{
    m_neighbors.push_back(potential_neighbor);
}

bool Frame::hasNoNeighbors()
{
    return m_neighbors.empty();
}

void Frame::popNeighborsFront()
{
    if (m_neighbors.empty())
        return;

    std::pop_heap(m_neighbors.begin(), m_neighbors.end());
    m_neighbors.pop_back();
}

int Frame::frame_position() const
{
    return m_frame_position;
}

cv::Mat Frame::mat() const
{
    return m_mat;
}

cv::Mat Frame::descriptor() const
{
    return m_descriptor;
}

std::vector<Neighbor> Frame::neighbors() const
{
    return m_neighbors;
}

cv::Mat Frame::smooth() const
{
    return m_smooth;
}

void Frame::displayNeighbors()
{
    if (hasNoNeighbors())
    {
        std::cout << "Frame is intruder in the video." << std::endl;
        return;
    }
    for (auto n : m_neighbors)
    {
        std::cout << m_frame_position << " " << n.frame_position() << " " << n.matches() << " " << n.distance() << std::endl;
    }
}

void Frame::reorderNeighbors()
{
    std::make_heap(m_neighbors.begin(), m_neighbors.end());
}

float Frame::computeAbsDiff(cv::Mat opponent_smooth)
{
    cv::Mat differenceMat;
    cv::absdiff(m_smooth, opponent_smooth, differenceMat);

    int nb =0;

    // Count the white pixels in difference Mat
    for (int y = 0; y<differenceMat.size().height; y++)
    {
        for (int x = 0; x<differenceMat.size().width; x++)
        {
            cv::Vec3b pixel = differenceMat.at<cv::Vec3b>(x, y);
            if ((pixel[0] == 0) && (pixel[1] == 0 )&& (pixel[2] == 0))
                nb += 1;
        }
    }
    return (nb * 100.0) / (differenceMat.size().height * differenceMat.size().width);
}

bool Frame::compare(std::shared_ptr<Frame> opponent)
{
    try {

        cv::BFMatcher bfmatcher(cv::NORM_HAMMING, true);
        std::vector<cv::DMatch> knn_matches;

        // Find descriptor matches using Brute Force method.
        bfmatcher.match(m_descriptor, opponent->m_descriptor, knn_matches );

        int good_matches = knn_matches.size();
        if (good_matches > m_matchesThreshold)
        {
            // If it has enough matches, compare smoothed images to more precise difference measurement.
            float avg = computeAbsDiff(opponent->smooth());

            // Add neighboring value to both frames.
            addNeighbor(Neighbor(opponent->frame_position(), good_matches, avg));
            opponent->addNeighbor(Neighbor(m_frame_position, good_matches, avg));
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

