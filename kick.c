#include "kick.h"

float circle[36][2];
struct lg leg={
/* ctx   cty   len   mxs   rad   mas   els*/
     0,    0,  500, 3000,   60,  0.1,  1.0
};
struct obj ball[OBJECTS]={
/* rad   mas   els    frc   rot   dps   psx   psy   vlx   vly   acx   acy  r  g  b */
    60,  0.1,    1,0.0001,    0,    0,    0,  200, 1000, 1000,    0,   00, 1, 1, 0,
    60,  0.1,    1,0.0001,    0,    0,    0,  500, 1000,-1000,    0,   00, 0, 1, 1,
    60,  0.1,    1,0.0001,    0,    0,    0, -500, 1000,-1000,    0,   00, 0, 1, 1,
};
int i=0,j=0;
int c=1,k=0;
int n=1;
float p=1.0;
struct Vector2 inf={10000,10000};

void display(void){
 glClear(GL_COLOR_BUFFER_BIT);
 glColor3f(1,1,1);
 glBegin(GL_LINE_LOOP);
  glVertex2f(1200,600);
  glVertex2f(1200,-600);
  glVertex2f(-1200,-600);
  glVertex2f(-1200,600);
 glEnd();
 
 glBegin(GL_LINE_LOOP);
  for(j=0;j<36;j++)glVertex2f(leg.toe.pos.x+leg.toe.rad*circle[j][0],leg.toe.pos.y+leg.toe.rad*circle[j][1]);
 glEnd();
 glBegin(GL_LINES);
  glVertex2f(leg.toe.pos.x,leg.toe.pos.y);
  glVertex2f(leg.cnt.x,leg.cnt.y);
 glEnd();
 glBegin(GL_LINE_LOOP);
  for(j=0;j<36;j++)glVertex2f(leg.cnt.x+10*circle[j][0],leg.cnt.y+10*circle[j][1]);
 glEnd();

 for(i=0;i<OBJECTS;i++){
  glPushMatrix();
   glTranslatef(ball[i].pos.x,ball[i].pos.y,0);
   glRotatef(ball[i].rot, 0.0, 0.0, 1.0);
   glColor3f(ball[i].r,ball[i].g,ball[i].b);
   glBegin(GL_LINE_LOOP);
    for(j=0;j<36;j++)glVertex2f(ball[i].rad*circle[j][0],ball[i].rad*circle[j][1]);
   glEnd();
   glBegin(GL_LINES);
    glVertex2f(0,ball[i].rad);
    glVertex2f(0,-ball[i].rad);
    glVertex2f(ball[i].rad,0);
    glVertex2f(-ball[i].rad,0);
   glEnd();
  glPopMatrix();
 }
 glutSwapBuffers();
}

void reshape(int w, int h){
 glViewport( 0, 0, w, h );
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1295, 1295, -744, 744, -1.0, 1.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y){
 switch(key){
  case '\r':
   n^=1;
   if(n)glutIdleFunc(simulation);
   else glutIdleFunc(NULL);
  break;
  case 'd':p*=2.0;break;
  case 'a':p/=2.0;break;
  case 'c':c^=1;break;
  case 'k':
   k^=1;
   if(!k){
    leg.toe.pos=VmV(leg.toe.pos,inf);
    leg.cnt=VmV(leg.cnt,inf);
   }
   else{
    leg.toe.pos=VmV(leg.toe.pos,VxS(inf,-1));
    leg.cnt=VmV(leg.cnt,VxS(inf,-1));
   }
  break;
 }
}

void mouse(int button, int state, int x, int y){
 switch (button) {
  default:break;
 }
}

int main(int argc, char *argv[]){

 for(float x=0;x<36.0;x+=1.0){
  circle[(int)x][0]=sin(x*M_PI/18);
  circle[(int)x][1]=cos(x*M_PI/18);
 }
 leg.toe.pos.x=leg.cnt.x;
 leg.toe.pos.x=leg.cnt.y-leg.len;
 if(!k){
  leg.toe.pos=VmV(leg.toe.pos,inf);
  leg.cnt=VmV(leg.cnt,inf);
 }
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize (1295,744);
 glutInitWindowPosition(0,0);
 glutCreateWindow(argv[0]);
 glClearColor(0, 0, 0, 0);
 glShadeModel(GL_FLAT);
 glutIdleFunc(simulation);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(keyboard);
 glutMouseFunc(mouse);
 glutDisplayFunc(display);
 glutMainLoop();
 return 0;
}
