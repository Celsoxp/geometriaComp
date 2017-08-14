#include <stdio.h>
#include <GL/glut.h>

/* Functions. */
void Init(void);
void Reshape(int w, int h);
void Display(void);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void DrawPoints(void);
void DrawLines();


/* Types. */
typedef struct point_2d
{
    double x;
    double y;

} point2d;

/* Defines. */
#define MAXNPTS 1000

/* Globals */
int winwidth, winheight;
int npts = 0;
point2d points[MAXNPTS];

/* Functions */
void Init(){
    
    /* Select clearing color. */
    glClearColor(1.0, 1.0, 1.0, 0.0);
}


int AreaTri(point2d a,point2d b,point2d c){
	return ((b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x));
}

int left(point2d a,point2d b,point2d c){
	return AreaTri(a,b,c)>0;
}

int colin(point2d a,point2d b,point2d c){
	return AreaTri(a,b,c)==0;
}


void Reshape(int w, int h){

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, 512.0, 0.0, 512.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   winwidth  = w;
   winheight = h;
}

void Keyboard(unsigned char key, int x, int y){

    switch (key){
        case 27:
            exit(0);
            break;
        case 'q':
        case 'Q':
            exit(0);
            break;
    }
}

void Mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                if (npts < MAXNPTS)
                {
		    points[npts].x = x;
                    points[npts].y = 512.0-y;
		    printf("(%f,%f)\n", points[npts].x, points[npts].y);
                    npts++;
                }
                
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
                npts = 0;
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}

void DrawPoints()
{
    int i;

    glColor3f (0.0, 0.0, 0.0);
    glPointSize((GLdouble) 5);
    glBegin(GL_POINTS);
    for (i = 0; i < npts; i++)
        glVertex2d(points[i].x, points[i].y);
    glEnd();
}

void DrawLines()
{
	int i;

    glColor3f (0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for (i = 0; i < npts; i++)
        glVertex2d(points[i].x, points[i].y);
    glEnd();
}

void DrawTriangles(){
	int i;
    glBegin(GL_TRIANGLES);
    for (i = 0; i < npts-npts%3; i+=3){
		glColor3f (1.0, 0.0, 0.0);
		if(left(points[i],points[i+1],points[i+2]))
			glColor3f (0.0, 1.0, 0.0);
        glVertex2d(points[i].x, points[i].y);
        glVertex2d(points[i+1].x, points[i+1].y);
        glVertex2d(points[i+2].x, points[i+2].y);
	}
    glEnd();
}
    
void Display(void){
    
    /* Clear buffers. */
    glClear(GL_COLOR_BUFFER_BIT);
   
    /* Draw. */
    DrawTriangles();
    DrawPoints();

    glFlush();
}

int main(int argc, char** argv){

    /* Call glut init functions. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(512, 512); 
    glutCreateWindow("Test");
    Init();

    /* Call glut drawing functions. */
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutDisplayFunc(Display);

    glutMainLoop();
    return 0;
}
