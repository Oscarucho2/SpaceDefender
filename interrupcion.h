#include <time.h>

class interrupcion{
int time1,time2,x,y,b,xf,yf;
int incx1, incx2, decx1,decx2;
void *parche;
public:
    tiempo(void)
    {time1=GetTickCount();
     time2=GetTickCount();
     incx1=GetTickCount();
     incx2=GetTickCount();
     decx1=GetTickCount();
     decx2=GetTickCount();
     x=0;y=0;b=1;}

void incremento(int t,int &dato, int inc)
{
      incx2=GetTickCount();
      if(incx1+t<=incx2){
        incx1=GetTickCount();
        dato+=inc;
        }
}


void decremento(int t,int &dato, int inc)
{
      decx2=GetTickCount();
      if(decx1+t<=decx2){
        decx1=GetTickCount();
        dato-=inc;
        }
}

void incremento(int t,double &dato, double inc)
{
      incx2=GetTickCount();
      if(incx1+t<=incx2){
        incx1=GetTickCount();
        dato+=inc;
        }
}


void decremento(int t,double &dato, double inc)
{
      decx2=GetTickCount();
      if(decx1+t<=decx2){
        decx1=GetTickCount();
        dato-=inc;
        }
}


void tam (int x, int y)
{ xf=x; yf=y; parche=malloc(imagesize(0,0,xf,yf));}

void desplazamiento_dia(int t,int xi,int yi,char ima[50])
{
    if(b){b=0;getimage(xi+x,yi-y,xi+xf+x,yi+yf-y,parche);}
      time2=GetTickCount();
      if(time1+t<=time2){
        b=1;
        time1=GetTickCount();
        putimage(xi+x,yi-y,parche,0);
        x+=10;y+=10;
        if(x>250){x=0;y=0;}
        }
      else {readimagefile(ima,xi+x,yi-y,xi+x+xf,yi-y+yf);}
}


void desplazamiento_der(int t,int xi,int yi,char ima[50])
{   y=0;
    if(b){b=0;getimage(xi+x,yi-y,xi+xf+x,yi+yf-y,parche);}
      time2=GetTickCount();
      if(time1+t<=time2){
        b=1;
        time1=GetTickCount();
        putimage(xi+x,yi-y,parche,0);
        x+=10;
        if(x>250){x=0;}
        }
      else {readimagefile(ima,xi+x,yi-y,xi+x+xf,yi-y+yf);}
}

void desplazamiento_aba(int t,int xi,int yi,char ima[50])
{   x=0;
    if(b){b=0;getimage(xi+x,yi+y,xi+xf+x,yi+yf+y,parche);}
      time2=GetTickCount();
      if(time1+t<=time2){
        b=1;
        time1=GetTickCount();
        putimage(xi+x,yi+y,parche,0);
        y+=10;
        if(y>250){y=0;}
        }
      else {readimagefile(ima,xi+x,yi+y,xi+x+xf,yi+y+yf);}
}

};
