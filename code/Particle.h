#pragma once

class Particle
{
public:
    vector3d coords, velocity;
    double mass;
    double radius;

    Particle( vector3d coords = 0, vector3d velocity = 0, double mass = 0, double radius = 0 );

    Particle &move( vector3d dcoords = vector3d(1, nan_, nan_) );
    Particle &move( double dt );
    Particle &operator+=( vector3d dcoords );
    Particle &operator-=( vector3d dcoords );
};