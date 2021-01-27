#pragma once

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include "Frame.h"


class Corrector
{
public:
    Corrector();
    Corrector(std::vector<std::shared_ptr<Frame>> frames, std::string debugOutputDir);
    ~Corrector();

    bool computeDistanceBetweenFrames();

    std::list<int> findpath(std::vector<std::shared_ptr<Frame>> nodes);
    std::vector<std::shared_ptr<Frame>> reOrderFrames();

private:
    std::vector<std::shared_ptr<Frame>> m_frames;
    std::list<int> m_rightPositionFrames;
    int m_init_node = -1;

    std::string m_debugOutputDir="";
    std::ofstream m_debugFile;

    inline void printInFile(std::string log) ;
};
