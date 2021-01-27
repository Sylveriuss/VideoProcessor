#pragma once

class Neighbor
{
public:
    Neighbor();
    Neighbor(int frame_position, int matches, float distance);
    ~Neighbor();

    bool operator<( const Neighbor & n ) const;

    int frame_position() const;
    int matches() const;
    float distance() const;

private:
    int m_frame_position = -1;
    int m_matches = -1;
    float m_distance = 0.;
};
