#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
struct termios term;
struct termios save;
int main(void){
    tcgetattr(STDIN_FILENO, &term);
    save = term;
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
        char tmp,formula[256]={0};
        double a=0,b=1,c=1;
        int jj=0,d=0,f=0;
    while(tmp!=10){
        /*もっとココらへん減らせる*/
        tmp = fgetc(stdin);
        formula[jj]=tmp;
        formula[jj+1]='\0';
        if(tmp==127&&jj>0){jj--;formula[jj]='\0';
        }else{jj++;}
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
            if((formula[0]==10||formula[0]==127||formula[0]=='\0'||tmp=='+'||tmp=='-')){a--;}
            printf("\033[2K\r%lf=%s",a,formula);//}
        }
    tcsetattr(STDIN_FILENO, TCSANOW, &save);
}
