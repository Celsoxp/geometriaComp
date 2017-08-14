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

typedef struct polygon
{
	int n;
    point2d at;
	struct polygon *next;
} polygon;

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

int lefton(point2d a,point2d b,point2d c){
	return AreaTri(a,b,c)>=0;
}

int colin(point2d a,point2d b,point2d c){
	return AreaTri(a,b,c)==0;
}

int intersecP(point2d a,point2d b,point2d c,point2d d){
	return (left(a, b, c)!=left(a, b, d)) && (left(c,d,a)!=left(c,d,b));
}

int intersecI(point2d a,point2d b,point2d c,point2d d){
	return colin(a, b, c) || colin(a, b, d) || colin(c,d,a) || colin(c,d,b);
}

int intersecIP(point2d a,point2d b,point2d c,point2d d){
	return (lefton(a, b, c)!=lefton(a, b, d)) && (lefton(c,d,a)!=lefton(c,d,b));
}

int polyIntersec(polygon *p, point2d a, point2d b){
	polygon *aux;
	for(aux=p;aux->next!=p;aux=aux->next){
		if(intersecIP(a,b,aux->at,aux->next->at))
			return 1;
	}
	if(intersecIP(a,b,aux->at,aux->next->at))
		return 1;
	return 0;
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
        case 't':
			DrawTriangles();
			break;
		case 'i':
			if(npts>3){
				if(intersecIP(points[npts-1],points[npts-2],points[npts-3],points[npts-4]))
					printf("\n Oh yeah bro \n");
			}
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
    
void Display(void){
    
    /* Clear buffers. */
    glClear(GL_COLOR_BUFFER_BIT);
   
    /* Draw. */
    DrawLines();
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
