#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include <winbgim.h>
#include <windows.h>
#include <string.h>
#include <time.h>


#define N_OP 4
#define TAMBX 150
#define TAMBY 80
#define HORIZ 0
#define VERT 1

//incrementos de malla
#define INC 30
#define N 5

//Estructura para manejar el arreglo.
typedef struct
{
    int nr;
    int nc;
    char **amb;
} TSpace;

//Estructura para manejo de botones.
typedef struct
{
    int x, y;
    int tamx, tamy;
    char texto[50];
    int imag[100][100];
} TBoton;

//Estructura para el manejo del jugador
typedef struct nod
{
    int x,y;
    struct nod *izq,*der;
    struct nod *arr,*aba;
} TNodo;

//Estructura para manejo de enemigos
typedef struct
{
    int x;
    int y;
} enemigo;

typedef struct
{
    char nombre[50];
    int pts;
} TReg;

//Estructura para manejo de balas
typedef struct
{
    int x;
    int y;
    int existe;
} bala;

//Prototipos
void Portada();
void DibujaBoton(TBoton bot, int pres);
void Menu(TBoton botones[], int tamBot, int xi, int yi, int tamx, int tamy);
void inText(int x, int y, char texto[], int color_I, int color_F, char nombre[]);
void imprimeMalla(int nc, int nr, int tamxm, int tamym);
void creaMalla(float nC, float nR, int tamxm, int tamym);
void dibujaVidas();
void elijeJugador(char *nave);
int ayuda(TBoton botones2[], int nBots2);
void enemigos();
void player();
void juego(char nombre[25], TSpace sp);
void malla(TNodo **cab, int xi,int yi, int nc, int nr);
void imprime_ambiente(char nombre[25], int punt, int col1, int col2, int col3);
void imprime_tiempo(clock_t ini, int pts);
void cargaSprites(TNodo nodo, enemigo ene);
void itoa(int value, char* str, int base);
void imprimepts(int puntosxd);
void gameOver(int puntos);

int main()
{
    //Botones en menú (botones[3]) y en la interfaz de juego (botones2[1]).
    TBoton botones[3], botones2[1];
    //Malla para la interfaz de juego.
    TSpace sp;
    sp.nr = 20;
    sp.nc = 15;

    int i, salir=0, x, y, nBots1 = 3, nBots2 = 1;


    //Arreglo para manejar la captura de texto.
    char texto[50];
    char nombre[25];

    initwindow(700, 600, "SPACE DEFENDER");

    strcpy(botones[0].texto, "Play");
    strcpy(botones[1].texto, "Help");
    strcpy(botones[2].texto, "Exit");

    strcpy(botones2[0].texto, "Back");

    cleardevice();

    readimagefile("Tittle.jpg", 0, 0, getmaxx(), getmaxy());

    Menu(botones, nBots1, 100, 500, 100, 50);
    i=-1;
    do
    {
        //Condicional para verificar si se presionó en clic izquierdo.
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            //Coordenadas del clic izquierdo.
            getmouseclick(WM_LBUTTONDOWN, x, y);
            i = 0;

            //Siempre se va a imprimir en la consola las coordenadas donde se presionó el clic izquierdo.
            printf("X: %d, Y: %d\n", x, y);

            while(i<nBots1)
            {
                if(x >= botones[i].x && x <= botones[i].x + botones[i].tamx && y >= botones[i].y && y <= botones[i].y + botones[i].tamy)
                    break;
                else
                    i++;
            }


            //Condicional para la animación de los botones.
            if(i<nBots1)
            {
                DibujaBoton(botones[i], 1);
                delay(500);
                DibujaBoton(botones[i], 0);
            }

            //Switch para manejar las funciones de los botones del menú.
            switch(i)
            {
                //Play.
            case 0:
                cleardevice();
                readimagefile("PlayerName.jpg", 0, 0, getmaxx(), getmaxy());
                inText(230, 260, texto, 15, 7, nombre);
                cleardevice();
                juego(nombre, sp);
                break;
                //Help.
            case 1:
                cleardevice();

                ayuda(botones2, nBots2);

                break;


                //Exit.
            case 2:
                salir = 1;
                break;
            }
            if(salir==0)
            {
                //Impresión de la portada y los botones en el menú principal.
                cleardevice();
                readimagefile("Tittle.jpg", 0, 0, getmaxx(), getmaxy());
                //Titulo();
                for(i=0; i<nBots1; i++)
                    DibujaBoton(botones[i], 0);
            }
        }
    }
    while(!salir);
    return 0;
}


//Función para dibujar los botones.
void DibujaBoton(TBoton bot, int pres)
{
    //Variables para las cooordenadas de los botones.
    int xit, yit, x1, y1, x2, y2, col_li;
    x1=bot.x;
    y1=bot.y;
    x2=bot.x+bot.tamx;
    y2=bot.y+bot.tamy;

    //Impresión de los botones.
    setfillstyle(1, BLACK);
    bar(x1, y1, x2, y2);

    //Animaciones de los botones.

    //Botón sin presionar.
    col_li=(pres==0)?15:8;
    setcolor(col_li);
    line(x1, y1, x1, y2);
    line(x1, y1, x2, y1);

    //Botón presionado.
    col_li=(pres==0)?8:15;
    setcolor(col_li);
    line(x2, y1, x2, y2);
    line(x1, y2, x2, y2);

    //Impresión de texto en los botones.
    settextstyle(3, 0, 3);
    setbkcolor(BLACK);
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    xit=x1+((x2-x1)-textwidth(bot.texto))/2;
    yit=y1+((y2-y1)-textheight(bot.texto))/2;
    outtextxy(xit, yit, bot.texto);
    setbkcolor(0);
}


//Función para imprimir los botones en las distintas interfaces.
void Menu(TBoton botones[], int tamBot, int xi, int yi, int tamx, int tamy)
{
    int i;
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);

    //Ciclo para asignar las coordenadas a los botones (se utilizan las coordenadas del primer botón y a partir del mismo se imprimen el resto de botones).
    for(i=0; i<tamBot; i++)
    {
        botones[i].x=xi+(i*2*tamx);
        botones[i].y=yi;
        botones[i].tamx=tamx;

        botones[i].tamy=tamy;
    }

    //Ciclo para imprimir los botones.
    for(i=0; i<tamBot; i++)
        DibujaBoton(botones[i], 0);
}


//Función para imprimir el texto escrito desde el teclado en ventana de gráficos.
void inText(int x, int y, char texto[], int color_I, int color_F, char nombre[])
{
    int tecla, i=0;
    char cad[2];
    texto[0]='\0';

    //Do while que permite realizar las siguientes funciones mientras no se presione la tecla ENTER.
    do
    {
        //Do while para la animación del caracter "|" que simula el cursor de entrada de texto.
        do
        {
            setcolor(color_I);
            setbkcolor(0);
            outtextxy(x, y, "|");
            delay(300);
            setcolor((color_F));
            outtextxy(x, y, "|");
            delay(300);
        }
        while(!kbhit());
        tecla=getch();

        //Condicional para imprimir el caracter de la tecla que se presionó y capturarlo en el arreglo "texto", mientras no se presionen las teclas ESC, ENTER y BACKSPACE.
        if(tecla!=27 && tecla!=13 && tecla!=8)
        {
            cad[0]=tecla;
            cad[1]='\0';
            setcolor(color_I);
            setbkcolor(0);
            outtextxy(x, y, cad);
            x+=textwidth(cad);
            texto[i++]=cad[0];
            texto[i]='\0';
        }
        else
        {
            //Condicional que indica que si se presiona la tecla 8 (BACKSPACE) nos permita borrar el último caracter y regrese a la posición del mismo.
            if(tecla==8 && i>0)
            {
                cad[0]=texto[--i];
                cad[1]='\0';
                x-=textwidth(cad);
                setcolor(color_F);
                setbkcolor(0);
                outtextxy(x, y, cad);
                texto[i]='0';
            }
        }
    }
    while(tecla!=13);
    strcpy(nombre,texto);
}


//Función para imprimir la malla en la ventana de gráficos.
void imprimeMalla(int nc, int nr, int tamxm, int tamym)
{
    int i;
    int incx, incy;

    setcolor(15);
    rectangle(0, 0, tamxm, tamym);

    //Las variables incx y incy son incrementos, estas almacenan la cantidad de pixeles que habrá entre una linea y otra de la malla.
    incx = tamxm / nc;
    incy = tamym / nr;

    //La malla está compuesta por un rectangulo de 450x600 pixeles, las divisiones de los renglones y columnas se imprimen a traves de lineas.
    for(i = 1; i <= nc; i++)
        line(i*incx, 0, i*incx, tamym);

    for(i = 1; i <= nr; i++)
        line(0, i*incy, tamxm, i*incy);
}

/*
void dibujaVidas()
{
    setcolor(RED);
    circle(550,500,10);
    setfillstyle(1,RED);
    floodfill(550,500,RED);
    setcolor(RED);
    circle(575,500,10);
    setfillstyle(1,RED);
    floodfill(575,500,RED);
    setcolor(RED);
    circle(600,500,10);
    setfillstyle(1,RED);
    floodfill(600,500,RED);

}
*/


int ayuda(TBoton botones2[], int nBots2)
{
    int i = 0, x, y, salir = 0;

    readimagefile("HelpScreen.jpg", 0, 0, getmaxx(), getmaxy());

    Menu(botones2, nBots2, 550, 500, 100, 50);

    do

    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            printf("\nX: %d, Y: %d\n", x, y);

            while(i<nBots2)
            {
                //Condicional para verificar si se dio click dentro del área de algún botón.
                if(x >= botones2[i].x && x <= botones2[i].x + botones2[i].tamx && y >= botones2[i].y && y <= botones2[i].y + botones2[i].tamy)
                    break;
                else
                    i++;
            }

            //Condicional para realizar la animación de presionar botón.
            if(i<nBots2)
            {
                DibujaBoton(botones2[i], 1);
                delay(500);
                DibujaBoton(botones2[i], 0);
            }

            //Se imprime en la consola el número de botón que se presionó.
            printf("\nBotón %d presionado.\n", i);

            switch(i)
            {
            case 0:
                salir = 1;
            }

        }
    }
    while(salir == 0);
    cleardevice();
    return 0;
}

TNodo *crea_nodo(int x, int y)
{
    TNodo *aux;
    aux=(TNodo*)malloc(sizeof(TNodo));
    aux->x=x;
    aux->y=y;
    aux->aba=aux->arr=aux->der=aux->izq=NULL;
    return(aux);
}

void malla(TNodo **cab, int xi,int yi, int nc, int nr)
{
    int r,c;
    TNodo *aux,*ant;

    for(r=0; r<nr; r++)
        for(c=0; c<nc; c++)
            if(r == 0)
            {
                if(c == 0)
                {
                    aux = crea_nodo(xi+(INC*c), yi+(INC*r));
                    *cab = aux;
                    ant = aux;
                }
                else
                {
                    aux->der = crea_nodo(xi+(INC*c), yi+(INC*r));
                    aux->der->izq = aux;
                    aux = aux->der;
                }

            }
            else
            {
                if(c == 0)
                {
                    aux = crea_nodo(xi+(INC*c), yi+(INC*r));
                    aux->arr = ant;
                    ant->aba = aux;
                    ant = aux;
                }
                else
                {
                    aux->der = crea_nodo(xi+(INC*c), yi+(INC*r));
                    aux->der->izq = aux;
                    aux = aux->der;
                    aux->izq->arr->der->aba = aux;
                    aux->arr = aux->izq->arr->der;

                }
            }

}

/*
void cargaSprites(TNodo nodo, enemigo ene)
{
    void *imagen;
    unsigned int tamImagen, ancho, alto;
    FILE *archivo;
    archivo = fopen("Nave1", "rb");
    fread(&ancho, sizeof(unsigned), 1, archivo);
    fread(&alto, sizeof(unsigned), 1, archivo);
    tamImagen = imagesize(0, 0, ancho, alto);
    imagen = malloc(tamImagen);
    rewind(archivo);
    fread(imagen, tamImagen, 1, archivo);
    fclose(archivo);
    putimage(0, 0, imagen, COPY_PUT);

      void *imagen2;
    unsigned int tamImagen2, ancho2, alto2;
    FILE *archivo2;
    archivo2 = fopen("Enemigo1", "rb");
    fread(&ancho2, sizeof(unsigned), 1, archivo2);
    fread(&alto2, sizeof(unsigned), 1, archivo2);
    tamImagen2 = imagesize(0, 0, ancho2, alto2);
    imagen2 = malloc(tamImagen2);
    rewind(archivo2);
    fread(imagen, tamImagen2, 1, archivo2);
    fclose(archivo2);
    putimage(0, 0, imagen2, COPY_PUT);

      void *imagen3;
    unsigned int tamImagen3, ancho3, alto3;
    FILE *archivo3;
    archivo3 = fopen("Enemigo2", "rb");
    fread(&ancho, sizeof(unsigned), 1, archivo);
    fread(&alto, sizeof(unsigned), 1, archivo);
    tamImagen = imagesize(0, 0, ancho, alto);
    imagen = malloc(tamImagen);
    rewind(archivo);
    fread(imagen, tamImagen, 1, archivo);
    fclose(archivo);
    putimage(0, 0, imagen, COPY_PUT);

      void *imagen4;
    unsigned int tamImagen, ancho, alto;
    FILE *archivo;
    archivo = fopen("Enemigo3", "rb");
    fread(&ancho, sizeof(unsigned), 1, archivo);
    fread(&alto, sizeof(unsigned), 1, archivo);
    tamImagen = imagesize(0, 0, ancho, alto);
    imagen = malloc(tamImagen);
    rewind(archivo);
    fread(imagen, tamImagen, 1, archivo);
    fclose(archivo);
    putimage(0, 0, imagen, COPY_PUT);

      void *imagen5;
    unsigned int tamImagen, ancho, alto;
    FILE *archivo;
    archivo = fopen("Enemigo4", "rb");
    fread(&ancho, sizeof(unsigned), 1, archivo);
    fread(&alto, sizeof(unsigned), 1, archivo);
    tamImagen = imagesize(0, 0, ancho, alto);
    imagen = malloc(tamImagen);
    rewind(archivo);
    fread(imagen, tamImagen, 1, archivo);
    fclose(archivo);
    putimage(0, 0, imagen, COPY_PUT);

      void *imagen6;
    unsigned int tamImagen, ancho, alto;
    FILE *archivo;
    archivo = fopen("Enemigo5", "rb");
    fread(&ancho, sizeof(unsigned), 1, archivo);
    fread(&alto, sizeof(unsigned), 1, archivo);
    tamImagen = imagesize(0, 0, ancho, alto);
    imagen = malloc(tamImagen);
    rewind(archivo);
    fread(imagen, tamImagen, 1, archivo);
    fclose(archivo);
    putimage(0, 0, imagen, COPY_PUT);

      void *imagen2;
    unsigned int tamImagen, ancho, alto;
    FILE *archivo;
    archivo = fopen("Enemigo6", "rb");
    fread(&ancho, sizeof(unsigned), 1, archivo);
    fread(&alto, sizeof(unsigned), 1, archivo);
    tamImagen = imagesize(0, 0, ancho, alto);
    imagen = malloc(tamImagen);
    rewind(archivo);
    fread(imagen, tamImagen, 1, archivo);
    fclose(archivo);
    putimage(0, 0, imagen, COPY_PUT);

}
*/

void imprime_ambiente(char nombre[25], int punt, int col1, int col2, int col3)
{
    int n = 20;
    int c = 30;
    char punt1[40];
    itoa(punt,punt1,10);
    setcolor(WHITE);
    rectangle(0, 0, (c+1)*15, (n+1)*29);
    outtextxy(485, 40, "PLAYER NAME:");
    outtextxy(485, 65, nombre);
    outtextxy(485, 105, "LIVES: ");
    outtextxy(485, 175, "SCORE: ");
    outtextxy(485,200, punt1);
    outtextxy(485,285, "Nivel 1");
    outtextxy(485, 550, "PRESS |ESC| TO EXIT");
    setcolor(col1);
    circle(500, 150, 15);
    setfillstyle(1, col1);
    floodfill(500, 150, col1);
    setcolor(col2);
    circle(540, 150, 15);
    setfillstyle(1, col2);
    floodfill(540, 150, col2);
    setcolor(col3);
    circle(580, 150, 15);
    setfillstyle(1, col3);
    floodfill(580, 150, col3);
}


void imprimepts(int puntosxd)
{
    char punt1[40];
    itoa(puntosxd,punt1,10);
    setcolor(BLACK);
    rectangle(485, 200, 510, 240);
    floodfill(487, 202, BLACK);
    setcolor(WHITE);
    outtextxy(485,200, punt1);
}


void gameOver(int puntos)
{
    int tecla;
    tecla = getch();
    do
    {
        readimagefile("GameOver.jpg", 0, 0, getmaxx(), getmaxy());
    }while(tecla != 27);
    cleardevice();

}


void juego(char nombre[25], TSpace sp)
{
    int j, i = 0, vidas = 3, punt = 200, auxene1 = 0, auxene2 = 0, nivel = 150;

    void *nave;
    nave = malloc(imagesize(0, 0, 29, 29));
    readimagefile("obj/Debug/P/Nave1.gif",0, 0, 29, 29);
    getimage(0, 0, 29, 29, nave);

    void *nenemiga;
    nenemiga = malloc(imagesize(30, 0, 59, 29));
    readimagefile("obj/Debug/P/Enemigo1.gif", 30, 0, 59, 29);
    getimage(30 ,0, 59, 29, nenemiga);

    void *nenemiga2;
    nenemiga2 = malloc(imagesize(60, 0, 89, 29));
    readimagefile("obj/Debug/P/Enemigo2.gif", 60 ,0, 89, 29);
    getimage(60, 0, 89, 29, nenemiga2);

    /*void *nenemiga3 = NULL;
    nenemiga3 = malloc(imagesize(91,0,120,30));
    readimagefile("enemigo.jpg",91,0,120,30);
    getimage(91,0,120,30,nenemiga3);

    void *nenemiga4 = NULL;
    nenemiga4 = malloc(imagesize(121,0,150,30));
    readimagefile("Enemigo4.gif",121,0,150,30);
    getimage(121,0,150,30,nenemiga3);

    void *nenemiga5 = NULL;
    nenemiga5 = malloc(imagesize(151,0,180,30));
    readimagefile("Enemigo5.gif",151,0,180,30);
    getimage(151,0,180,30,nenemiga5);

    void *nenemiga6 = NULL;
    nenemiga6 = malloc(imagesize(181,0,210,30));
    readimagefile("Enemigo3.gif",181,0,210,30);
    getimage(181,0,210,30,nenemiga6);*/

    void *disparo[5];
    for(j = 0; j < 5; j++)
    {
        disparo[j] = malloc(imagesize(0,31,30,60)*5);
        readimagefile("Bala.gif",0,31,30,60);
        getimage(0,31,30,60,disparo[j]);
    }


    bala b[5];
    srand(time(NULL));
    enemigo ene, ene2;

    ene.x = (rand()%15)*30;
    auxene1 = ene.x;
    ene.y = 30;
    ene2.x = (rand()%15)*30;
    auxene2 = ene2.x;
    ene2.y = 30;

    TNodo *m;
    TNodo *corre;
    int dir = 5; //0. Arriba, 1. Derecha, 2.Abajo 3.Izquierda
    int tecla;
    int np = 0;
    malla(&m,0,0,15,20);

    //imprimeMalla(sp.nc,sp.nr,450,600);

    corre = m->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->der->der->der->der->der->der->der->der->der->der->der->der->der;
    setactivepage(0);
    cleardevice();
    putimage(corre->x,corre->y,nave,0);
    putimage(b[i].x,b[i].y,disparo[i],0);
    imprime_ambiente(nombre, punt, 4, 4, 4);
    setcolor(BLACK);
    rectangle(corre->x,corre->y,corre->x+INC,corre->y+INC); //pon imagen
    setactivepage(1);
    cleardevice();
    imprime_ambiente(nombre, punt, 4, 4, 4);
    setcolor(BLACK);
    rectangle(corre->x,corre->y,corre->x+INC,corre->y+INC); //pon imagen FONDO

    putimage(corre->x,corre->y,nave,0);
    putimage(auxene1, ene.y, nenemiga, 0);
    //putimage(auxene2, ene2.y, nenemiga2, 0);
    putpixel(1, 1, GREEN);

    if(b[i].existe == 1)
    {
        putimage(b[i].x,b[i].y,disparo[i],0);
    }

    setvisualpage(np);

    do
    {

        do
        {

            np = (np+1)%2;
            setactivepage(np);
            setfillstyle(1,0);
            bar(1,1,(30+1)*15-1,(20+1)*29-1);

            if(b[i].existe == 1)
            {

                putimage(b[i].x,b[i].y,disparo[i],0);

            }

            switch(dir)
            {
            case 0:
                if(corre->arr)
                    corre=corre->arr;
                break;
            case 1:
                if(corre->der)
                    corre=corre->der;

                break;
            case 2:
                if(corre->aba)
                    corre=corre->aba;

                break;
            case 3:
                if(corre->izq)
                    corre=corre->izq;
                break;

            }
            delay(50);
            setcolor(0);
            rectangle(corre->x,corre->y,corre->x+INC,corre->y+INC); //pon imagen
            putimage(corre->x,corre->y,nave,0);

            if(b[i].existe == 1)
            {
                putimage(b[i].x,b[i].y,disparo[i],0);
            }

            putimage(ene.x,ene.y,nenemiga,0);

            setvisualpage(np);
            if(ene.y + INC < getmaxy())
            {
                if(b[i].x >= auxene1 && b[i].x < auxene1 + INC && b[i].y >= ene.y && b[i].y < ene.y + INC)
                {
                    free(nenemiga);
                    free(disparo[j]);
                    printf("Colision enemigo 1\n");
                    punt += 10;
                    printf("%d", punt);
                    imprimepts(punt);

                    ene.x = (rand()%15)*30;
                    auxene1 = ene.x;
                    ene.y = 30;
                    ene.y += INC;
                }

                if(corre->x >= auxene1 && corre->x < auxene1 + INC && corre->y >= ene.y && corre->y < ene.y + INC)
                {
                    free(nenemiga);
                    printf("Chocaste carnal :(");
                    switch(vidas)
                    {
                        case 3:
                            imprime_ambiente(nombre, punt, 4, 4, 0);
                            vidas -= 1;
                            break;

                        case 2:
                            imprime_ambiente(nombre, punt, 4, 0, 0);
                            vidas -= 1;
                            break;

                        case 1:
                            imprime_ambiente(nombre, punt, 0, 0, 0);
                            vidas -= 1;
                            break;

                        case 0:
                            cleardevice();
                            readimagefile("GameOver.jpg", 0, 0, getmaxx(), getmaxy());
                            tecla = getch();
                            delay(100);
                            cleardevice();
                            break;
                    }

                    ene.x = (rand()%15)*30;
                    auxene1 = ene.x;
                    ene.y = 30;
                    ene.y += INC;

                }

                else
                {
                    ene.y += INC;
                    if(ene.y + INC >= 600)
                    {
                        free(nenemiga);

                        printf("Enemigo llegó\n");
                        punt -= 10;
                        printf("%d", punt);
                        imprimepts(punt);

                        ene.x = (rand()%15)*30;
                        auxene1 = ene.x;
                        ene.y = 30;
                        ene.y += INC;
                    }
                }
            }


            putimage(ene2.x, ene2.y, nenemiga2,0);
            if(ene2.y + INC < getmaxy())
            {
                if(b[i].x >= auxene2 && b[i].x < auxene2 + INC && b[i].y >= ene2.y && b[i].y < ene2.y + INC)
                {
                    free(nenemiga2);
                    free(disparo[j]);
                    printf("Colision enemigo 1\n");
                    punt += 10;
                    printf("%d", punt);
                    imprimepts(punt);

                    ene2.x = (rand()%15)*30;
                    auxene2 = ene2.x;
                    ene2.y = 30;
                    ene2.y += INC;
                }

                if(corre->x >= auxene2 && corre->x < auxene2 + INC && corre->y >= ene2.y && corre->y < ene2.y + INC)
                {
                    free(nenemiga2);
                    printf("Chocaste carnal :(");
                    switch(vidas)
                    {
                        case 3:
                            imprime_ambiente(nombre, punt, 4, 4, 0);
                            vidas -= 1;
                            break;

                        case 2:
                            imprime_ambiente(nombre, punt, 4, 0, 0);
                            vidas -= 1;
                            break;

                        case 1:
                            imprime_ambiente(nombre, punt, 0, 0, 0);
                            vidas -= 1;
                            break;

                        case 0:
                            cleardevice();
                            readimagefile("GameOver.jpg", 0, 0, getmaxx(), getmaxy());
                            tecla = getch();
                            delay(100);
                            cleardevice();
                            break;
                    }

                    ene2.x = (rand()%15)*30;
                    auxene2 = ene2.x;
                    ene2.y = 30;
                    ene2.y += INC;

                }

                else
                {
                    ene2.y += INC;
                    if(ene2.y + INC >= 600)
                    {
                        free(nenemiga2);

                        printf("Enemigo llegó\n");
                        punt -= 10;
                        printf("%d", punt);
                        imprimepts(punt);

                        ene2.x = (rand()%15)*30;
                        auxene2 = ene2.x;
                        ene2.y = 30;
                        ene2.y += INC;
                    }
                }
            }


            if(b[i].y > 5)
            {
                b[i].y -= INC;
            }
            else
            {
                b[i].existe = 0;
            }



            delay(nivel);

            if(punt == 210)
            {
                nivel = 100;
                setcolor(WHITE);
                outtextxy(485,325,"Nivel 2");
            }

            if(punt == 220)
            {
                nivel = 50;
                setcolor(WHITE);
                outtextxy(485,375,"Nivel 3");
            }

            if(punt == 0)
            {
                readimagefile("GameOver.jpg", 0, 0, getmaxx(), getmaxy());
                tecla = getch();
                delay(100);
                cleardevice();
            }


        }
        while(!kbhit());
        tecla=getch();
        if(!tecla)
            tecla = getch();
        switch(tecla)
        {
        case KEY_UP:
            dir = 0;
            break;
        case KEY_DOWN:
            dir = 2;
            break;
        case KEY_LEFT:
            dir = 3;
            break;
        case KEY_RIGHT:
            dir = 1;
            break;
        case 32:
            b[i].x=corre->x;
            b[i].y=corre->y-30;
            b[i].existe=1;
            j = j+1;
            break;
        }
    }
    while(tecla != 27);
    cleardevice();
}


void strreverse(char* begin, char* end)
{

	char aux;

	while(end>begin)

		aux=*end, *end--=*begin, *begin++=aux;

}


void itoa(int value, char* str, int base)
 {

	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	char* wstr=str;

	int sign;

	div_t res;




	// Validate base

	if (base<2 || base>35){ *wstr='\0'; return; }



	// Take care of sign

	if ((sign=value) < 0) value = -value;




	// Conversion. Number is reversed.

	do {

		res = div(value,base);

		*wstr++ = num[res.rem];

	}while((value = res.quot));

	if(sign<0) *wstr++='-';

	*wstr='\0';

	// Reverse string

	strreverse(str,wstr-1);

}

