vector3d::vector3d( double x, double y, double z ) 
{
    this->x = x; this->y = y; this->z = z;
}

vector3d::vector3d( const vector3d &vec )
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

const vector3d operator+( const vector3d &left, const vector3d &right )
{
    return vector3d( left.x + right.x,
                     left.y + right.y,
                     left.z + right.z );
}

const vector3d operator-( const vector3d &left, const vector3d &right )
{
    return vector3d( left.x - right.x, left.y - right.y, left.z - right.z );
}

const vector3d operator*( const vector3d &vec, double mul )
{
    return vector3d( vec.x * mul,
                     vec.y * mul,
                     vec.z * mul );
}

const vector3d operator*( double mul, const vector3d &vec )
{
    return vector3d( vec.x * mul,
                     vec.y * mul,
                     vec.z * mul );
}

const vector3d operator/( const vector3d &vec, double div )
{
    return vector3d( vec.x / div,
                     vec.y / div,
                     vec.z / div );
}

const vector3d operator+( const vector3d &vec )
{
    return vec;
}

const vector3d operator-( const vector3d &vec )
{
    return vector3d( -vec.x,
                     -vec.y,
                     -vec.x );
}

vector3d &operator+=( vector3d &vec, const vector3d &add )
{
    vec.x += add.x;
    vec.y += add.y;
    vec.z += add.z;
    return vec;
}

vector3d &operator-=( vector3d &vec, const vector3d &add )
{
    vec.x -= add.x;
    vec.y -= add.y;
    vec.z -= add.z;
    return vec;
}

vector3d &operator*=( vector3d &vec, double mul )
{
    vec.x *= mul;
    vec.y *= mul;
    vec.z *= mul;
    return vec;
}

vector3d &operator/=( vector3d &vec, double div )
{
    vec.x /= div;
    vec.y /= div; 
    vec.z /= div;
    return vec;
}

double abs( const vector3d &vec )
{
    return std::sqrt( vec.x*vec.x + vec.y*vec.y + vec.z*vec.z );
}