#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include "smfobj.h"

#include "../../newmat/newmat.h"
#include "../../newmat/newmatio.h"
#include "../../newmat/newmatap.h"

#define PILOT	  0
#define SPECTATOR 1

using namespace std;



float sealevel;
float polysize;
Matrix currPos(3,1);
Matrix lookat(3,1);
Matrix ref(3,1);
Matrix up(3,1);



float speed;
float nFPS=60;


SmfObj plane;

Matrix R(3,3);
bool flag=false;

Matrix planeR(3,3);

int mode=PILOT;

GLfloat white[] = {1.0,1.0,1.0,1.0};
GLfloat lpos[] = {0.0,1.0,0.0,0.0};

void cross(Matrix const & A, Matrix const & B, Matrix & ret)
{
	
//	Matrix ret(3,1);
	ret<< A(2,1)*B(3,1) - A(3,1)*B(2,1)
	   << A(3,1)*B(1,1) - A(1,1)*B(3,1)
	   << A(1,1)*B(2,1) - A(2,1)*B(1,1);
	return;
}



	
void init() 
{
	plane.ObjRead("plane.smf");
	plane.getFaceNormals();	//calculate all the face normals since they are not given


	speed =.003; //initialize speed
		
	currPos<<.6<<.6<<.8; //initialize plane position
	lookat<<1<<0<<-1;//initialize lookat
	lookat=lookat/sqrt(lookat.SumSquare());	//normalize lookat
	up << 0 <<0 << 1;	//initialize lookat
	
	
	cross(lookat,up,ref);	//ref=lookat x up
//	cout<<"Ref sum: "<<ref.SumSquare()<<endl;
//	
//	cout<<"lookAt"<<endl<<lookat;
//	cout<<"ref"<<endl<<ref;
//	cout<<"up"<<endl<<up;
	


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glClearColor (0.5, 0.5, 1.0, 0.0);
	/* glShadeModel (GL_FLAT); */
	glEnable(GL_DEPTH_TEST);

	sealevel = 0.0;
	polysize = 0.01;
}


int seed(float x, float y) {
    static int a = 1588635695, b = 1117695901;
	int xi = *(int *)&x;
	int yi = *(int *)&y;
    return ((xi * a) % b) - ((yi * b) % a);
}

void mountain(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float s)
{
	float x01,y01,z01,x12,y12,z12,x20,y20,z20;

	if (s < polysize) {
		x01 = x1 - x0;
		y01 = y1 - y0;
		z01 = z1 - z0;

		x12 = x2 - x1;
		y12 = y2 - y1;
		z12 = z2 - z1;

		x20 = x0 - x2;
		y20 = y0 - y2;
		z20 = z0 - z2;

		float nx = y01*(-z20) - (-y20)*z01;
		float ny = z01*(-x20) - (-z20)*x01;
		float nz = x01*(-y20) - (-x20)*y01;

		float den = sqrt(nx*nx + ny*ny + nz*nz);

		if (den > 0.0) {
			nx /= den;
			ny /= den;
			nz /= den;
		}

		glNormal3f(nx,ny,nz);
		glBegin(GL_TRIANGLES);
			glVertex3f(x0,y0,z0);
			glVertex3f(x1,y1,z1);
			glVertex3f(x2,y2,z2);
		glEnd();

		return;
	}

	x01 = 0.5*(x0 + x1);
	y01 = 0.5*(y0 + y1);
	z01 = 0.5*(z0 + z1);

	x12 = 0.5*(x1 + x2);
	y12 = 0.5*(y1 + y2);
	z12 = 0.5*(z1 + z2);

	x20 = 0.5*(x2 + x0);
	y20 = 0.5*(y2 + y0);
	z20 = 0.5*(z2 + z0);

	s *= 0.5;

	srand(seed(x01,y01));
	z01 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
	srand(seed(x12,y12));
	z12 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
	srand(seed(x20,y20));
	z20 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);

	mountain(x0,y0,z0,x01,y01,z01,x20,y20,z20,s);
	mountain(x1,y1,z1,x12,y12,z12,x01,y01,z01,s);
	mountain(x2,y2,z2,x20,y20,z20,x12,y12,z12,s);
	mountain(x01,y01,z01,x12,y12,z12,x20,y20,z20,s);
}







//*********Rotate 2 direction vectors (ret1, ret2)  about axis (std) for theta angle
void rotateAxis(Matrix & std, Matrix & ret1, Matrix & ret2, float theta)
{


//	cout<<"std det: "<<std.SumSquare();
	flag=true;
	
	std=std/sqrt(std.SumSquare()); //nomalize std

	float nx = std(1,1);
	float ny = std(2,1);
	float nz = std(3,1);
	
	//from book http://tom.cs.byu.edu/~455/hw/rotation.pdf
	//equation 26 rotation about arbitrary axis
	//since there is no translation for the direction vectors, it is just a 3x3 rotation matrix
	
	
	R(1,1)= (nx*nx)+(ny*ny+nz*nz)*cos(theta);
	R(1,2)= nx*ny*(1-cos(theta)) - nz * sin(theta);
	R(1,3)= nx*nz*(1-cos(theta)) + ny*sin(theta);
	
	R(2,1)= (nx*ny)*(1-cos(theta))+ nz*sin(theta);
	R(2,2)= ny*ny + (nx*nx+nz*nz)*cos(theta);
	R(2,3)= ny*nz*(1-cos(theta))-nx*sin(theta);
	
	R(3,1)= nx*nz*(1-cos(theta)) - ny*sin(theta);
	R(3,2)= ny*nz*(1-cos(theta)) + nx*sin(theta);
	R(3,3)= nz*nz + (nx*nx + ny*ny)* cos(theta);
	
	//	R=R/sqrt(R.SumSquare()); //normalize to elimation calmulative numerical hazard
	
//	cout<<"R det: "<<R.Determinant();
	
	
	//rotate ret1 and normalize
	ret1=R*ret1;
	ret1=ret1/sqrt(ret1.SumSquare());
	ret2=R*ret2;
	ret2=ret2/sqrt(ret2.SumSquare());
	
	
//	cout<<"ret2"<<endl<<ret2;
	
//	cout<<"std"<<endl<<std;
}

void display(void)
{
	static GLfloat angle = 0.0;

	GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
	GLfloat tandiff[] = {0.4,0.3,0.2,1.0};

	GLfloat seaamb[] = {0.0,0.0,0.2,1.0};
	GLfloat seadiff[] = {0.0,0.0,1,1.0};
	GLfloat seaspec[] = {0.5,0.5,1.0,1.0};

//cout<<"ref "<<ref.SumSquare()<<endl<<ref;
//cout<<"lookat "<<lookat.SumSquare()<<endl<<lookat;
//cout<<"up "<<lookat.SumSquare()<<endl<<up;


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();             /* clear the matrix */
			/* viewing transformation  */
			
			
	currPos=currPos+lookat*speed;	//update the Current Position
	switch(mode)
	{
		case PILOT:
			gluLookAt(	currPos(1,1), currPos(2,1), currPos(3,1), 	//eye position
					currPos(1,1)+lookat(1,1),currPos(2,1)+lookat(2,1), currPos(3,1)+lookat(3,1), //lookat target position	
					up(1,1),up(2,1),up(3,1)); //up direction
			break;	
		case SPECTATOR:
			gluLookAt (.8,1.2,0.8, 	//eye position
					0,0,0, 	//lookat target position	
					0,0,1); //up direction	
			break;
	}
	
	glPushMatrix();	
	
		//reload lighting positions to fix the lighting issue
		glLightfv(GL_LIGHT0, GL_POSITION, lpos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, white);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	
	
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

		mountain(0.0,0.0,0.0, 1.0,0.0,0.0, 0.0,1.0,0.0, 1.0);
		mountain(1.0,1.0,0.0, 0.0,1.0,0.0, 1.0,0.0,0.0, 1.0);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

		glNormal3f(0.0,0.0,1.0);
		glBegin(GL_QUADS);
			glVertex3f(0.0,0.0,sealevel);
			glVertex3f(1.0,0.0,sealevel);
			glVertex3f(1.0,1.0,sealevel);
			glVertex3f(0.0,1.0,sealevel);
		glEnd();
	glPopMatrix();

		GLfloat planeAmb[] ={0,0,0,1.0};
		GLfloat planeDiff[] = {0.2,0.20,0.20,1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, planeAmb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, planeDiff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, planeDiff);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
//		glNormal3f(0.0,0.0,1.0);
	//draw the plane if in spectator mode
	if (mode==SPECTATOR)
	{
		glPushMatrix();
			glTranslatef(currPos(1,1),currPos(2,1),currPos(3,1));
	//		glTranslatef(.6, .7, .5);
		
		
//			cout<<"lookat: "<<endl<<lookat;
//			cout<<"loolat size: "<< lookat.SumSquare()<<endl;
			//EHH!!! IT GOES BY COLUMNS!!!!SO WHAT YOU SEE HERE IS THE TRANSPOSE
			float mat44[16]={(float)-lookat(1,1),	(float)-lookat(2,1),	(float)-lookat(3,1),	0,
					 (float)ref(1,1),	(float)ref(2,1),	(float)ref(3,1),	0,
					 (float)up(1,1),	(float)up(2,1),		(float)	up(3,1),	0,
					 0,		0,		0,		1};
			glMultMatrixf(mat44);
		
			glScalef(.00001,0.00001,0.00001);
//			readjust the coordinates of the plane
			glTranslatef(-7300,0,-1960);
			plane.draw();
	
		glPopMatrix();
	}

	glutSwapBuffers();
	glFlush ();

	angle += 1.0;

}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(90.0,1.0,0.01,10.0);
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
		case '-':
			sealevel -= 0.01;
			break;
		case '+':
		case '=':
			sealevel += 0.01;
			break;
		case 'f':
			polysize *= 0.5;
			break;
		case 'c':
			polysize *= 2.0;
			break;
		case 27:
			exit(0);
			break;
			
			
		case 'w':
			rotateAxis(ref,lookat,up,.05);	//pitch, rotate about ref
//			cross(ref,lookat,up);
			break;
		case 's':
			rotateAxis(ref,lookat,up,-.05);
//			cross(ref,lookat,up);
			break;
		case 'a':
			rotateAxis(lookat,ref,up,-.05);//roll, rotate about lookat
			break;
		case 'd':
			rotateAxis(lookat,ref,up,.05);
			break;
		case 'q':
			speed+=-.001;
			break;
		case 'e':
			speed+=.001;
			break;
		
	
		case '1': mode=PILOT;break;
		case '2': mode=SPECTATOR;break;
		
		case '`':
			init();
			break;
   }
}


void timer(int v)
{
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
	
	glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 600); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("FLIGHT SIM");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation
   glutMainLoop();
   return 0;
}
