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
double sphereRadius;
double cylinderHeight;
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
point position;
point upVector;
point rightVector;
point lookVector;
point CrossProduct(point p,point q){
    /*************
    |x  y   z|
    |x1 y1 z1|
    |x2 y2 z2|

    x(y1z2-z1y2)
    y(z1x2 - x1z2)
    z(x1y2 - x2y1)



    ******************/
    point cp;
    cp.x = p.y*q.z - p.z*q.y;
    cp.y = p.z*q.x - p.x*q.z;
    cp.z = p.x*q.y - p.y*q.x;
    return cp;
}
void drawAxes()
{
	if(drawaxes==1)
	{
		//glColor3f(1.0, 1.0, 1.0);
		//RGB
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

void drawSquare(double a)
{
    glColor3f(1,1,1);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}






void drawCylinder(double radius,double halfheight,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=halfheight;i++)
	{
		r=radius;
		h=i;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*0.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*0.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<halfheight;i++)
	{
      //  glColor3f(0.9,(double)i/(double)stacks,(double)i/(double)stacks);
      glColor3f(0,1,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}

void drawSphere(double radius,int slices,int stacks,int flag)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*0.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*0.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
       //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,0.9);
     glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    if(flag==1){
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			    }
                //lower hemisphere
                if(flag==-1){
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }
			}glEnd();
		}
	}
}



void drawCubeToSphere(){

    double trans= 25- sphereRadius;
    for(int i=-1;i<=1;i+=2){
    glPushMatrix();{
    glTranslatef(trans,trans,i*trans);
    drawSphere(sphereRadius,24,30,i);
    }
    glPopMatrix();
    glPushMatrix();{
    glTranslatef(-trans,trans,i*trans);
    glRotatef(90,0,0,1);
    drawSphere(sphereRadius,24,30,i);
    }
    glPopMatrix();
    glPushMatrix();{
    glTranslatef(trans,-trans,i*trans);
    glRotatef(-90,0,0,1);
    drawSphere(sphereRadius,24,30,i);
    }
    glPopMatrix();
    glPushMatrix();{

    glTranslatef(-trans,-trans,i*trans);
     glRotatef(180,0,0,1);
    drawSphere(sphereRadius,24,30,i);
    }
    glPopMatrix();
    }


    glPushMatrix();{
    glTranslatef(trans,trans,0);
    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    glPushMatrix();{
    glTranslatef(-trans,trans,0);
    glRotatef(90,0,0,1);
    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    glPushMatrix();{
    glTranslatef(trans,-trans,0);
    glRotatef(-90,0,0,1);
    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    glPushMatrix();{

    glTranslatef(-trans,-trans,0);
    glRotatef(180,0,0,1);
    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    /****corner ones*****/
    glPushMatrix();{

    glTranslatef(0,trans,-trans);
    glRotatef(90,0,1,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    glPushMatrix();{

    glTranslatef(0,trans,trans);
    glRotatef(-90,0,1,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();

    glPushMatrix();{

    glTranslatef(trans,0,trans);
    glRotatef(90,1,0,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
     glPushMatrix();{

    glTranslatef(trans,0,-trans);
    glRotatef(-90,1,0,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
/*************************/
    glPushMatrix();{

    glTranslatef(0,-trans,trans);
    glRotatef(90,0,-1,0);
    glRotatef(180,1,0,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    glPushMatrix();{

    glTranslatef(0,-trans,-trans);
    glRotatef(90,0,1,0);
    glRotatef(180,1,0,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();

    glPushMatrix();{
    glTranslatef(-trans,0,trans);
    glRotatef(90,1,0,0);
    glRotatef(180,0,1,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    glPushMatrix();{
    glTranslatef(-trans,0,-trans);
    glRotatef(90,-1,0,0);
    glRotatef(180,0,1,0);

    drawCylinder(sphereRadius,trans,20,30);
    }
    glPopMatrix();
    /*************squares*****/
    for(int i=-1;i<=1;i+=2){
    glPushMatrix();{
        glTranslatef(0,0,i*(trans+sphereRadius));
        drawSquare(trans);
    }
    glPopMatrix();
    }
    for(int i=-1;i<=1;i+=2){
    glPushMatrix();{
        // glRotatef(90,0,i,0);
        glTranslatef(i*(trans+sphereRadius),0,0);
        glRotatef(90,0,i,0);
        drawSquare(trans);
    }
    glPopMatrix();
    }
    for(int i=-1;i<=1;i+=2){
    glPushMatrix();{
        glTranslatef(0,i*(trans+sphereRadius),0);
        glRotatef(90,i,0,0);
        drawSquare(trans);
    }
    glPopMatrix();
    }



}
void keyboardListener(unsigned char key, int x,int y){
    double change = 0.03;
	switch(key){
//So, u = r X l //rotate l wrt r
//Then we scale l by cosA and scale u by sinA and take their sum
//u.r = r.l = l.u = 0, u =  r X l, l =  u X r, and r =  l X
		case '1':{
		    //w.r.t up vector
            point prevLook = lookVector;
            point prevUp = upVector;
            point prevRight = rightVector;
            rightVector.x = prevRight.x*cos(change) + prevLook.x*sin(change);
            rightVector.y = prevRight.y*cos(change) + prevLook.y*sin(change);
            rightVector.z = prevRight.z*cos(change) + prevLook.z*sin(change);

            lookVector = CrossProduct(upVector,rightVector);
		}
            break;
        case '2':{
		    //w.r.t up vector
            point prevLook = lookVector;
            point prevUp = upVector;
            point prevRight = rightVector;
            rightVector.x = prevRight.x*cos(-change) + prevLook.x*sin(-change);
            rightVector.y = prevRight.y*cos(-change) + prevLook.y*sin(-change);
            rightVector.z = prevRight.z*cos(-change) + prevLook.z*sin(-change);

            lookVector = CrossProduct(upVector,rightVector);
		}
            break;
        case '3':{
		    //w.r.t right vector
            point prevLook = lookVector;
            point prevUp = upVector;
            point prevRight = rightVector;
            lookVector.x = prevLook.x*cos(change) + prevUp.x*sin(change);
            lookVector.y = prevLook.y*cos(change) + prevUp.y*sin(change);
            lookVector.z = prevLook.z*cos(change) + prevUp.z*sin(change);

            upVector = CrossProduct(rightVector,lookVector);
		}
            break;
        case '4':{
		    //w.r.t right vector
            point prevLook = lookVector;
            point prevUp = upVector;
            point prevRight = rightVector;
            lookVector.x = prevLook.x*cos(-change) + prevUp.x*sin(-change);
            lookVector.y = prevLook.y*cos(-change) + prevUp.y*sin(-change);
            lookVector.z = prevLook.z*cos(-change) + prevUp.z*sin(-change);

            upVector = CrossProduct(rightVector,lookVector);
		}
            break;
        case '5':{
		    //w.r.t look vector
            point prevLook = lookVector;
            point prevUp = upVector;
            point prevRight = rightVector;
            upVector.x = prevUp.x*cos(change) + prevRight.x*sin(change);
            upVector.y = prevUp.y*cos(change) + prevRight.y*sin(change);
            upVector.z = prevUp.z*cos(change) + prevRight.z*sin(change);

            rightVector = CrossProduct(lookVector,upVector);
		}
            break;
        case '6':{
		    //w.r.t look vector
            point prevLook = lookVector;
            point prevUp = upVector;
            point prevRight = rightVector;
            upVector.x = prevUp.x*cos(-change) + prevRight.x*sin(-change);
            upVector.y = prevUp.y*cos(-change) + prevRight.y*sin(-change);
            upVector.z = prevUp.z*cos(-change) + prevRight.z*sin(-change);

            rightVector = CrossProduct(lookVector,upVector);
		}
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
    double change = 2.0;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
            position.x -= change*lookVector.x;
            position.y -= change*lookVector.y;
            position.z -= change*lookVector.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
		    position.x += change*lookVector.x;
            position.y += change*lookVector.y;
            position.z += change*lookVector.z;
			//cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
		    position.x += change*rightVector.x;
            position.y += change*rightVector.y;
            position.z += change*rightVector.z;
			//cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
            position.x -= change*rightVector.x;
            position.y -= change*rightVector.y;
            position.z -= change*rightVector.z;
			//cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
            position.x += change*upVector.x;
            position.y += change*upVector.y;
            position.z += change*upVector.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    position.x -= change*upVector.x;
            position.y -= change*upVector.y;
            position.z -= change*upVector.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(sphereRadius != 25) {
                sphereRadius+=1;
		    }
			break;
		case GLUT_KEY_END:
		    if(sphereRadius!=0) {
                 sphereRadius-=1;
		    }

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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
	gluLookAt(position.x, position.y, position.z, position.x + lookVector.x, position.y + lookVector.y,
            position.z + lookVector.z,upVector.x, upVector.y, upVector.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objectsition

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

   // drawSphere(10,10,10);

    //drawCircle(30,24);

    //drawCone(20,50,24);
   // drawCylinder(20,30,24,20);
   drawCubeToSphere();

//	drawSphere(30,24,20);




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
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	upVector = point(0,0,1);
	sphereRadius = 10.0;
	cylinderHeight = 10.0;
	rightVector = point(-1.0/sqrt(2), 1.0/sqrt(2), 0);
	lookVector = point(-1.0/sqrt(2), -1.0/sqrt(2), 0);
	position = point(100,100,0);
	//  u = (0, 0, 1), r = (-1/√2, 1/√2, 0),  l = (-1/√2, -1/√2, 0), and pos = (100, 100, 0

	//clear the screen
	glClearColor(0,0,0,0);

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
