#pragma once

class vector3d
{
public:
    double x, y, z;

    vector3d( double x = 0, double y = 0, double z = 0 );
    vector3d( const vector3d &vec );

    friend const vector3d operator+( const vector3d &left, const vector3d &right );
    friend const vector3d operator-( const vector3d &left, const vector3d &right );
    friend const vector3d operator*( const vector3d &vec, double mul );
    friend const vector3d operator*( double mul, const vector3d &vec );
    friend const vector3d operator/( const vector3d &vec, double div );
    friend const vector3d operator+( const vector3d &vec );
    friend const vector3d operator-( const vector3d &vec );
    friend vector3d &operator+=( vector3d &vec, const vector3d &add );
    friend vector3d &operator-=( vector3d &vec, const vector3d &sub );
    friend vector3d &operator*=( vector3d &vec, double mul );
    friend vector3d &operator/=( vector3d &vec, double div );
    friend double abs( const vector3d &vec );
};