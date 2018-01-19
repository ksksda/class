#ifndef ROBOKICK	/*include guard*/
 #define ROBOKICK

 #include<GL/glut.h>
 #include<math.h>
 #include<stdio.h>

 #define OBJECTS 3	/*ボールの個数*/
 #define F 60		/*フレーム数[fps]*/
 
 struct Vector2{	/*二次元ベクトル*/
  float x;
  float y;
 };
 struct obj{		/*ボール*/
  float	 rad;		/*半径[mm]*/
  float	 mas;		/*[kg]*/
  float	 els;		/*反発係数0<x<1*/
  float	 frc;		/*摩擦係数0<x<1*/
  float	 rot;		/*現在の角度0<x[deg]<360*/
  float	 dps;		/*回転速度0<x[deg]<360*/
  struct Vector2 pos;	/*位置[mm]*/
  struct Vector2 vel;	/*速度[mm/s]*/
  struct Vector2 acc; 	/*加速度[mm/ss]*/
  float		 r;	/*red*/
  float		 g;	/*green*/
  float		 b;	/*blue*/
 };
 struct lg{		/*足*/
  struct Vector2 cnt;	/*関節の中央座標*/
  float 	 len;	/*足の長さ*/
  float 	 mxs;	/*速度*/
  struct obj	 toe;	/*足先*/
 };

 extern struct lg leg;
 extern struct obj ball[OBJECTS];
 extern int i,j;
 extern int c,k;
 extern float p;
 struct Vector2 VmV(struct Vector2,struct Vector2);	/*ベクトル - ベクトル*/
 struct Vector2 Nom(struct Vector2);			/*単位ベクトル化*/
 struct Vector2 VtV(struct Vector2);			/*垂直なベクトルを返す*/
 struct Vector2 VxS(struct Vector2,float);		/*ベクトルのスカラー倍*/
 float dist(struct Vector2);				/*大きさ*/
 void kick(void);					/*足とボールの衝突*/
 void colide(void);					/*ボールとボールの衝突*/
 void out(void);					/*枠外判定*/
 void simulation(void);					/*物理シミュレーション*/

#endif
