#include "mydefs.h"
using namespace std;

int argc;
char **args;

void tickGravity( World &world );

World w = World( 3600, tickGravity );
int relind = -1;
int speed = 0;
double ttime = 0;

int wWidth = 700;
int wHeight = 700;

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
SDL_Event ev;
bool running, rendering;
bool antialiased = true, ortho = false;
bool drawing_points = false;
double zoom = 1./(1ll<<38);
double fps;
double curt, lastt;  // current time, last time
double viewangle = 45;
double cameramovspeed = 0.01;

double camrx = 0, camry = 0, camrz = 0;  // angles of rotation
double camx = 1.08, camy = -1.3, camz = 0.34;  // camera position
double movVline = 0;

bool shift_pressed = false;
bool clearing = true;

int main( int argc, char *args[] )
{
	::argc = argc;
	::args = args;
    
    if( !init() )
    {
        fprintf( stderr, "Failed to initialize!\n" );
        return -1;
    }
    running = true;
	rendering = true;
    lastt = (float)clock() / CLOCKS_PER_SEC;
    for( ;running; handleEvents() )
    {
		update();
        if( rendering )
		{
			render();
		}
    }

    close();
    
    return 0;
}

void tickGravity( World &world )
{
	for( int i = 0; i < world.particles.size(); i++ )
	{
		for( int j = i+1; j < world.particles.size(); j++ )
		{
			if( abs(world[i].coords - world[j].coords) <= world[i].radius + world[j].radius )
			{
				if( world[i].mass < world[j].mass )
					swap( world[i], world[j] );
				world[i].velocity = world[i].velocity * world[i].mass + world[j].velocity * world[j].mass;
				world[i].velocity /= world[i].mass + world[j].mass;
				world[i].coords = world[i].coords * world[i].mass + world[j].coords * world[j].mass;
				world[i].coords /= world[i].mass + world[j].mass;
				world[i].mass += world[j].mass;
				world[j].mass = nan_;
				world[i].radius = pow( pow( world[i].radius, 3) + pow( world[j].radius, 3 ), 1/3. );
			}
		}
	}
	for( int i = 0; i < world.particles.size(); )
	{
		if( world[i].mass != world[i].mass || world[i].mass == nan_ )
		    world.del(i);
		else
			i++;
	}
    for( int i = 0; i < world.particles.size(); i++ )
    {
        for( int j = i+1; j < world.particles.size(); j++ )
        {
            vector3d dist = world[j].coords - world[i].coords;
            world[i].velocity += ( world[j].mass * G_const ) / ( abs(dist) * abs(dist) ) *
                                 ( dist / abs(dist) ) * world.ticktime;
			world[j].velocity -= ( world[i].mass * G_const ) / ( abs(dist) * abs(dist) ) *
                                 ( dist / abs(dist) ) * world.ticktime;
        }
    }

    for( int i = 0; i < world.particles.size(); i++ )
    {
        world[i].move( world.ticktime );
    }
}

#include "mydefs.cpp"