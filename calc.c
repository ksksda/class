#include<stdio.h>	/*標準入出力*/
#include<stdlib.h>/*atof用*/
#include<math.h>	/*関数計算用*/
#include<ctype.h>	/*isdigit用*/

#define LENGTH 256
#define OPES 5
#define FUNS 15
#define FUNCLEN 3
#define ANSS 10

int check_o(char);
int check_f(char*);
double calc_o(double,double,int);
double calc_f(int,double);

struct itm{
 short ism;		/*負数であれば1にする*/
 short isf;		/*計算済みであれば1にして、計算上無視する*/
 short ope;		/*前と接続する演算子の種類*/
 short div;		/*演算子のインデックス*/
 double now;	/*数字*/
};
struct par{
 short lyr;		/*何重のカッコであるか（この数字が大きいほど計算の優先度が高い）*/
 short bgn;		/*"("の項番号*/
 short end;		/* ) の項番号*/
 short idx;		/* （ のインデックス*/
};

double ans[ANSS]={0};
char f_list[FUNS][FUNCLEN+1]={
 "srt",		/*平方根*/
 "crt",		/*立方根*/
 "sin",		/*正弦（deg）*/
 "cos",		/*余弦（deg）*/
 "tan",		/*正接（deg）*/
 "asn",		/*逆正弦（deg）*/
 "acs",		/*逆余弦（deg）*/
 "atn",		/*逆正接（deg）*/
 "r2d", 		/*rad→deg*/
 "d2r", 		/*deg→rad*/
 "lgn",		/*自然対数*/
 "lgd",		/*常用対数*/
 "exp",		/*ネイピア数（e）のべき乗*/
 "fct",		/*階乗*/
 "ans"		/*N個前の計算の答え（10個まで記憶）*/
};
 
int main(){
 
 char formula[LENGTH]={0};		/*数式（string）*/
 struct itm number[1+LENGTH/2]={0};	/*項の配列*/
 struct itm resetn={0};			/*リセット用の構造体*/
 struct par arc[1+LENGTH/2]={0};	/*カッコの配列*/
 struct par reseta={0};			/*リセット用の構造体*/
 
 int a,b,c,d,e;				/*カウンター*/
 int x,y;					/*x:項数 y:"("の数*/

 /* 計算フェイズにおいて
　   　a:現在計算するレイヤー
 　  　b:現在計算しているカッコのインデックス
  　 　c:現在計算している項の項番号	> 現在計算している演算子の種類
  　　 d:現在計算している項のインデックス	>　演算される側の数
  　 　e:項内のインデックスオフセット		>　演算する側の数
 */
 
 while(1){
  /*スタートアップ*/
   for(a=ANSS-1;a>0;a--)ans[a]=ans[a-1];	//計算結果をシフト
   ans[0]=number[0].now;			//先の計算の答えを記憶
   number[0]=resetn;
   arc[0]=reseta;
   arc[0].idx=-1;					//最初の"（"の位置を数式の始まる1つ前とみなす
  /*スタートアップ*/

  /*入力・演算子識別*/
   scanf("%s",formula);
   if(formula[0]=='q')return 0;					//先頭文字がqなら終了
   if(check_o(formula[0]))number[0].now=ans[0];			//先頭文字が演算子なら先ほどの答えにさらに計算を行い
   else number[0].div=-1;						//そうでないなら最初の演算子の位置を数式の始まる1つ前としてみなす
   for(a=b=0,x=y=1;formula[a]!='\0';a++){				//最初の項及びカッコのセットアップは終わったのでx=y=1
    if(check_o(formula[a])){						//もし演算子にぶつかったら
     number[x]=resetn;							//x番目の項を初期化
     number[x].ope=check_o(formula[a]);				//x番目の項の演算子を設定
     if(number[x-1].div+1!=a)number[x++].div=a;			//演算子が連続していないなら新たな項として認識、演算子の位置を設定し次に進む
     else{
      number[x-1].div=a;						//そうでなければ先の項の演算子の位置を再設定
      if(number[x].ope==check_o('-'))number[x-1].ism^=1;	//"-"なら符号を反転させる
     }
    }
    else if(formula[a]=='('){		//演算子でなく"("なら
     arc[y]=reseta;			//y番目のカッコを初期化し
     arc[y].lyr=++b;			//レイヤーを設定
     arc[y].bgn=x-1;			//先ほどの項に"("が含まれると認識
     arc[y].idx=a;			//"("のインデックスを設定し
     y++;					//次に進む
    }
    else if(formula[a]==')'){		//そうでもなく")"なら
     for(c=y-1;arc[c].lyr!=b;c--);	//一番最近登場した現在のレイヤーと一致する"("を探す
     arc[c].end=x;			//その"("の終端を設定
     b--;					//レイヤーを下げる
    }
    else if(isupper(formula[a]))formula[a]=tolower(formula[a]);	//大文字は小文字に変換
   }
   for(a=0;a<y;a++)if(arc[a].end==0)arc[a].end=x;			//")"がなかった"("の終端を式の終端として設定
  /*入力・演算子識別*/

  /*計算*/
   for(a=b=0;b<y;b++)if(arc[b].lyr>a)a=arc[b].lyr;	//aを最高レイヤーに
   for(;a>=0;a--){						//レイヤーを下げつつ計算をすすめる
    for(b=0;b<y;b++){
     if(arc[b].lyr==a){						//それぞれのカッコに関してレイヤーが計算レイヤーと一致したら計算開始
      if(check_o(formula[arc[b].idx+1])){			//"("のすぐ次が演算子なら
       c=arc[b].bgn+1;						//"("の含まれる項の次の項(カッコ内最初の項)を計算対象に設定
       d=number[c].div;
      }
      else{
       c=arc[b].bgn;						//そうでなければ"("の含まれる項を計算対象に設定
       d=arc[b].idx;						//"("の位置から計算開始("("より左の関数は計算しない)
      }
      for(;c<arc[b].end;d=number[++c].div){		//上記の初期化条件で、カッコ内のそれぞれの項において
       if(!number[c].isf){					//計算済みでないなら(現在よりレイヤーの高いカッコ内で処理されている可能性がある)
        for(e=1;;e+=FUNCLEN){					//数字が現れる可能性があるのは演算子の1+n*3(FUNCLEN)個右の文字のみ
         if(isdigit(formula[d+e])){				//数字だったら
          number[c].now=atof(&formula[d+e]);		//数字変換して検索を終了
          break;
         }
         else if(formula[d+e]=='(')break;			//"("だったら終了
        }								//"("にぶつかったということはカッコ内はレイヤーが計算レイヤーより高いので
       }								//"("の含まれる項のnowにはカッコ内の計算結果がすでに入っている
       for(e-=FUNCLEN;e>=1;e-=FUNCLEN)number[c].now=calc_f(check_f(&formula[d+e]),number[c].now);	//項の先頭に向かって関数計算
       if(c!=arc[b].bgn&&number[c].ism)number[c].now*=-1;								//符号がマイナスなら-1倍
      }
      for(c=OPES;c>0;c--){									//各演算子に対して(数字が大きいほど計算優先度は高い)
       for(d=arc[b].bgn,e=arc[b].bgn+1;e<arc[b].end;e++){				//カッコ内の最初の項と次の項とで計算を行っていく
        if(!number[e].isf){									//演算する側の項が計算済みでなかったら計算開始
         if(number[e].ope==c){								//演算する側の項の演算子が現在計算する演算子と一致したら
          number[d].now=calc_o(number[d].now,number[e].now,number[e].ope);	//演算を行い
          number[e].isf=1;									//計算済みにする
         }
         else d=e;										//一致しなければ演算される側の項を更新
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
 for(int a=0;a<FUNS;a++)
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
 case 10:return x*M_PI/180;
 case 11:return log(x);
 case 12:return log10(x);
 case 13:return exp(x);
 case 14:for(a=b=1;b<=x;b++)a*=b;return a;
 case 15:return ans[(int)x-1];
 default:return x;
 }
}
