bool init();
bool initGL();
inline void handleEvents();
inline void update();
inline void render();
inline void close();

inline void drawCircle( double x, double y, double radius, int cnt = 256 );
inline void drawPoint( double x, double y, double z = 0, bool withGLBegin = true );
inline void drawCube( double x, double y, double z, double a = 1 );
inline void drawSpriteCircle( double x, double y, double z, double radius, int cnt = 256 );
void setdefault();
void setprojection();
void modifyCoordsS( bool toCameraNotToReal = true, bool reset = false );