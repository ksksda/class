#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
int main(){
 char X[255],d,f,i;
 double a=0,b,c,A=M_PI/180;
 while(c=1){
  scanf("%s",X);
  b=(X[0]=='+'|X[0]=='-'|X[0]=='*'|X[0]=='/')?a:1;
  for(a=d=f=i=0;X[i];i++){
   switch(X[i]){
    case'-':c*=-1;
    case'+':a+=b;b=1;break;
    case'/':d=1;break;
    case't':f++;
    case'c':f++;
    case's':f++;
   }
   if(isdigit(X[i])){
    c*=atof(&X[i]);
    switch(f){
     case 1:c=sin(c*A);break;
     case 2:c=cos(c*A);break;
     case 3:c=tan(c*A);break;
    }
    b*=(d?(1/c):c);
    c=1;
    d=f=0;
    while(isdigit(X[++i]));
    i--;
   }
  }
  printf("=%lf\n",a+=b);
 }
 return 0;
}
