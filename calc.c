#include<stdio.h>	/*標準入出力*/
#include<stdlib.h>	/*atof用*/
#include<math.h>	/*関数計算用*/
#include<ctype.h>	/*isdigit用*/

#define FUNCTIONS 14
#define LENGTH 256

int check_o(char);
int check_f(char*);
double calc_o(double,double,int);
double calc_f(int,double);

struct itm{
 short ism;		/*負数であれば1にする*/
 short isf;		/*計算済みで空白になっていれば1にして、計算上無視する*/
 short ope;		/*前と接続する演算子の種類*/
 short div;		/*演算子が数式の何番目の要素として現れるか*/
 double now;		/*数字*/
};
struct par{
 short ism;		/*カッコ内の一項目が負数であれば1にする*/
 short lyr;		/*何重のカッコであるか（この数字が大きいほど計算の優先度が高い）*/
 short bgn;		/* ( が何番目の項として現れるか*/
 short end;		/* ) が何番目の項として現れるか*/
 short idx;		/* （ が何番目の要素として現れるか*/
};

double ans[10]={0};
char f_list[FUNCTIONS][3]={
 's','r','t',		/*平方根*/
 'c','r','t',		/*立方根*/
 's','i','n',		/*正弦（deg）*/
 'c','o','s',		/*余弦（deg）*/
 't','a','n',		/*正接（deg）*/
 'a','s','n',		/*逆正弦（deg）*/
 'a','c','s',		/*逆余弦（deg）*/
 'a','t','n',		/*逆正接（deg）*/
 'r','a','d', 		/*rad→deg*/
 'l','g','n',		/*自然対数*/
 'l','g','d',		/*常用対数*/
 'e','x','p',		/*ネイピア数（e）のべき乗*/
 'f','c','t',		/*階乗*/
 'a','n','s' 		/*N個前の計算の答え（10個まで記憶）*/
};
 
int main(){
 
 char formula[LENGTH]={0};		/*数式（string）*/
 struct itm number[1+LENGTH/2]={0};	/*項の配列*/
 struct itm resetn={0};			/*リセット用の構造体*/
 struct par arc[1+LENGTH/2]={0};	/* ( の配列*/
 struct par reseta={0};			/*リセット用の構造体*/
 int use_ans=0;				/*答えを使うかどうか（式の先頭が演算子であれば先ほどの答えに対してさらに計算）*/
 
 int a,b,c,d,e;				/*単なるカウンター*/
 int x,y;				/*x:項数 y: ( の数*/
 
 while(1){
  /*スタートアップ*/
   for(a=9;a>0;a--)ans[a]=ans[a-1];
   ans[0]=number[0].now;
   number[0]=resetn;
   arc[0]=reseta;
   arc[0].idx=-1;
   use_ans=0;
  /*スタートアップ*/

  /*入力・演算子識別*/
   scanf("%s",formula);
   if(check_o(formula[0]))use_ans=1;
   if(formula[0]=='q')return 0;
   if(use_ans)number[0].now=ans[0];
   else number[0].div=-1;
   for(a=b=0,x=y=1;formula[a]!='\0';a++){
    number[x]=resetn;
    if(number[x].ope=check_o(formula[a])){
     if(x==1||number[x-1].div+1!=a)number[x++].div=a;
     else if(number[x].ope==2){
      number[x-1].div=a;
      number[x-1].ism=1;
     }
    }
    else if(formula[a]=='('){
     b++;
     arc[y]=reseta;
     arc[y].lyr=b;
     arc[y].bgn=x-1;
     arc[y].idx=a;
     y++;
    }
    else if(formula[a]==')'){
     for(c=y;arc[c].lyr!=b;c--);
     b--;
     arc[c].end=x;
    }
    else if(isupper(formula[a]))formula[a]=tolower(formula[a]);
   }
   for(a=0;a<y;a++)if(arc[a].end==0)arc[a].end=x;
  /*入力・演算子識別*/

  /*計算*/
   for(a=b=0;a<y;a++)if(arc[b].lyr>a)a=arc[a].lyr;
   for(;a>=0;a--){
    for(b=0;b<y;b++){
     if(arc[b].lyr==a){
      if(check_o(formula[arc[b].idx+1])){
       arc[b].idx++;
       if(a){
        arc[b].ism=1;
        d=arc[b].bgn;
       }
       else d=arc[b].bgn+1;
       c=number[d].div;
      }
      else{
       c=arc[b].idx;
       d=arc[b].bgn;
      }
      for(;d<arc[b].end;c=number[++d].div){
       if(!number[d].isf){
        for(e=1;;e+=3){
         if(isdigit(formula[c+e])){
          number[d].now=atof(&formula[c+e]);
          break;
         }
         else if(formula[c+e]=='(')break;
        }
       }
       for(e-=3;e>=1;e-=3)number[d].now=calc_f(check_f(&formula[c+e]),number[d].now);
       if(d!=arc[b].bgn&&number[d].ism)number[d].now*=-1;
      }
      if(arc[b].ism)number[arc[b].bgn].now*=-1;
      if(b==0)arc[b].bgn=0;
      for(c=5;c>0;c--){
       for(d=arc[b].bgn+1,e=arc[b].bgn;d<arc[b].end;d++){
        if(!number[d].isf){
         if(number[d].ope==c){
          number[e].now=calc_o(number[e].now,number[d].now,number[d].ope);
          number[d].isf=1;
         }
         else e=d;
        }
       }
      }
     }
    }
   }
  /*計算*/

 printf("=%lf\n",number[0].now);
 }
 /*qを押した時return 0*/
}

int check_o(char operator){
 switch(operator){
  case '+':return 1;
  case '-':return 2;
  case '*':return 3;
  case '/':return 4;
  case '^':return 5;
   default:return 0;
 }
}

int check_f(char * c){
 for(int a=0;a<FUNCTIONS;a++)
  if(*c==f_list[a][0]&&*(c+1)==f_list[a][1]&&*(c+2)==f_list[a][2])
   return 1+a;
   
 return 0;
}

double calc_o(double b,double a,int operator){
 switch(operator){
  case 1:return b+a;
  case 2:return b-a;
  case 3:return b*a;
  case 4:return b/a;
  case 5:return pow(b,a);
  default:return b;
 }
}

double calc_f(int function,double x){
 double a,b;
 switch(function){
 case 1:return sqrt(x);
 case 2:return cbrt(x);
 case 3:return sin((x>0? x:360+x)*M_PI/180);
 case 4:return cos((x>0? x:360+x)*M_PI/180);
 case 5:return tan((x>0? x:360+x)*M_PI/180);
 case 6:return asin(x)*180/M_PI;
 case 7:return acos(x)*180/M_PI;
 case 8:return atan(x)*180/M_PI;
 case 9:return x*180/M_PI;
 case 10:return log(x);
 case 11:return log10(x);
 case 12:return exp(x);
 case 13:for(a=b=1;b<=x;b++)a*=b;return a;
 case 14:return ans[(int)x-1];
 default:return x;
 }
}

