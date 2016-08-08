/*
Universidad del Valle de Guatemala
Algoritmos y estructura de datos

Xavier Cifuentes Pérez 13316
Pablo de León Rodas 13227
Ana Fernanda Solares 13125
Jose Rosales 12576
Julio Isaac Rodriguez 14298
Julian Estuardo Escobedo 14684

*/

#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"
#include "ping.h"
#include "stack.h"

//constantes
int dist;
int adelante;
int izquierda;
int derecha;
int giro;
int flgTope;

int irLeft;
int irRight;
int maxSide;


int main()                                    // Main function
{
  // Add startup code here.
  
  //tiempo de espera
  //verde
  high(7);
  pause(1000);
  low(7);
  //rojo
  high(6);
  pause(500);
  low(6);
  
   // Add startup code here.
  dist = 17;
  
  giro = 0;
  adelante = 0;
  izquierda = 0;
  derecha = 0;
  
  flgTope = 0;
  
  //se recomienda dejar un margen de error
  //lado maximo
  maxSide = 30;
  
  create();
  

  while(1)
  {
    // Add main loop code here.
    
    //revisa ambos lados y regresa los valores en irLeft y irRight
    /*
     *Cambio: el check se realiza dentro de verIR()
     *  ademas ahora solo se verifica izquierda y derecha con el 
     *  ultrasonico y se cambian las banderas si es o no camino
    */

      verIR();
      //revisar frente
      verFrente();      
    
    //todos los diferentes casos para intersecciones
    if((adelante == 1 && izquierda == 0 && derecha == 1  && flgTope == 0) || (adelante == 1 && izquierda == 1 && derecha == 0  && flgTope == 0) || (adelante == 0 && izquierda == 1 && derecha == 1  && flgTope == 0) || (adelante == 1 && izquierda == 1 && derecha == 1  && flgTope == 0) || (adelante == 0 && izquierda == 0 && derecha == 0  && flgTope == 0)){
      decidirC();
    }
    
    //si ha llegado a interseccion pero flgTope ==1 se retoma interseccion
    if((adelante == 1 && izquierda == 0 && derecha == 1 && flgTope == 1) || (adelante == 1 && izquierda == 1 && derecha == 0 && flgTope == 1) || (adelante == 0 && izquierda == 1 && derecha == 1 && flgTope == 1) || (adelante == 1 && izquierda == 1 && derecha == 1 && flgTope == 1)){
      //leer tope de pila, corregirGiro y actualizar interseccion
      returnInter();
    }
    
    else{
      soloGirar();
    }
    
    //moverse
    drive_goto(46,46);
              
  }  
}


/*
**************************************************************
                    funciones principales
**************************************************************
*/
//************************************************************/
//************************************************************/
void verIR(){
  
  //se obtienen valores de los IR's
  low(26);
  low(27);
  
  irLeft = 0;
  irRight = 0;
  
  for (int dacVal=0; dacVal<200; dacVal += 4){
    dac_ctr(26, 0, dacVal);
    freqout(11, 1, 38000);
    irLeft += input(10);
    
    dac_ctr(27, 1, dacVal);
    freqout(3, 1, 38000);
    irRight += input(4);
  }
  
  //se compara para ver si es interseccion
  if (irLeft>maxSide || irRight>maxSide){
    //codigo de colores 
    ledVerde();
    ledRojo();
    //se revisan los lados con el ultrasonico
    revSidePing();
  }    
  
}

//************************************************************/
//************************************************************/
void verFrente(){
  if (ping_cm(8) > dist){
    adelante = 1;
    //codigo
    ledVerde();
  }
  
  else{
    revSidePing();
  }
}

//************************************************************/
//************************************************************/
//funcion para toma decision hacia donde virar
void decidirC(){
  if(adelante == 0 && izquierda == 0 && derecha == 0){
    //dar la vuelta 2 veces a la derecha
    drive_goto(26,-25);
    drive_goto(26,-25);
    //activar bandera de tope
    flgTope = 1;
  }
  else{
    if(adelante == 1 && izquierda == 0 && derecha == 1){
    //girar derecha
    drive_goto(26,-25);
    derecha = 0;
    giro = 1;
    }
    if(adelante == 1 && izquierda == 1 && derecha == 0){
      //ir hacia adelante
      adelante = 0;
      giro = 2;
    }
    if(adelante == 0 && izquierda == 1 && derecha == 1){
      //ir derecha
      drive_goto(26,-25);
      derecha = 0;
      giro = 1;
    }
    if(adelante == 1 && izquierda == 1 && derecha == 1){
      //ir derecha
      drive_goto(26,-25);
      derecha = 0;
      giro = 1;
    }
    //mandar datos a la pila
    beep();
    sendStack();
  }   
  //reset contadores
  resetCont(); 
}

//************************************************************/
//************************************************************/
void returnInter(){
  //codigo
  beep();
  beep();
  int dato = topelement();
  //eliminar tope de pila
  pop();
  int arreglo[] = {0,0,0,0,0};
  int contador = 4;
  while(dato > 0){
    int digit = dato % 10;
    arreglo[contador] = digit;
    contador--;
    dato /= 10;
  }
  int left = arreglo[1];
  int front = arreglo[2];
  int right = arreglo[3];
  int spin = arreglo[4];
  
  //reset orientacion a la original
  corregirGiro(spin);
  
  if(left == 0 && front == 0 && right == 0){
    // girar 2 veces y regresar a otra interseccion
    drive_goto(26,-25);
    drive_goto(26,-25);
    flgTope = 1;
  }
  else{
    if(left == 1 && front == 0 && right == 0){
      //ir a la izquierda
      drive_goto(-25,26);
      left = 0;
      spin = 3;
    }
    if(left == 1 && front == 1 && right == 0){
      //ir hacia adelante
      front = 0;
      spin = 2;
    }
    //mandar a pila interseccion actualizada
    sendNewS(left, front, right, spin);
    //reset bandera de tope
    flgTope = 0;
  }
  //reset contadores
  resetCont();
}

//************************************************************/
//************************************************************/
void soloGirar(){
  if(adelante == 0 && izquierda == 1 && derecha == 0){
    //codigo
    ledRojo();
    ledRojo();
    ledVerde();
    //girar izquierda
    drive_goto(-25,26);
  }
  if(adelante == 0 && izquierda == 0 && derecha == 1){
    //codigo
    ledRojo();
    ledRojo();
    ledVerde();
    //girar derecha
    drive_goto(26,-25);
  }
  if(adelante == 1 && izquierda == 0 && derecha == 0){
    //codigo
    ledRojo();
    ledRojo();
    ledVerde();
    //seguir adelante
    freqout(0,500, 1000);
  }
  resetCont();
}

/*
*******************************************************
                    funciones secundarias
*******************************************************
*/

//************************************************************/
//************************************************************/
//revisa 2 lados con el sensor ultrasonico
/*
 *Cambio: funcion similar a revLados, pero se omite la revision
 *  del frente
*/          
void revSidePing(){
  //revisar derecha
  drive_goto(26,-25);
  if (ping_cm(8) > dist){
    derecha = 1;
    //codigo
    ledVerde();
  }
  
  //revisar izquierda
  drive_goto(-50,51);
  if (ping_cm(8)> dist){
    izquierda = 1;
    //codigo
    ledVerde();
  }
  drive_goto(26,-25);
}

//************************************************************/
//************************************************************/
void sendStack(){
  //usar un 1 de inicio para que sea significativo el 0 de izquierda
  int dato = 10000+izquierda*1000+adelante*100+derecha*10+giro;
  //mandar a pila
  push(dato);
}    

//************************************************************/
//************************************************************/
void corregirGiro(int g){
  ledVerde();
  beep();
  if(g == 1){
    //girar derecha
    drive_goto(26,-25);
  }
  if(g == 2){
    // girar 2 veces a la derecha
    drive_goto(26,-25);
    drive_goto(26,-25);
  }
  if(g == 3){
    //girar izquierda
    drive_goto(-25,26);
  }
}         

//************************************************************/
//************************************************************/
void sendNewS(int l, int f, int r, int g){
  int dato = 10000+l*1000+f*100+r*10+g;
  //madar a pila
  push(dato);
}   


/*
*******************************************************
                    subrutinas
*******************************************************
*/

void ledRojo(){
  high(6);
  pause(20);
  low(6);
}

void ledVerde(){
  high(7);
  pause(20);
  low(7);
}

void resetCont(){
  giro = 0;
  adelante = 0;
  izquierda = 0;
  derecha = 0;
}

void beep(){
  freqout(5,500,500);
}  
