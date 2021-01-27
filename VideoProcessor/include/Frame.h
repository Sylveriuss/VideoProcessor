#pragma once

#include <memory>
#include <vector>
#include <opencv2/core/mat.hpp>
#include "Neighbor.h"

struct Frame
{
public:
    Frame();
    Frame(int best_matches, int frame_position, cv::Mat mat);
    ~Frame();

    bool compare(std::shared_ptr<Frame> opponent);
    void displayNeighbors();
    void reorderNeighbors();
    bool hasNoNeighbors();
    void popNeighborsFront();
    void addNeighbor(Neighbor potential_neighbor);

    int frame_position() const;
    cv::Mat mat() const;
    cv::Mat descriptor() const;
    cv::Mat smooth() const;
    std::vector<Neighbor> neighbors() const;

private:
    int m_matchesThreshold = 190;
    int m_frame_position = -1;
    cv::Mat m_mat;
    cv::Mat m_descriptor;
    cv::Mat m_smooth;
    std::vector<Neighbor> m_neighbors;

    float computeAbsDiff(cv::Mat opponent_smooth);
};
