#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <GL/glu.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/gl.h>

#define PI 3.141592654

GLUquadric *quad;
GLUquadric *Mercury;
GLUquadric *Venus;
GLUquadric *Earth;
GLUquadric *Moon;
GLUquadric *Mars;
GLUquadric *Jupiter;
GLUquadric *Saturn;
GLUquadric *Uranus;
GLUquadric *Neptun;

float transX = 60.0;
float transY = 0.0;

GLuint texture[11];
int stars[101][101];

static float mercury = 360.0 / 88.0;
static float venus = 360.0 / 224.7;
static float earth = 360.0 / 365.3; 
static float mars = 360.0 / 687.0;
static float jupiter = 360.0 / 4328.9;
static float saturn = 360.0 / 10585.0;
static float uranus = 360.0 / 30660.0;
static float neptun = 360.0 / 60225.0;

const int mercuryRad = 200;
const int venusRad = mercuryRad + 150;
const int earthRad = venusRad + 150;
const int marsRad = earthRad + 150;
const int jupiterRad = marsRad + 200;
const int saturnRad = jupiterRad + 200;
const int uranusRad = saturnRad + 150;
const int neptunRad = uranusRad + 150;

static float mercuryAngle;
static float venusAngle;
static float earthAngle; 
static float marsAngle;
static float jupiterAngle;
static float saturnAngle;
static float uranusAngle;
static float neptunAngle;

float eyeX = 0.0f;
float eyeY = 600.0f;
float eyeZ = 1700.0f;

float angle = 0.0f;
float i = 0.0f;

GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat mat_specular[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_shininess[] = {50.0};
GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
GLfloat model_ambient[] = {1.0, 1.0, 1.0, 1.0};

void LightMaterials() {

  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void loadTexture(unsigned char *fileName, GLuint i) {

   stbi_set_flip_vertically_on_load(true);
   unsigned char *data;
   int w, w1, h1, h,ch, channels;
    // Создание текстуры
    glGenTextures(1, &texture[i]);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    //ввод текстуры обмотки и фильтрации изображения
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //теперь загрузим изображение
    
    data = stbi_load(fileName, &w, &h, &channels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      printf("Error to load texture\n");
    }
    stbi_image_free(data);
}

void changeColour(GLfloat r, GLfloat g, GLfloat b, GLfloat A) {

  model_ambient[0] = r;
  model_ambient[1] = g;
  model_ambient[2] = b;
  model_ambient[3] = A;

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
}

void initStars() {

  srand(time(NULL));

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      stars[i][j] = rand() % 500;
    }
  }
}

void changeSize(int width, int height) {

  if (height == 0) {
    height = 1;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(0, 0, width, height);

  GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
  gluPerspective(45, aspectRatio, 1, 4000);

  glMatrixMode(GL_MODELVIEW);
}

void rings(const int angl) {
  for (double i = 0; i < 2 * PI; i += PI/ 1000) {
    glVertex3f(cos(i) * angl, sin(i) * angl, 0.0);
  }
}

void locus(const int planetRad) {

  glBegin(GL_POINTS);
  for (double i = 0; i < 2 * PI; i += PI / planetRad) {
    glVertex3f(cos(i) * planetRad, 1.0, sin(i) * planetRad);
  }
  glEnd();
}

void display() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glPushMatrix();
  glTranslatef(transX, transY, -100.0);
  glColor4f(0.5, 0.5, 0.5, 0.5);
  
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 25; j++) {
      glBegin(GL_POINTS);
      glVertex2i(-stars[i][j], -stars[i+1][j+1]);
      glEnd();

      glBegin(GL_POINTS);
      glVertex2i(stars[i+2][j+2], -stars[i+1][j+1]);
      glEnd();

      glBegin(GL_POINTS);
      glVertex2i(-stars[i+3][j+3], stars[i+2][j+1]);
      glEnd();

      glBegin(GL_POINTS);
      glVertex2i(stars[i+2][j+5], stars[i+3][j+1]);
      glEnd();
      
    }
  }
  glPopMatrix();

  glPointSize(0.5);
  glColor3f(1.0, 1.0, 1.0);
  
  gluLookAt( eyeX, eyeY, eyeZ,
  	     0.0f, 0.0f, 0.0f,
  	     0.0f, 1.0f, 0.0f);

  changeColour(1.0, 1.0, 1.0, 1.0);

  locus(mercuryRad);
  locus(venusRad);
  locus(earthRad);
  locus(marsRad);
  locus(jupiterRad);
  locus(saturnRad);
  locus(uranusRad);
  locus(neptunRad);


  //The Sun
  glPushMatrix();
  glRotatef(angle, 0.0, 1.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  gluQuadricTexture(quad,1);
  gluSphere(quad, 100, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  

  //Mercury
  glPushMatrix();
  glRotatef(mercuryAngle, 0.0, 1.0, 0.0);
  glTranslatef(mercuryRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  loadTexture("mercury.jpg", 1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  gluQuadricTexture(Mercury,1);
  gluSphere(Mercury, 10, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


  //Venus
  glPushMatrix();
  glRotatef(venusAngle, 0.0, 1.0, 0.0);
  glTranslatef(venusRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  loadTexture("venus.jpg", 2);
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  gluQuadricTexture(Venus,1);
  gluSphere(Venus, 25.8, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


  //Earth
  glPushMatrix();
  glRotatef(earthAngle, 0.0, 1.0, 0.0);
  glTranslatef(earthRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  Earth = gluNewQuadric();
  loadTexture("earth.jpg", 3);
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  gluQuadricTexture(Earth,1);
  gluSphere(Earth, 20.6, 100, 100);

  //Moon
  glRotatef(5 * earthAngle, 0.0, 1.0, 0.0);
  locus(50);
  glTranslatef(50, 0.0 , 0.0);
  glColor3f(1.0, 1.0, 1.0);
  Moon = gluNewQuadric();
  gluSphere(Moon, 7, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  //Mars
  glPushMatrix();
  glRotatef(marsAngle, 0.0, 1.0, 0.0);
  glTranslatef(marsRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  loadTexture("mars.jpg", 4);
  Mars = gluNewQuadric();
  glBindTexture(GL_TEXTURE_2D, texture[4]);
  gluQuadricTexture(Mars,1);
  gluSphere(Mars, 18.4, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


  //Jupiter
  glPushMatrix();
  glRotatef(jupiterAngle, 0.0, 1.0, 0.0);
  glTranslatef(jupiterRad, 0.0, 0.0);
  glRotatef(angle + angle*4, 0.0, 1.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  Jupiter = gluNewQuadric();
  loadTexture("jupiter.jpg", 5);
  glBindTexture(GL_TEXTURE_2D, texture[5]);
  gluQuadricTexture(Jupiter,1);
  gluSphere(Jupiter, 64.5, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


  //Saturn
  glPushMatrix();
  glRotatef(saturnAngle, 0.0, 1.0, 0.0);
  glTranslatef(saturnRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  glRotatef(angle + angle*4, 0.0, 1.0, 0.0);
  Saturn = gluNewQuadric();
  loadTexture("saturn.jpg", 6);
  glBindTexture(GL_TEXTURE_2D, texture[6]);
  gluQuadricTexture(Saturn,1);
  gluSphere(Saturn, 54, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  glPushMatrix();
  glRotatef(saturnAngle, 0.0, 1.0, 0.0);
  glTranslatef(saturnRad, 0.0, 0.0);
  glRotatef(80.0, 1.0, 0.0, 0.0);
  changeColour(163/255.0, 149/255.0, 79/255.0, 0.0);
  glBegin(GL_POINTS);
  for (int j = 70; j < 110; j += 5) {
    rings(j);
  }
  glEnd();
  glPopMatrix();

  //Uranus
  glPushMatrix();
  glRotatef(uranusAngle, 0.0, 1.0, 0.0);
  glTranslatef(uranusRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  Uranus = gluNewQuadric();
  loadTexture("uranus.jpg", 7);
  glBindTexture(GL_TEXTURE_2D, texture[7]);
  gluQuadricTexture(Uranus,1);
  gluSphere(Uranus, 22.5, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


  //Neptun
  glPushMatrix();
  glRotatef(neptunAngle, 0.0, 1.0, 0.0);
  glTranslatef(neptunRad, 0.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  Neptun = gluNewQuadric();
  loadTexture("neptun.jpg", 8);
  glBindTexture(GL_TEXTURE_2D, texture[8]);
  gluQuadricTexture(Neptun,1);
  gluSphere(Neptun, 24, 100, 100);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  
  //Changing angles of rotation of every planet
  angle += 0.2f;
  mercuryAngle += mercury / 10;
  venusAngle += venus / 10;
  earthAngle += earth / 10;
  marsAngle += mars / 10;
  jupiterAngle += jupiter / 2;
  saturnAngle += saturn / 2;
  uranusAngle += uranus * 10;
  neptunAngle += neptun * 10;
  
  glutSwapBuffers();
}

void keyboard(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP:
    eyeY -= 10;
    eyeZ -= 10;
    break;

  case GLUT_KEY_LEFT:
    eyeZ -= 10;
    break;

  case GLUT_KEY_RIGHT:
    eyeZ += 10;
    break;

  case GLUT_KEY_DOWN:
    eyeY += 10;
    eyeZ += 10;
    break;

  default:
    break;
  }
}

void normalKeys(unsigned char k, int x, int y) {
  switch(k) {

  case 'a':
    eyeX -= 10;
    break;
  case 'd':
    eyeX += 10;
    break;

  default:
    break;
  }

  //glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {

  switch(button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      glutIdleFunc(display);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      glutIdleFunc(NULL);
    }
    break;
  }
}

void init() {

  glClearColor(0.0 ,0.0, 0.0, 1.0);
  LightMaterials();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0); 
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  quad = gluNewQuadric();
  Mercury = gluNewQuadric();
  Venus = gluNewQuadric();
}

int main(int argc, char** argv) {

  initStars();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(1000, 500);
  glutInitWindowPosition(100, 100);

  glutCreateWindow("Solar system model");

  init();
  loadTexture("sun.jpg", 0);
  glDepthFunc(GL_LEQUAL);
  
  glutDisplayFunc(display);
  glutReshapeFunc(changeSize);
  glutSpecialFunc(keyboard);
  glutMouseFunc(mouse);
  glutKeyboardFunc(normalKeys);
  
  glutMainLoop();

  return 0;
}
