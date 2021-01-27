#include "Neighbor.h"

Neighbor::Neighbor() {}

Neighbor::Neighbor(int frame_position, int matches, float distance)
    : m_frame_position(frame_position)
    , m_matches(matches)
    , m_distance(distance)
{
}

Neighbor::~Neighbor()
{
}

bool Neighbor::operator<(const Neighbor &n) const
{
    return m_distance < n.m_distance;
}

int Neighbor::frame_position() const
{
    return m_frame_position;
}

int Neighbor::matches() const
{
    return m_matches;
}

float Neighbor::distance() const
{
    return m_distance;
}
