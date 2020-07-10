#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
	point(){
	    x=0;
	    y=0;
	    z=0;
	}
	point(double x1,double y1,double z1){
	    x=x1;
	    y=y1;
	    z=z1;
	}
	printPoint(){
	    printf("(%f,%f,%f)",&x,&y,&z);
	}
};

point wheelPosition;
point a1;
point a2;
double wheelRadius;
double wheelWidth;
double wheelAngle;
double tilt;
double distance;
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
//X-AXIS is RED
		    glColor3f(1.0, 0, 0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			//Y-AXIS IS GREEN
            glColor3f(0, 1.0,0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			//Z-AXIS IS BLUE
            glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}








void drawCylinder(double radius,double height,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=height;i++)
	{
		r=radius;
		h=i;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	int k = 0;
	for(j=0;j<slices;j++)
	{

      glColor3f((double)k/(double)stacks,(double)k/(double)stacks,(double)k/(double)stacks);
     if(j<slices/2)
       k+=2;
     else
        k-=2;
            for(i=0;i<height;i++)
		{

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}

}
void drawWheel(){
    double perimeter = 2*pi*wheelRadius;
    tilt = 360*distance/perimeter;

    glTranslatef(wheelPosition.x,wheelPosition.y,wheelPosition.z);
    glRotatef(wheelAngle,0,0,1);
     glTranslatef(0,wheelWidth/2,wheelRadius);
    glRotatef(tilt,0,wheelRadius,0);
    //
    glRotatef(90,1,0,0);
    //


    drawCylinder(wheelRadius,wheelWidth,80,80);
    double spikeWidth = wheelWidth/2 - 4;
    point end1 = point(0,wheelRadius,wheelWidth/2);
    point end2 = point(0,-wheelRadius,wheelWidth/2);
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(end1.x,end1.y,end1.z+spikeWidth);
				glVertex3f(end1.x,end1.y,end1.z-spikeWidth);
				glVertex3f(end2.x,end2.y,end2.z-spikeWidth);
				glVertex3f(end2.x,end2.y,end2.z+spikeWidth);
			}glEnd();
     end1 = point(wheelRadius,0,wheelWidth/2);
     end2 = point(-wheelRadius,0,wheelWidth/2);
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(end1.x,end1.y,end1.z+spikeWidth);
				glVertex3f(end1.x,end1.y,end1.z-spikeWidth);
				glVertex3f(end2.x,end2.y,end2.z-spikeWidth);
				glVertex3f(end2.x,end2.y,end2.z+spikeWidth);
			}glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
    double perimeter = 2*pi*wheelRadius;
        double dis;//= tilt/perimeter;

	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'w':

                tilt+=5;

                wheelPosition.x+=2*cos(wheelAngle*pi/180.0);
                wheelPosition.y+=2*sin(wheelAngle*pi/180.0);
                distance+=2;
                break;
            case 's':

                tilt-=5;

                wheelPosition.x-=2*cos(wheelAngle*pi/180.0);
                wheelPosition.y-=2*sin(wheelAngle*pi/180.0);
                distance-=2;
                break;
            case 'a':
                wheelAngle+=2;
                break;
            case 'd':
                wheelAngle-=2;
                break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);
    drawWheel();
    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

    /**********************wheel**********/

    wheelRadius = 40;
    wheelWidth = 18;
    wheelPosition = point(0,0,0);
    wheelAngle = 0;
    a2 = point(1,0,0);
    tilt = 0;
    distance = 0;
	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
