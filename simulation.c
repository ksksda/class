#include "kick.h"

struct Vector2 r={0,0};
struct Vector2 a={0,0};
struct Vector2 b={0,0};
struct Vector2 fw={0,0};
struct Vector2 up={0,0};
int flags[OBJECTS+1][OBJECTS+1]={0};
float t=0;

struct Vector2 VmV(struct Vector2,struct Vector2);
struct Vector2 Nom(struct Vector2);
struct Vector2 VtV(struct Vector2);
struct Vector2 VxS(struct Vector2,float);
float dist(struct Vector2);
void kick(void);
void collide(void);
void out(void);

void simulation(void){
 for(i=0;i<OBJECTS;i++){
  if(dist(VmV(leg.toe.pos,ball[i].pos))<leg.toe.rad+ball[i].rad)kick();
  else flags[0][i+1]=0;
  if(c){
   for(j=i+1;j<OBJECTS;j++){
    if(dist(VmV(ball[i].pos,ball[j].pos))<ball[i].rad+ball[j].rad)collide();
    else flags[i+1][j+1]=0;
   }
  }
  ball[i].vel.x+=ball[i].acc.x/F*p;
  ball[i].vel.y+=ball[i].acc.y/F*p;
  ball[i].pos.x+=ball[i].vel.x/F*p;
  ball[i].pos.y+=ball[i].vel.y/F*p;
  out();
  ball[i].rot=fmod(ball[i].rot+ball[i].dps/F*p,360.0);
 }
  leg.toe.vel=VxS(Nom(VtV(VmV(leg.toe.pos,leg.cnt))),leg.mxs);
  leg.toe.pos.x+=leg.toe.vel.x/F*p;
  leg.toe.pos.y+=leg.toe.vel.y/F*p;
  leg.toe.pos=VmV(leg.cnt,VxS(Nom(VmV(leg.toe.pos,leg.cnt)),-leg.len));
  leg.toe.rot=fmod(leg.toe.rot+leg.toe.dps/F*p,360.0);

 glutPostRedisplay();
 for(;glutGet(GLUT_ELAPSED_TIME)-t<1000.0/F;);
 t=glutGet(GLUT_ELAPSED_TIME);
}

struct Vector2 VmV(struct Vector2 s,struct Vector2 t){
 r.x=s.x-t.x;
 r.y=s.y-t.y;
 return r;
}

struct Vector2 VxS(struct Vector2 s,float a){
 r.x=s.x*a;
 r.y=s.y*a;
 return r;
}
struct Vector2 VtV(struct Vector2 s){
 r.x=-s.y;
 r.y=s.x;
 return r;
}
struct Vector2 Nom(struct Vector2 s){
 r.x=s.x/dist(s);
 r.y=s.y/dist(s);
 return r;
}

float dist(struct Vector2 s){
 return sqrt(s.x*s.x+s.y*s.y);
}

void kick(void){
 if(flags[0][i+1])return;
 flags[0][i+1]=1;
 a=ball[i].vel;
 b=leg.toe.vel;
 ball[i].pos=VmV(leg.toe.pos,VxS(VmV(leg.toe.pos,ball[i].pos),(leg.toe.rad+ball[i].rad)/dist(VmV(leg.toe.pos,ball[i].pos))));
 fw=VmV(leg.toe.pos,ball[i].pos);
 up=VtV(fw);
 ball[i].vel=VxS(Nom(up),(fw.x*a.y-fw.y*a.x)/dist(fw));
 ball[i].vel=VmV(ball[i].vel,VxS(Nom(up),2*(ball[i].rad*ball[i].dps*M_PI/180+dist(ball[i].vel))*ball[i].frc*F/5));
 ball[i].dps=-(fw.x/ball[i].vel.y>0&&fw.y/ball[i].vel.x<0? 1:-1)*dist(ball[i].vel)*180/M_PI/ball[i].rad;
 ball[i].vel=VmV(ball[i].vel,VxS(Nom(fw),leg.toe.mas*(((fw.x*( a.x-b.x)+fw.y*( a.y-b.y))*leg.toe.els*ball[i].els-(fw.x*(ball[i].mas*a.x+leg.toe.mas*b.x)+fw.y*(ball[i].mas*a.y+leg.toe.mas*b.y))/leg.toe.mas)/dist(fw)/(ball[i].mas+leg.toe.mas))));
 return;
}

void collide(void){
 if(flags[i+1][j+1])return;
 flags[i+1][j+1]=1;
 a=ball[i].vel;
 b=ball[j].vel;
 ball[i].pos=VmV(ball[j].pos,VxS(VmV(ball[j].pos,ball[i].pos),(ball[j].rad+ball[i].rad)/dist(VmV(ball[j].pos,ball[i].pos))));
 fw=VmV(ball[j].pos,ball[i].pos);
 up=VtV(fw);
 ball[i].vel=VxS(Nom(up),(fw.x*a.y-fw.y*a.x)/dist(fw));
 ball[i].vel=VmV(ball[i].vel,VxS(Nom(up),2*(ball[i].rad*ball[i].dps*M_PI/180+dist(ball[i].vel))*ball[i].frc*F/5));
 ball[j].vel=VxS(Nom(up),(fw.x*b.y-fw.y*b.x)/dist(fw));
 ball[j].vel=VmV(ball[j].vel,VxS(Nom(up),2*(ball[j].rad*ball[j].dps*M_PI/180+dist(ball[j].vel))*ball[j].frc*F/5));
 ball[i].dps=(fw.x*(ball[i].vel.y-ball[j].vel.y)>0&&fw.y*(ball[i].vel.x-ball[j].vel.y)<0? -1:1)*dist(VmV(ball[i].vel,ball[j].vel))*180/M_PI/ball[i].rad;
 ball[j].dps=(fw.x*(ball[j].vel.y-ball[i].vel.y)>0&&fw.y*(ball[j].vel.x-ball[i].vel.y)<0? 1:-1)*dist(VmV(ball[i].vel,ball[j].vel))*180/M_PI/ball[j].rad;
 ball[i].vel=VmV(ball[i].vel,VxS(Nom(fw),ball[j].mas*(((fw.x*( a.x-b.x)+fw.y*( a.y-b.y))*ball[j].els*ball[i].els-(fw.x*(ball[i].mas*a.x+ball[j].mas*b.x)+fw.y*(ball[i].mas*a.y+ball[j].mas*b.y))/ball[j].mas)/dist(fw)/(ball[i].mas+ball[j].mas))));
 ball[j].vel=VmV(ball[j].vel,VxS(Nom(fw),ball[i].mas*(((fw.x*(-a.x+b.x)+fw.y*(-a.y+b.y))*ball[i].els*ball[j].els-(fw.x*(ball[i].mas*a.x+ball[j].mas*b.x)+fw.y*(ball[i].mas*a.y+ball[j].mas*b.y))/ball[i].mas)/dist(fw)/(ball[i].mas+ball[j].mas))));
}

void out(){
 if(ball[i].pos.x>-ball[i].rad+1200){
  ball[i].vel.y+=-2*(ball[i].rad*ball[i].dps*M_PI/180+ball[i].vel.y)*ball[i].frc*F/5;
  ball[i].dps=-ball[i].vel.y*180/M_PI/ball[i].rad;
  ball[i].pos.x=-ball[i].rad+1200;
  ball[i].vel.x=-ball[i].vel.x*ball[i].els;
 }
 else if(ball[i].pos.x<ball[i].rad-1200){
  ball[i].vel.y+=2*(ball[i].rad*ball[i].dps*M_PI/180+ball[i].vel.y)*ball[i].frc*F/5;
  ball[i].dps=ball[i].vel.y*180/M_PI/ball[i].rad;
  ball[i].pos.x=ball[i].rad-1200;
  ball[i].vel.x=-ball[i].vel.x*ball[i].els;
 }
 if(ball[i].pos.y>-ball[i].rad+600){
  ball[i].vel.x+=2*(ball[i].rad*ball[i].dps*M_PI/180+ball[i].vel.x)*ball[i].frc*F/5;
  ball[i].vel.x+=(ball[i].vel.x>0 ? -1:1)*ball[i].frc*-9800.0;
  ball[i].dps=ball[i].vel.x*180/M_PI/ball[i].rad;
  ball[i].pos.y=-ball[i].rad+600;
  ball[i].vel.y=-ball[i].vel.y*ball[i].els;
 }
  else if(ball[i].pos.y<ball[i].rad-600){
  ball[i].vel.x+=-2*(ball[i].rad*ball[i].dps*M_PI/180+ball[i].vel.x)*ball[i].frc*F/5;
  ball[i].vel.x+=(ball[i].vel.x>0 ? 1:-1)*ball[i].frc*-9800.0;
  ball[i].dps=-ball[i].vel.x*180/M_PI/ball[i].rad;
  ball[i].pos.y=ball[i].rad-600;
  ball[i].vel.y=-ball[i].vel.y*ball[i].els;
 }
}

