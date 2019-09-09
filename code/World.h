#pragma once

class World
{
public:
    std::vector<Particle> particles;
    double ticktime;
    
    void (*tickf)( World &world );

    World( double ticktime = 1,
           void (*tickFunc)( World &world ) = nullptr );
    ~World();

    Particle &get( int index );
    Particle &operator[]( int index );
    Particle pop( int index = -1 );
    void push( const Particle &particle );
    void del( int index = -1 );
    void tick( int times = 1 );
};