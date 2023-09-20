#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <allegro.h>
#include <time.h>
#include <math.h>

typedef struct punto{
    int x;
    int y;
}comidita;

comidita meal;

typedef struct snake{
    int x;
    int y;
    int sdir;
    int pos;
    struct snake *next;
    struct snake *ant;
}parte;

typedef struct NODO {
    parte *prin;
    parte *fin;
}Serpientita;

Serpientita Lista;

BITMAP *buffer;
BITMAP *borde_v_d;
BITMAP *borde_h_ab;
BITMAP *rArD;
BITMAP *rAbD;
BITMAP *rArI;
BITMAP *rAbI;
BITMAP *cabezabmp;
BITMAP *cabeza;
BITMAP *cuerpobmp;
BITMAP *cuerpo;
BITMAP *colabmp;
BITMAP *cola;
BITMAP *girobmp;
BITMAP *giro;
BITMAP *comida;
BITMAP *GOver;
BITMAP *suelo;
BITMAP *Inicio;
BITMAP *Inicio1;
BITMAP *Inicio2;
BITMAP *Inicio3;
BITMAP *cursor;
BITMAP *rio;
BITMAP *borde_int;
BITMAP *nivel1;
BITMAP *nivel2;
BITMAP *nivel3;
BITMAP *niveles;

char* lecturaFichArray(char*,int);
void pantalla(void);
void dibujar_mapa(char[]);
void serpiente();
void creacion(int ,int);
void cuerpazo(int[],char[]);
void update(int,int,int);
void comer(int[],char[]);
int colision(void);
int choque(int,int,int[],char[]);
int inicio(void);
int* saltos_linea(char[],int*);
int numero_s_linea(char []);
int nivelitos(void);

int main(void)
{
    allegro_init();
    install_keyboard();
    install_mouse();

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,700,700,0,0);

    buffer = create_bitmap(700,700);

    borde_h_ab=load_bitmap("imagenes\\borde_h_ab.bmp",NULL);
    borde_v_d=load_bitmap("imagenes\\borde_v_d.bmp",NULL);
    rArD=load_bitmap("imagenes\\rincon_ar_de.bmp",NULL);
    rArI=load_bitmap("imagenes\\rincon_ar_iz.bmp",NULL);
    rAbD=load_bitmap("imagenes\\rincon_ab_de.bmp",NULL);
    rAbI=load_bitmap("imagenes\\rincon_ab_iz.bmp",NULL);
    suelo=load_bitmap("imagenes\\suelo.bmp",NULL);
    comida=load_bitmap("imagenes\\comida.bmp",NULL);
    cabezabmp=load_bitmap("imagenes\\cabeza.bmp",NULL);
    colabmp=load_bitmap("imagenes\\cola.bmp",NULL);
    GOver=load_bitmap("imagenes\\gover.bmp",NULL);
    girobmp=load_bitmap("imagenes\\giro.bmp",NULL);
    rio=load_bitmap("imagenes\\rio.bmp",NULL);
    borde_int=load_bitmap("imagenes\\borde_interior.bmp",NULL);
    giro=create_bitmap(40,40);
    cabeza=create_bitmap(40,40);
    cola=create_bitmap(40,40);
    cuerpobmp=load_bitmap("imagenes\\cuerpo.bmp",NULL);
    cuerpo=create_bitmap(40,40);


    int opcion=0,lvl=1;
    opcion=inicio();
    if(opcion==3){
       lvl=nivelitos();
    }

    char *mapa;
    mapa=lecturaFichArray(mapa,lvl);
    int* saltos;
    saltos=saltos_linea(mapa,saltos);

    int x=5,y=5,dir;
    creacion(x,y);
    comer(saltos,mapa);
    while(!key[KEY_ESC]){
        dibujar_mapa(mapa);
        fflush(stdin);
        if(key[KEY_W]){
            dir=0;
            y-=1;
        }
        else if(key[KEY_D]){
            dir=1;
            x+=1;
        }
        else if(key[KEY_S]){
            dir=2;
            y+=1;
        }
        else if(key[KEY_A]){
            dir=3;
            x-=1;
        }
        else{
            if(dir==0){
                y-=1;
            }
            else if(dir==1){
                x+=1;
            }
            else if(dir==2){
                y+=1;
            }
            else if(dir==3){
                x-=1;
            }
        }
        if(choque(x,y,saltos,mapa)==1){
                break;
            }

        draw_sprite(buffer, comida, meal.x*40, meal.y*40);
        cuerpazo(saltos,mapa);
        if(keypressed()==TRUE)
        update(x,y,dir);
        serpiente();
        if(colision()==0){
            break;
        }
        pantalla();
        rest(100);
        clear(cuerpo);
        clear(cola);
        clear(cabeza);
        clear(giro);
        clear(buffer);
        rest(100);
    }
    clear(buffer);
    blit(GOver,buffer,0,0,0,0,500,500);
    pantalla();
    rest(1000);
    return 0;
}END_OF_MAIN();

int* saltos_linea(char mapa[],int *saltos){
    int ayuda=0;
    int x;
    for(x=0;x<strlen(mapa);x++){
        if(mapa[x]=='\n'){
            ayuda++;
        }
    }
    saltos=(int*)calloc(ayuda,sizeof(int));
    int i=0;
    for(x=0;x<strlen(mapa);x++){
        if(mapa[x]=='\n'){
            saltos[i]=x+1;
            //printf(".%i-",saltos[i]);
            i++;
        }
    }
    return saltos;
}

char* lecturaFichArray(char *mibufer,int lvl){
  unsigned char prueba[1];
  FILE *archivo;
  int caracteres=0;
  char fichero[15]="mapas\\mapa1.txt";
    fichero[10]=lvl+'0';

  archivo = fopen(fichero, "r");
  if (!archivo) {
    printf("No se pudo abrir el archivo %s", "mapa.txt");
    return mibufer;
  }

  while (!feof(archivo)) {
    fread(prueba, sizeof(char), sizeof(prueba), archivo);
    caracteres++;
  }
  fclose(archivo);
  strset(fichero,'\0');
  strcpy(fichero,"mapas\\mapa1.txt");
  fichero[10]=lvl+'0';
  archivo = fopen(fichero, "r");
   if (!archivo) {
    printf("No se pudo abrir el archivo %s", "mapa.txt");
    return mibufer;
  }

  mibufer=(char*)calloc(caracteres+1,sizeof(char));
  strset(mibufer,'\0');
  for(int i=0;i<sizeof(mibufer);i++){
    mibufer[i]='\0';
  }
    fread(mibufer,sizeof(char),caracteres,archivo);
    fwrite(mibufer, sizeof(char),caracteres -1, 0);
  fclose(archivo);
  return mibufer;
}

void dibujar_mapa(char mapa[]){
    int x=0,y=0,cont=0;

    for(cont =0;cont<strlen(mapa);cont++){
         if(mapa[cont] =='-'){
            draw_sprite(buffer, borde_h_ab, x*40, y*40);
        }
        else if(mapa[cont] =='|'){
            draw_sprite(buffer, borde_v_d, x*40, y*40);
        }
        else if(mapa[cont]=='1'){
            draw_sprite(buffer, rArI,x*40,y*40);
        }
        else if(mapa[cont]=='2'){
            draw_sprite(buffer, rArD,x*40,y*40);
        }
        else if(mapa[cont]=='3'){
            draw_sprite(buffer, rAbI,x*40,y*40);
        }
        else if(mapa[cont]=='4'){
            draw_sprite(buffer, rAbD,x*40,y*40);
        }
        else if(mapa[cont]==' '){
            draw_sprite(buffer,suelo,x*40,y*40);
        }
        else if(mapa[cont]=='0'){
            draw_sprite(buffer,rio,x*40,y*40);
        }
        else if(mapa[cont]=='.'){
            draw_sprite(buffer,borde_int,x*40,y*40);
        }
        if(mapa[cont]=='\n'){
            y++;
            x=0;
        }
        else{
            x++;
        }
    }

}

void pantalla(){
    blit(buffer,screen,0,0,0,0,700,700);
}

void creacion(int x,int y){
    int dir=0;
    Lista.prin=NULL;
    Lista.fin=NULL;

    Lista.prin=(parte*)calloc(1,sizeof(parte));
    Lista.fin=(parte*)calloc(1,sizeof(parte));
    Lista.prin->pos=0;
    Lista.prin->sdir=dir;
    Lista.prin->x=x;
    Lista.prin->y=y;
    Lista.prin->ant=NULL;
    Lista.prin->next=Lista.fin;
    Lista.fin->pos=1;
    Lista.fin->sdir=dir;
    Lista.fin->next=NULL;
    Lista.fin->ant=Lista.prin;
    Lista.fin->x=Lista.fin->ant->x;
    Lista.fin->y=Lista.fin->ant->y+1;
}

void serpiente(){
    parte *aux;
    aux=Lista.prin;
    int cant;
    cant=Lista.fin->pos;
    int n;
    for(n=0;n<=cant;n++){

        if (n==0){
            blit(cabezabmp,cabeza,aux->sdir*40,0,0,0,40,40);
            draw_sprite(buffer,cabeza,aux->x*40,aux->y*40);
        }
        if(n!=0&&n!=cant){
            if((aux->ant->sdir==2&&aux->sdir==3)||(aux->ant->sdir==1&&aux->sdir==0)){
                blit(girobmp,giro,3*40,0,0,0,40,40);
                draw_sprite(buffer,giro,aux->x*40,aux->y*40);
            }
            else if((aux->ant->sdir==2&&aux->sdir==1)||(aux->ant->sdir==3&&aux->sdir==0)){
                blit(girobmp,giro,0*40,0,0,0,40,40);
                draw_sprite(buffer,giro,aux->x*40,aux->y*40);
            }
            else if((aux->ant->sdir==0&&aux->sdir==1)||(aux->ant->sdir==3&&aux->sdir==2)){
                blit(girobmp,giro,1*40,0,0,0,40,40);
                draw_sprite(buffer,giro,aux->x*40,aux->y*40);
            }
            else if((aux->ant->sdir==1&&aux->sdir==2)||(aux->ant->sdir==0&&aux->sdir==3)){
                blit(girobmp,giro,2*40,0,0,0,40,40);
                draw_sprite(buffer,giro,aux->x*40,aux->y*40);
            }
            else{
                blit(cuerpobmp,cuerpo,(aux->sdir%2)*40,0,0,0,40,40);
                draw_sprite(buffer,cuerpo,aux->x*40,aux->y*40);
            }
        }
        if(n==cant){
            aux->sdir=aux->ant->sdir;
            blit(colabmp,cola,aux->sdir*40,0,0,0,40,40);
            draw_sprite(buffer,cola,aux->x*40,aux->y*40);
        }
        aux=aux->next;
    }
}

void cuerpazo(int saltos[],char mapa[]){
    parte *aux2,*aux;
    aux=(parte *)calloc(1,sizeof(parte));
    if(Lista.prin->x==meal.x&&Lista.prin->y==meal.y){
        comer(saltos,mapa);
        if(Lista.fin->pos==1){
            Lista.prin->next=aux;
            aux->next=Lista.fin;
            Lista.fin->ant=aux;
            aux->ant=Lista.prin;
            aux->sdir=aux->ant->sdir;
            aux->pos=aux->ant->pos+1;
            aux->next->pos=aux->pos+1;
            if(Lista.prin->sdir==0){
                aux->x=aux->ant->x;
                aux->y=aux->ant->y+1;
                Lista.fin->x=Lista.fin->ant->x;
                Lista.fin->y=Lista.fin->ant->y+1;
            }
            else if(Lista.prin->sdir==1){
                aux->x=aux->ant->x-1;
                aux->y=aux->ant->y;
                Lista.fin->x=Lista.fin->ant->x-1;
                Lista.fin->y=Lista.fin->ant->y;
            }
            else if(Lista.prin->sdir==2){
                aux->x=aux->ant->x;
                aux->y=aux->ant->y-1;
                Lista.fin->x=Lista.fin->ant->x;
                Lista.fin->y=Lista.fin->ant->y-1;
            }
            else if(Lista.prin->sdir==3){
                aux->x=aux->ant->x+1;
                aux->y=aux->ant->y;
                Lista.fin->x=Lista.fin->ant->x+1;
                Lista.fin->y=Lista.fin->ant->y;
            }
        }
        else{
            aux2=Lista.fin->ant;
            aux2->next=aux;
            aux->ant=aux2;
            Lista.fin->ant=aux;
            aux->next=Lista.fin;
            aux->sdir=aux->ant->sdir;
            aux->pos=aux->ant->pos+1;
            aux->next->pos=aux->pos+1;
            if(aux->sdir==0){
                aux->x=aux->ant->x;
                aux->y=aux->ant->y+1;
                Lista.fin->x=Lista.fin->ant->x;
                Lista.fin->y=Lista.fin->ant->y+1;
            }
            else if(aux->sdir==1){
                aux->x=aux->ant->x-1;
                aux->y=aux->ant->y;
                Lista.fin->x=Lista.fin->ant->x-1;
                Lista.fin->y=Lista.fin->ant->y;
            }
            else if(aux->sdir==2){
                aux->x=aux->ant->x;
                aux->y=aux->ant->y-1;
                Lista.fin->x=Lista.fin->ant->x;
                Lista.fin->y=Lista.fin->ant->y-1;
            }
            else if(aux->sdir==3){
                aux->x=aux->ant->x+1;
                aux->y=aux->ant->y;
                Lista.fin->x=Lista.fin->ant->x+1;
                Lista.fin->y=Lista.fin->ant->y;
            }
        }
    }
}

void update(int x,int y, int dir){
    fflush(stdin);

    parte *aux;
    aux=Lista.fin;
    int cant=Lista.fin->pos;
    int n;
    for(n=0;n<cant;n++){
        aux->sdir=aux->ant->sdir;
        aux->x=aux->ant->x;
        aux->y=aux->ant->y;
        aux=aux->ant;
    }
    aux->x=x;
    aux->y=y;
    aux->sdir=dir;
}


void comer(int saltos[],char mapa[]){
    int n_lineas;
    n_lineas=numero_s_linea(mapa);
    //for(;;){

    int q=1;
    int n=0;
    int p=0;
    int cant=0,existe=0;
    int *posibles;
    parte *aux2;

    aux2=Lista.prin;
    for(p=0;p<=Lista.fin->pos;p++){
        mapa[saltos[(aux2->y-1)]+aux2->x]='x';
        aux2=aux2->next;
    }
    //printf("%s\n\n",mapa);
    aux2=NULL;

    do{
        meal.x=0;
        meal.y=0;
        srand(time(NULL));
        existe=0;
        do{
            n=rand()%(n_lineas);
            if(n==0){
                n++;
            }
            //printf("\n\n yo soy n %i\n\n",n);
            for(p=saltos[n-1];p<saltos[n];p++){
                //printf("\n\n yo soy p %i\n\n",p);
                if(mapa[p]==' '){
                    existe=1;
                    //printf("\n\n yo soy mapa[p] %c y p %i\n\n",mapa[p],p);
                    break;
                }
            }
        }while(existe==0);
        meal.y=n;
        //printf("\n\n yo soy y %i\n\n",meal.y);
        existe=0;
        for(p=0;p<saltos[0];p++){
            if(mapa[saltos[meal.y-1]+p]==' '){
                existe++;
            }
            //printf("%c",mapa[saltos[meal.y-1]+p]);
        }
       // printf("\n\n yo soy existe %i\n\n",existe);
        posibles=(int*)calloc(existe,sizeof(int));
        existe=0;
        for(p=0;p<saltos[0];p++){
            if(mapa[saltos[meal.y-1]+p]==' '){
                posibles[existe]=saltos[meal.y-1]+p;
                //printf("-.%i.-",posibles[existe]);
                existe++;
            }
        }
        //printf("\n\n yo soy existe %i\n\n",existe);
        n=posibles[rand()%existe]-saltos[meal.y-1];
        meal.x=n;
        //printf("\n\n yo soy x %i\n\n",meal.x);
        if(meal.y==0){
            cant=meal.x;
        }
        else{
            cant=(meal.y*saltos[0])+meal.x;
        }
        //printf("\n%c\n",mapa[cant]);
        if(mapa[cant]==' '){
            q=0;
        }
    }while(q!=0);
    aux2=Lista.prin;
    for(p=0;p<=Lista.fin->pos;p++){
        mapa[saltos[(aux2->y-1)]+aux2->x]=' ';
        aux2=aux2->next;
    }
    //printf("%s\n\n",mapa);
    aux2=NULL;
    /*printf("\n\n%s",mapa);
    mapa[(meal.y*14)+meal.x]='x';
    printf("\n\n%s",mapa);
    getchar();*/
    //}

    return 0;
}


int colision(void){
    parte *aux;
    int n,cont;
    aux=Lista.fin;
    cont=Lista.fin->pos;
    for(n=0;n<cont;n++){
        if(Lista.prin->x==aux->x&&Lista.prin->y==aux->y){
            return 0;
        }
        aux=aux->ant;
    }
    return 1;
}

int inicio(void){
    Inicio=load_bitmap("imagenes\\Inicio.bmp",NULL);
    Inicio1=load_bitmap("imagenes\\Inicio1.bmp",NULL);
    Inicio2=load_bitmap("imagenes\\Inicio2.bmp",NULL);
    Inicio3=load_bitmap("imagenes\\Inicio3.bmp",NULL);
    cursor=load_bitmap("imagenes\\cursor.bmp",NULL);

    int sigo=0;

    while(sigo==0){
        if(mouse_x > 149 && mouse_x < 217 &&
           mouse_y > 190 && mouse_y < 212 ){
            blit(Inicio1,buffer,0,0,0,0,500,500);
            if(mouse_b & 1){
                sigo=1;
            }
        }
        else if(mouse_x > 152 && mouse_x < 208 &&
           mouse_y > 298 && mouse_y < 319 ){
            blit(Inicio2,buffer,0,0,0,0,400,400);
            if(mouse_b & 1){
                sigo=2;
            }
        }
        else if(mouse_x > 136 && mouse_x < 226 &&
           mouse_y > 240 && mouse_y < 261 ){
            blit(Inicio3,buffer,0,0,0,0,400,400);
            if(mouse_b & 1){
                sigo=3;
            }
        }
        else {
        blit(Inicio,buffer,0,0,0,0,400,400);
        }
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,30,30);
        blit(buffer,screen,0,0,0,0,400,400);
    }
    return sigo;
}

int choque(int x,int y,int saltos[],char mapa[]){
    int cant;
    if(y==0){
       cant=x;
    }
    else{
        cant=saltos[y-1]+x;
    }
    if(mapa[cant]!=' '){
        return 1;
    }
    else{
        return 0;
    }
}


int nivelitos(void){
    nivel1=load_bitmap("imagenes\\lvl1.bmp",NULL);
    nivel2=load_bitmap("imagenes\\lvl2.bmp",NULL);
    nivel3=load_bitmap("imagenes\\lvl3.bmp",NULL);
    niveles=load_bitmap("imagenes\\lvls.bmp",NULL);
    cursor=load_bitmap("imagenes\\cursor.bmp",NULL);
    int sigo=0;

    while(sigo==0){
        blit(niveles,buffer,0,0,0,0,400,400);
        if(mouse_x > 5 && mouse_x < 34 &&
           mouse_y > 5 && mouse_y < 35 ){
            blit(nivel1,buffer,0,0,0,0,40,40);
            if(mouse_b & 1){
                sigo=1;
            }
        }
        else if(mouse_x > 45 && mouse_x < 74 &&
           mouse_y > 5 && mouse_y < 35 ){
            blit(nivel2,buffer,0,0,40,0,40,40);
            if(mouse_b & 1){
                sigo=2;
            }
        }
        else if(mouse_x > 85 && mouse_x < 114 &&
           mouse_y > 5 && mouse_y < 35 ){
            blit(nivel3,buffer,0,0,80,0,40,40);
            if(mouse_b & 1){
                sigo=3;
            }
        }
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,30,30);
        blit(buffer,screen,0,0,0,0,400,400);
    }
    return sigo;
}

int numero_s_linea(char mapa[]){
    int ayuda=0;
    int x;
    for(x=0;x<strlen(mapa);x++){
        if(mapa[x]=='\n'){
            ayuda++;
        }
    }
    return ayuda;
}
