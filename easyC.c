#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
int main(){
 char formula[256]={0};
 double a=0,b=1,c=1;
 int d=0,f=0;
 while(1){
  scanf("%s",formula);
  if(formula[0]=='+'||formula[0]=='-'||formula[0]=='*'||formula[0]=='/')b=a;
  else b=1;
  c=1;
  a=d=f=0;
  for(int i=0;formula[i]!='\0';i++){
   switch(formula[i]){
    case '-':c*=-1;
    case '+':a+=b;b=1;break;
    case '/':d=1;break;
    case 's':f=1;break;
    case 'c':f=2;break;
    case 't':f=3;break;
   }
   if(isdigit(formula[i])){
    c*=atof(&formula[i]);
    switch(f){
    case 1:c=sin(c*M_PI/180);break;
    case 2:c=cos(c*M_PI/180);break;
    case 3:c=tan(c*M_PI/180);break;
    }
    b*=(d? (1/c):c);
    c=1;
    d=f=0;
    for(;isdigit(formula[i])||formula[i]=='.';i++);
    i--;
   }
  }
  a+=b;
  printf("=%lf\n",a);
 }
 return 0;
}
