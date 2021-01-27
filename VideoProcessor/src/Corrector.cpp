#include "Corrector.h"

#include <set>
#include <iostream>


Corrector::Corrector()
{
}

Corrector::Corrector(std::vector<std::shared_ptr<Frame> > frames, std::string debugOutputDir)
    : m_frames(frames)
    , m_debugOutputDir(debugOutputDir)
{
    if (m_debugOutputDir != "")
    {
        m_debugFile.open(m_debugOutputDir+"debug_logs.txt");
    }
}

Corrector::~Corrector()
{
    if (m_debugOutputDir != "")
    {
        m_debugFile.close();
    }
}

bool Corrector::computeDistanceBetweenFrames()
{
    std::cout << "[computeDistanceBetweenFrames] 3. Compute Distance between frames." << std::endl;

    for (u_int64_t i=0; i<m_frames.size(); i++)
    {
        for (u_int64_t j=i+1; j<m_frames.size(); j++)
        {
            m_frames[i]->compare(m_frames[j]);
        }

        if (m_debugOutputDir != "")
        {
            for (auto n : m_frames[i]->neighbors())
            {
                std::string log;
                log = std::to_string(m_frames[i]->frame_position()) + " " + std::to_string(n.frame_position());
                log += " " + std::to_string(n.matches()) + " " + std::to_string(n.distance());
                printInFile(log);
            }
        }

    }

    std::cout << "[computeDistanceBetweenFrames] 4. Sort Neighbors according to their distance." << std::endl;

    u_int64_t least_best_matches = 10000;

    for (u_int64_t i=0; i<m_frames.size(); i++)
    {
        // Reorder Neighbors in Heap : to have the nearest at the front
        m_frames[i]->reorderNeighbors();

        // Notice the least friendly one.
        if (!m_frames[i]->hasNoNeighbors() && m_frames[i]->neighbors().size() < least_best_matches)
        {
            least_best_matches = m_frames[i]->neighbors().size();
            m_init_node = i;
        }
    }

    return true;
}


std::list<int> Corrector::findpath(std::vector<std::shared_ptr<Frame> > nodes)
{
    std::set<int> seen;
    std::list<int> res;

    int current_node = m_init_node;
    bool push_front = false;
    bool deadend = false;

    std::cout << "[findpath] 5. Find path of best matches." << std::endl;

    printInFile("Finding path of best matches");

    while (!deadend)
    {
        auto frame = nodes[current_node];
        printInFile("current node "+std::to_string(frame->frame_position()));

        if (seen.find(frame->frame_position()) == seen.end())
        {
            seen.insert(frame->frame_position());
            if (push_front)
            {
                res.push_front(frame->frame_position());
            } else {
                res.push_back(frame->frame_position());
            }
        }


        if (m_debugOutputDir != "")
        {
            std::string log = "seen ";
            for (auto it = seen.begin(); it != seen.end(); it++)
            {
                log += std::to_string(*it) + " ";
            }
            printInFile(log);

            log = "neighbors  ";
            for (auto s : frame->neighbors())
            {
                log += std::to_string(s.frame_position()) + " ";
            }
            printInFile(log);
        }

        printInFile("best one  " + std::to_string(frame->neighbors().front().frame_position()));


        while(!frame->neighbors().empty()
              && seen.find(frame->neighbors().front().frame_position()) != seen.end())
        {
            printInFile("already seen neighbor " + std::to_string(frame->neighbors().front().frame_position()) );
            frame->popNeighborsFront();
        }

        if (frame->neighbors().empty())
        {
            printInFile("all neighbors seen ");
            if (current_node == m_init_node)
            {
                deadend = true;
                printInFile("every node seen from init node");
                break;
            }
            current_node = m_init_node;
            printInFile("changing the insert direction : push_front");

            push_front = true; // Change order from init_node (won't be the case normally)
            continue;
        }

        current_node = frame->neighbors().front().frame_position();
        frame->popNeighborsFront();

        printInFile("updating new position " + std::to_string(current_node));
    }

    return res;

}

std::vector<std::shared_ptr<Frame>> Corrector::reOrderFrames()
{
    if (m_rightPositionFrames.empty())
    {
        m_rightPositionFrames = findpath(m_frames);
    }

    std::string right_order = "right_order ";
    for (auto f : m_rightPositionFrames)
    {
      right_order += std::to_string(f) + " ";
    }
    printInFile(right_order);

    std::vector<std::shared_ptr<Frame>> reorderedFrames;
    for (auto frame_position : m_rightPositionFrames)
    {
        reorderedFrames.push_back(m_frames[frame_position]);
    }

    return reorderedFrames;
}

void Corrector::printInFile(std::string log)
{
    if (m_debugOutputDir == "")
        return;

    m_debugFile << log << "\n";
}
