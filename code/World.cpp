Particle nanp = Particle( nan_, nan_, nan_ );

World::World( double ticktime,
              void ( *tickFunc )( World &world ) )
{
    this->ticktime = ticktime;
	this->tickf = tickFunc;
	this->particles = std::vector<Particle>(0);
}

World::~World()
{
    ;
}

Particle &World::get( int index )
{
    if ( index < 0 )
    {
        index = particles.size() + index;
    }
    return particles[index];
}

Particle &World::operator[]( int index )
{
    if ( index < 0 )
    {
        index = particles.size() + index;
    }
    return particles[index];
}

Particle World::pop( int index )
{
    if( index < 0 )
    {
        index = particles.size() + index;
    }
    if( index >= particles.size() || index < 0 )
    {
        return nanp;
    }
    if( index < particles.size() - 1 )
    {
        Particle temp = particles[index];
        particles[index] = particles[particles.size() - 1];
		particles.pop_back();
        return temp;
    }
    else
    {
		Particle temp = particles[particles.size()-1];
		particles.pop_back();
        return temp;
    }
}

void World::del( int index )
{
    if( index < 0 )
    {
        index = particles.size() + index;
    }
    if ( index >= particles.size() || index < 0 )
    {
        return;
    }
    if ( index < particles.size() - 1 )
    {
        particles[index] = particles[particles.size() - 1];
        particles.pop_back();
        return;
    }
    else
    {
        particles.pop_back();
        return;
    }
}

void World::push( const Particle &particle )
{
    particles.push_back( particle );
}

void World::tick( int times )
{
    for( ; times>0; times-- )
    {
        this->tickf( *this );
    }
}