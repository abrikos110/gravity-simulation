bool init()
{
	//=============================INPUT=============================
	if( argc != 1 )
	    freopen( args[1], "r", stdin );
	else
	{
		char filename[1024];
		in >> filename;
		freopen( filename, "r", stdin );
	}
    int cnt;
	in >> cnt;
    while( w.particles.size() != 0 )
    {
        w.del();
    }
    for( int i = 0; i < cnt; i++ )
    {
        Particle curp;
        in >> curp.coords.x >> curp.coords.y >> curp.coords.z;
        in >> curp.velocity.x >> curp.velocity.y >> curp.velocity.z;
        in >> curp.mass;
        in >> curp.radius;
        w.push( curp );
    }
	//===============================================================
	
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) // Init SDL
    {
        fprintf( stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 ); // Use OpenGL 2.1
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    gWindow = SDL_CreateWindow( "SSP",
                                0,  //SDL_WINDOWPOS_UNDEFINED,
								25, //SDL_WINDOWPOS_UNDEFINED,
								wWidth,
								wHeight,
								SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if( gWindow == NULL ) // Create window
    {
        fprintf( stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    
    gContext = SDL_GL_CreateContext( gWindow ); // Create OpenGL Context
    if( gContext == NULL )
    {
        fprintf( stderr, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
    }
    
    if( SDL_GL_SetSwapInterval( 1 ) < 0 ) // VSync
    {
        fprintf( stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    if( !initGL() ) // Init OpenGL
    {
        fprintf( stderr, "Unable to initialize OpenGL!\n" );
        return false;
    }
	
	///////////////////////////////////
	if( antialiased )
	{
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
		glDisable( GL_LINE_SMOOTH );
		glDisable( GL_POLYGON_SMOOTH );
		glDisable( GL_MULTISAMPLE );
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POLYGON_SMOOTH );
		glDisable( GL_MULTISAMPLE );
	}
	///////////////////////////////////
    
	setdefault();
	modifyCoordsS( true );
	
    return true;
}

bool initGL()
{
    GLenum error = GL_NO_ERROR;

    setprojection();
    if( ( error = glGetError() ) != GL_NO_ERROR )
    {
        fprintf( stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    if( ( error = glGetError() ) != GL_NO_ERROR )
    {
        fprintf( stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    if( ( error = glGetError() ) != GL_NO_ERROR )
    {
        fprintf( stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    
    return true;
}

void setdefault()
{
	if( !shift_pressed || 1 )
	{
		zoom = 1./(1ll<<38);
		relind = -1;
		
		camrx = -24, camry = 36, camrz = 0;
        camx = 1.3, camy = -1.13, camz = 1.09;
		movVline = 0;
		//w.ticktime = 3600;
		viewangle = 45;
		
		setprojection();
		cameramovspeed = 0.01;
	}
	speed = 0;
}

void setprojection()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	if( ortho )
		glOrtho( -(float)wWidth/wHeight, (float)wWidth/wHeight, -1, 1, -100, 100 );
	else
		gluPerspective( viewangle, (float)wWidth/wHeight, 0.001, 10000 );
}

inline void handleEvents()
{	
    while( SDL_PollEvent( &ev ) != 0 )
    {
        if( ev.type == SDL_QUIT )
        {
            running = false;
			return;
        }
		else if( ev.type == SDL_WINDOWEVENT )
		{
			if( ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
			{
				wWidth = ev.window.data1;
				wHeight = ev.window.data2;
				setprojection();
			}
		}
        else if( ev.type == SDL_KEYDOWN )
        {
			if( ev.key.keysym.sym == SDLK_LSHIFT || ev.key.keysym.sym == SDLK_RSHIFT )
			{
				shift_pressed = true;
			}
            else if( ev.key.keysym.sym == SDLK_KP_PLUS)     // zoom in
                zoom *= sqrt2_;
            else if( ev.key.keysym.sym == SDLK_KP_MINUS )   // zoom out
                zoom /= sqrt2_;
            else if( ev.key.keysym.sym == SDLK_r )          // change POV  // POV is custom if relind is -1, and POV = planets[relind] else
            {
				relind += shift_pressed ? -1 : 1;
				if( relind >= w.particles.size() )
					relind = -1;
				else if( relind < -1 )
					relind = w.particles.size() - 1;
            } 
            else if( ev.key.keysym.sym == SDLK_s )          // add ticks per frame
			{
                speed += shift_pressed ? -1 : 1;
				if( speed < 0 )
					speed = 0;
			}
			else if( ev.key.keysym.sym == SDLK_i )
			{
				w.ticktime *= -1;
			}
            else if( ev.key.keysym.sym == SDLK_p )          // print characteristics of simulation
			{
				if( shift_pressed )
				{
					drawing_points ^= 1;
				}
				else
				{
					out << "\nFPS: "<< fps << "\n";
					out << "Years: " << ttime / 86400 / 365.25 << "\n";
					out << (ortho ? "ortho\n" : "perspective\n") ;
					out << "speed: " << speed << "\n";
					out << "viewangle: " << viewangle << "\n";
					out << "camrx: " << camrx << "\n"
					    << "camry: " << camry << "\n"
						<< "camrz: " << camrz << "\n";
					out << "camx: " << camx << "\n"
					    << "camy: " << camy << "\n"
						<< "camz: " << camz << "\n";
				}
			}
			else if( ev.key.keysym.sym == SDLK_F5 )         // reset to default
			{
				setdefault();
			}
			else if( ev.key.keysym.sym == SDLK_t )          // increasing make simulation faster, but less accurate; decreasing make simulation more accurate, but slower
			{
				(shift_pressed) ? (w.ticktime /= sqrt2_) : (w.ticktime *= sqrt2_);
			}
			else if( ev.key.keysym.sym == SDLK_UP ||
			         ev.key.keysym.sym == SDLK_DOWN )       // rotate camera
		    {
				camrx += ( (ev.key.keysym.sym == SDLK_UP) ? 1: -1 ) * viewangle / 22.5;
			}
			else if( ev.key.keysym.sym == SDLK_LEFT ||
			         ev.key.keysym.sym == SDLK_RIGHT )      // rotate camera
			{
				camry += ( (ev.key.keysym.sym == SDLK_LEFT) ? 1: -1 ) * viewangle / 22.5;
			}
			else if( ev.key.keysym.sym == SDLK_LEFTBRACKET ||
			         ev.key.keysym.sym == SDLK_RIGHTBRACKET ) // rotate camera
			{
				camrz += ( (ev.key.keysym.sym == SDLK_LEFTBRACKET) ? -1: 1 ) * viewangle / 22.5;
			}
			else if( ev.key.keysym.sym == SDLK_KP_5 ||
			         ev.key.keysym.sym == SDLK_KP_0 )       // move camera
			{
				camz += (ev.key.keysym.sym == SDLK_KP_5 ? 1:-1 ) * cameramovspeed ;
			}
			else if( ev.key.keysym.sym == SDLK_KP_6 ||
                     ev.key.keysym.sym == SDLK_KP_4 )       // move camera
			{
				camx += (ev.key.keysym.sym == SDLK_KP_6 ? 1:-1 ) * cameramovspeed ;
			}
			else if( ev.key.keysym.sym == SDLK_KP_8 ||
                     ev.key.keysym.sym == SDLK_KP_2 )       // move camera
			{
				camy += (ev.key.keysym.sym == SDLK_KP_8 ? 1:-1 ) * cameramovspeed ;
			}
			else if( ev.key.keysym.sym == SDLK_o )          // view orthographic/perspective
			{
				ortho = !ortho;
				setdefault();
			}
			else if( ev.key.keysym.sym == SDLK_a )          // modify view angle
			{
				viewangle += (( !shift_pressed ) ? 1 : -1);
				setprojection();
			}
			else if( ev.key.keysym.sym == SDLK_KP_9 ||
			         ev.key.keysym.sym == SDLK_KP_7 )       // modify speed of moving camera
			{
				cameramovspeed *= ( ev.key.keysym.sym == SDLK_KP_9 ? sqrt2_ : 1/sqrt2_ );
			}
			else if( ev.key.keysym.sym == SDLK_PAGEUP ||
			         ev.key.keysym.sym == SDLK_PAGEDOWN )
			{
				movVline += ( ev.key.keysym.sym == SDLK_PAGEUP ? 1 : -1 ) * cameramovspeed;
			}
			else if( ev.key.keysym.sym == SDLK_g )          // stop/start visualisation
			{
				rendering = !rendering;
			}
			else if( ev.key.keysym.sym == SDLK_j )
			{
				clearing ^= 1;
			}
			else if( ev.key.keysym.sym == SDLK_e )          // print all particles
			{
				out << "\n" << w.particles.size() << endl;
				for( auto p : w.particles )
				{
					out << p.coords.x <<" "<< p.coords.y <<" "<< p.coords.z
					    <<" "<< p.velocity.x <<" "<< p.velocity.y <<" "<< p.velocity.z
						<<" "<< p.mass <<" "<< p.radius << endl;
				}
			}
        }
		else if( ev.type == SDL_KEYUP )
		{
			if( ev.key.keysym.sym == SDLK_LSHIFT || ev.key.keysym.sym == SDLK_RSHIFT )
				shift_pressed = false;
		}
    }
}

inline void update()
{
    if( speed )
	{
		w.tick( speed );
		ttime += w.ticktime*speed;
	}
	curt = (float)clock() / CLOCKS_PER_SEC;
    fps = 1 / ( curt - lastt );
    lastt = curt;
}

inline void drawCircle( double x, double y, double z, double radius, int cnt )
{
    glBegin( GL_TRIANGLE_FAN );
		glVertex3f( 0, 0, z );
        for( int i = 0; i <= cnt; i++ )
        {
            double cx = x + radius * sin( 2*pi_ / cnt * (i-1) ),
                   cy = y + radius * cos( 2*pi_ / cnt * (i-1) );
            glVertex3f( cx, cy, z );
			       cx = x + radius * sin( 2*pi_ / cnt * (i+1) ),
                   cy = y + radius * cos( 2*pi_ / cnt * (i+1) );
            glVertex3f( cx, cy, z );
			
        }
    glEnd();
}

inline void drawPoint( double x, double y, double z, bool withGLBegin )
{
    if( withGLBegin )
        glBegin( GL_POINTS );
    
    glVertex3f( x, y, z );
    
    if( withGLBegin )
        glEnd();
}

inline void drawCube( double x, double y, double z, double a )
{
	glColor3f( 1, 1, 1 ); 
	glBegin( GL_LINE_STRIP );
	  glVertex3f( a/2+x, a/2+y, a/2+z );
	  glVertex3f( a/2+x, -a/2+y, a/2+z );
	  glVertex3f( a/2+x, -a/2+y, -a/2+z );
	  glVertex3f( a/2+x, a/2+y, -a/2+z );
	  glVertex3f( a/2+x, a/2+y, a/2+z );
	  
	  glVertex3f( -a/2+x, a/2+y, a/2+z );
	  glVertex3f( -a/2+x, -a/2+y, a/2+z );
	  glVertex3f( -a/2+x, -a/2+y, -a/2+z );
	  glVertex3f( -a/2+x, a/2+y, -a/2+z );
	  glVertex3f( -a/2+x, a/2+y, a/2+z );
	glEnd();
	glBegin( GL_LINES );
	  glVertex3f( a/2+x, -a/2+y, a/2+z );
	  glVertex3f( -a/2+x, -a/2+y, a/2+z );
	  glVertex3f( a/2+x, a/2+y, -a/2+z );
	  glVertex3f( -a/2+x, a/2+y, -a/2+z );
	  glVertex3f( a/2+x, -a/2+y, -a/2+z );
	  glVertex3f( -a/2+x, -a/2+y, -a/2+z );
	glEnd();
	glBegin( GL_POINTS );
	  glVertex3f( -a/2+x, -a/2+y, -a/2+z );
	glEnd();
}

inline void drawSpriteCircle( double x, double y, double z, double radius, int cnt )
{
	glTranslatef( x, y, z );
	glRotatef( 90, 1, 0, 0 );
	glRotatef( camry, 0, 1, 0 );
	glRotatef( camrx, 1, 0, 0 );
	glRotatef( camrz, 0, 0, 1 );
	drawCircle( 0, 0, 0, radius, cnt );
	glRotatef( -camrz, 0, 0, 1 );
	glRotatef( -camrx, 1, 0, 0 );
	glRotatef( -camry, 0, 1, 0 );
	glRotatef( -90, 1, 0, 0 );
	glTranslatef( -x, -y, -z );
}

void modifyCoordsS( bool toCameraNotToReal, bool reset )
{
	if( toCameraNotToReal )
	{
		//glPushMatrix();
		if( relind != -1 && relind < w.particles.size() )
		{
			glTranslatef( w[relind].coords.x * zoom,
						  w[relind].coords.y * zoom,
						  w[relind].coords.z * zoom );
		}
		else
		{
			glTranslatef( camx, camy, camz );
		}
		glRotatef( 90, 1, 0, 0 );
		glRotatef( camry, 0, 1, 0 );
		glRotatef( camrx, 1, 0, 0 );
		glRotatef( camrz, 0, 0, 1 );
		glTranslatef( 0, 0, movVline );
	}
	else
	{
		if( reset )
		{
			setprojection();
			return;
		}
		glTranslatef( 0, 0, -movVline );
		glRotatef( -camrz, 0, 0, 1 );
		glRotatef( -camrx, 1, 0, 0 );
		glRotatef( -camry, 0, 1, 0 );
		glRotatef( -90, 1, 0, 0 );
		if( relind != -1 && relind < w.particles.size() )
		{
			glTranslatef( -w[relind].coords.x * zoom,
						  -w[relind].coords.y * zoom,
						  -w[relind].coords.z * zoom );
		}
		else
		{
			glTranslatef( -camx, -camy, -camz );
		}
	}
}

inline void render()
{
	const int len = 11;
    const int r[len] = { 255, 196, 180,  37, 155, 254, 127, 214, 208, 56,  255 };
    const int g[len] = { 250,   168, 174, 120, 149, 136, 106, 185, 246, 83,  255 };
    const int b[len] = { 240,   146, 160, 194, 148,  98,  86, 119, 247, 212, 255 };
    if(clearing)glClear( GL_COLOR_BUFFER_BIT );
    glViewport( 0, 0, wWidth, wHeight );
	
	modifyCoordsS( false, false );
	
	glBegin( GL_LINES );  // axes
	  glColor3f( 1, 0, 0 );
	  glVertex3f( 0, 0, 0 ); glVertex3f( 1, 0, 0 );
	  glColor3f( 0, 1, 0 );
	  glVertex3f( 0, 0, 0 ); glVertex3f( 0, 1, 0 );
	  glColor3f( 0, 0, 1 );
	  glVertex3f( 0, 0, 0 ); glVertex3f( 0, 0, 1 );
	glEnd();
	
    for( int i = 0; i < w.particles.size(); i++ )
    {
		if( i == relind && movVline < 0.001 )
			continue;
        glColor4f( r[(i<len) ? (i%len) : (len)] / 255., 
		           g[(i<len) ? (i%len) : (len)] / 255.,
				   b[(i<len) ? (i%len) : (len)] / 255.,
				   1 );
		if( drawing_points ) drawPoint( w[i].coords.x * zoom,
			                            w[i].coords.y * zoom,
				                        w[i].coords.z * zoom );
		drawSpriteCircle( w[i].coords.x * zoom,
			              w[i].coords.y * zoom,
				          w[i].coords.z * zoom,
						  w[i].radius * zoom );
    }
	
	modifyCoordsS( true );
	
	SDL_GL_SwapWindow( gWindow );
}

inline void close()
{    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
	w.particles.clear();

    SDL_Quit();
}

