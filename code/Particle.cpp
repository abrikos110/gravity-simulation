Particle::Particle( vector3d coords_, vector3d velocity_, double mass_, double radius_ )
{
    this->coords = coords_;
    this->velocity = velocity_;
    this->mass = mass_;
    this->radius = radius_;
}

Particle &Particle::move( vector3d dcoords )
{
    coords += dcoords;
    return *this;
}

Particle &Particle::move( double dt )
{
    coords += velocity * dt;
    return *this;
}

Particle &Particle::operator+=( vector3d dcoords )
{
    coords += dcoords;
    return *this;
}

Particle &Particle::operator-=( vector3d dcoords )
{
    coords -= dcoords;
    return *this;
}