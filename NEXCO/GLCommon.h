#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
	#ifdef _DEBUG
		#define GL_PATH "C:\\freeglut\\install\\lib\\Debug\\"
		#define GL_EXT_STR "d.lib"
	#else
		#define GL_PATH "C:\\freeglut\\install\\lib\\Release\\"
		#define GL_EXT_STR ".lib"
	#endif

	#pragma comment(lib, GL_PATH "freeglut_static" GL_EXT_STR)
#endif

#define REFRESH_RATE 30
