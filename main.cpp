#include <iostream>
#include <cstdlib>
#include <fstream>
#include "GL/glut.h"

using namespace std;

static unsigned int texture[5]; // Array of texture indices.
static int xPos=0,yPos=-6,zPos=0;
static int xAngle=0,yAngle=0,zAngle =0;
static float ceilingRotation=0;

static int fogMode = GL_LINEAR; // Fog mode.
static float fogDensity = 0.1; // Fog density.
static float fogStart = 0.0; // Fog start z value.
static float fogEnd = 100.0; // Fog end z value

  float lightPos[] = { -30, 30,-30, 1.0 }; // Spotlight position.
   float spotDirection[] = {0, -1.0, 0.0}; // Spotlight direction.  
   float spotExponent = 10.0; // Spotlight exponent = attenuation.
   
   // Light property vectors.
  float lightAmb[] = { 0.03, 0.03, 0.03, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };

// Struct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
   
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
  
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
 
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4); 
   
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   { 
      temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
   }

   return bmp;
}


// Load external textures
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[5]; 
   
   // Load the textures.
   image[0] = getBMPData("grass.bmp");
   image[1] = getBMPData("sky.bmp");   
   image[2] = getBMPData("wood.bmp"); 
   image[3] = getBMPData("lamp.bmp"); 
   image[4] = getBMPData("lampshade.bmp");
   // Bind grass image to texture index[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);		
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[3]->sizeX, image[3]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[3]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[4]->sizeX, image[4]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[4]->data);
}

void drawScene(void)
{
	 float fogColor[4] = {0.5, 0.5, 0.5, 1.0};
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //   glEnable(GL_FOG);
    glHint(GL_FOG_HINT, GL_NICEST);
   glFogfv(GL_FOG_COLOR, fogColor);
   glFogi(GL_FOG_MODE, fogMode);
   glFogf(GL_FOG_START, fogStart);
   glFogf(GL_FOG_END, fogEnd);
   glFogf(GL_FOG_DENSITY, fogDensity);


   gluLookAt(0.0, 10.0, 15.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);

   glLoadIdentity();
   glRotatef(xAngle,1,0,0);
   glRotatef(yAngle,0,1,0);
   glRotatef(zAngle,0,0,1);
   glTranslatef(xPos,yPos,zPos);

     // Light properties.
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,35.0);
   glEnable(GL_LIGHT0); // Enable particular light source.

   // Map the grass texture onto a rectangle along the xz-plane.
    // Blend the night sky texture onto a rectangle parallel to the xy-plane.
   glBlendFunc(GL_ONE, GL_ZERO); // Specify blending parameters to overwrite background.
   glBindTexture(GL_TEXTURE_2D, texture[0]);        
   glBegin(GL_POLYGON);
      glNormal3f(0,1,0);glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, 100.0);
      glNormal3f(0,1,0);glTexCoord2f(8.0, 0.0); glVertex3f(100.0, 0.0, 100.0);
      glNormal3f(0,1,0);glTexCoord2f(8.0, 8.0); glVertex3f(100.0, 0.0, -100.0);
      glNormal3f(0,1,0);glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, 0.0, -100.0);
   glEnd();

   // Map the sky texture onto a rectangle parallel to the xy-plane.
    // Blend the night sky texture onto a rectangle parallel to the xy-plane.
   glBlendFunc(GL_ONE, GL_ZERO); // Specify blending parameters to overwrite background.
   glBindTexture(GL_TEXTURE_2D, texture[1]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[2]);
	glBegin(GL_POLYGON);//front Wall
		glNormal3f(0,0,-1);glTexCoord2f(0,0);glVertex3f(-40,0,40);
		glNormal3f(0,0,-1);glTexCoord2f(8,0);glVertex3f(40,0,40);
		glNormal3f(0,0,-1);glTexCoord2f(8,8);glVertex3f(40,40,40);
		glNormal3f(0,0,-1);glTexCoord2f(0,8);glVertex3f(-40,40,40);
	glEnd();
	glBegin(GL_POLYGON);//left Wall
		glNormal3f(-1,0,0);glTexCoord2f(0,0);glVertex3f(-40,0,-40);
		glNormal3f(-1,0,0);glTexCoord2f(8,0);glVertex3f(-40,0,40);
		glNormal3f(-1,0,0);glTexCoord2f(8,8);glVertex3f(-40,40,40);
		glNormal3f(-1,0,0);glTexCoord2f(0,8);glVertex3f(-40,40,-40);
	glEnd();
	glBegin(GL_POLYGON);//right Wall
		glNormal3f(-1,0,0);glTexCoord2f(0,0);glVertex3f(40,0,-40);
		glTexCoord2f(8,0);glVertex3f(40,0,40);
		glTexCoord2f(8,8);glVertex3f(40,40,40);
		glTexCoord2f(0,8);glVertex3f(40,40,-40);
	glEnd();
	glBegin(GL_POLYGON);//top
		glNormal3f(0,-1,0);glTexCoord2f(0,0);glVertex3f(-40,40,40);
		glNormal3f(0,-1,0);glTexCoord2f(8,0);glVertex3f(40,40,40);
		glNormal3f(0,-1,0);glTexCoord2f(8,8);glVertex3f(40,40,-40);
		glNormal3f(0,-1,0);glTexCoord2f(0,8);glVertex3f(-40,40,-40);
	glEnd();
	glBegin(GL_POLYGON);//bottem
		glNormal3f(0,1,0);glTexCoord2f(0,0);glVertex3f(-40,1,40);
		glNormal3f(0,1,0);glTexCoord2f(8,0);glVertex3f(40,1,40);
		glNormal3f(0,1,0);glTexCoord2f(8,8);glVertex3f(40,1,-40);
		glNormal3f(0,1,0);glTexCoord2f(0,8);glVertex3f(-40,1,-40);
	glEnd();
	 glBegin(GL_POLYGON);//Back Wall top
		glNormal3f(0,0,-1);glTexCoord2f(0,0);glVertex3f(-40,20,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,0);glVertex3f(40,20,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,8);glVertex3f(40,40,-40);
		glNormal3f(0,0,-1);glTexCoord2f(0,8);glVertex3f(-40,40,-40);
	glEnd();
	 glBegin(GL_POLYGON);//Back Wall bottem
		glNormal3f(0,0,-1);glTexCoord2f(0,0);glVertex3f(-40,1,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,0);glVertex3f(40,1,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,8);glVertex3f(40,12,-40);
		glNormal3f(0,0,-1);glTexCoord2f(0,8);glVertex3f(-40,12,-40);
	glEnd();
	 glBegin(GL_POLYGON);//Back Wall left most wood square
		glNormal3f(0,0,-1);glTexCoord2f(0,0);glVertex3f(-40,12,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,0);glVertex3f(-20,12,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,8);glVertex3f(-20,20,-40);
		glNormal3f(0,0,-1);glTexCoord2f(0,8);glVertex3f(-40,20,-40);
	glEnd();
	glBegin(GL_POLYGON);//Back Wall right most wood square
		glNormal3f(0,0,-1);glTexCoord2f(0,0);glVertex3f(20,12,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,0);glVertex3f(40,12,-40);
		glNormal3f(0,0,-1);glTexCoord2f(8,8);glVertex3f(40,20,-40);
		glNormal3f(0,0,-1);glTexCoord2f(0,8);glVertex3f(20,20,-40);
	glEnd();

		  glColor4f(1.0, 1.0, 1.0, .2); 
	   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters to mix skies.   
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-20.0, 12.0, -40.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(20.0, 12.0, -40.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(20.0, 20.0, -40.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-20.0, 20.0, -40.0);
   glEnd();
   glEnable(GL_DEPTH_TEST);

   // Draw a white line in the direction of the sun with lighting still disabled.
   glColor3f(1.0, 1.0, 1.0);
   glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glBegin(GL_POLYGON);// Bracket for ceiling fan
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-1,38,-1);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(-1,38,1);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(-1,40,1);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(-1,40,-1);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(1,38,-1);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(1,38,1);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(1,40,1);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(1,40,-1);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-1,38,-1);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(1,38,-1);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(1,40,-1);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(-1,40,-1);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-1,38,1);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(1,38,1);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(1,40,1);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(-1,40,1);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	glBegin(GL_POLYGON);// Bracket for ceiling fan
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-.5,37,-.5);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(-.5,37,.5);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(-.5,38,.5);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(-.5,38,-.5);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(.5,37,-.5);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(.5,37,.5);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(.5,38,.5);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(.5,38,-.5);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-.5,37,-.5);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(.5,37,-.5);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(.5,38,-.5);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(-.5,38,-.5);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-.5,37,.5);
		glNormal3f(0,0,1);glTexCoord2f(8,0);glVertex3f(.5,37,.5);
		glNormal3f(0,0,1);glTexCoord2f(8,8);glVertex3f(.5,38,.5);
		glNormal3f(0,0,1);glTexCoord2f(0,8);glVertex3f(-.5,38,.5);
	}
	glEnd();

	glRotatef(ceilingRotation,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-1,38,5);
		glNormal3f(0,0,1);glTexCoord2f(1,0);glVertex3f(1,38,5);
		glNormal3f(0,0,1);glTexCoord2f(1,1);glVertex3f(1,38,-5);
		glNormal3f(0,0,1);glTexCoord2f(0,1);glVertex3f(-1,38,-5);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glNormal3f(0,0,1);glTexCoord2f(0,0);glVertex3f(-5,38,1);
		glNormal3f(0,0,1);glTexCoord2f(1,0);glVertex3f(5,38,1);
		glNormal3f(0,0,1);glTexCoord2f(1,1);glVertex3f(5,38,-1);
		glNormal3f(0,0,1);glTexCoord2f(0,1);glVertex3f(-5,38,-1);
	}
	glEnd();
	
	ceilingRotation +=.09;

	
	glutPostRedisplay();
   glutSwapBuffers();	
}
// Initialization routine.
void setup(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

  // glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Create texture index array and load external textures.
   glGenTextures(5, texture);
   loadExternalTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	
   glEnable(GL_BLEND);
}
// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
   case 'w':
	   zPos++;
	   glutPostRedisplay();
	   break;
   case 'W':
	   zPos--;
	   glutPostRedisplay();
	   break;
   case 'x':
	   xAngle++;
	   glutPostRedisplay();
	   break;
   case 'X':
	   xAngle--;
	   glutPostRedisplay();
	   break;
   case 'y':
	   yAngle++;
	   glutPostRedisplay();
	   break;
   case 'Y':
	   yAngle--;
	   glutPostRedisplay();
	   break;
   case 'z':
	   zAngle++;
	   glutPostRedisplay();
	   break;
   case 'Z':
	   zAngle--;
	   glutPostRedisplay();
	   break;
   case 'c':
		xAngle=0;
		yAngle=0;
		zAngle=0;
		xPos=0;
		yPos= -6;
		zPos=0;
		glutPostRedisplay();
		break;
      default:
         break;
   }
}
// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP)
	   if(yPos>-35)
       yPos--;
  if(key == GLUT_KEY_DOWN)
	  if(yPos<-6)
		yPos++;
  if(key == GLUT_KEY_RIGHT)
	  if(xPos>-35)
	  xPos--;
   if(key == GLUT_KEY_LEFT)
	 if(xPos< 35)
		 xPos++;
	cout<<xPos<<yPos<<zPos<<endl;
   glutPostRedisplay();
}
void printInteraction(void)
{
   cout << "Interaction:" << endl;

}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Final Assignment");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();

   return 0; 
}