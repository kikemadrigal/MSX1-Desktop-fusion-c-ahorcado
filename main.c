#include "fusion-c/header/msx_fusion.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


//Declaración de funciones, en C esto es necesario (suelen meterse en un archivo.h)
void inicializar_variables_juego();
void loop_principal();
void seleccionar_palabra_del_array();
char convertir_letra_a_mayuscula(char tecla);
void comprobar_que_la_letra_esta_en_la_palabra(char letra);
int comprobar_que_la_tecla_no_es_la_misma(char letra);
void mostrar_fecha();
void chequear_vidas();
void comprobar_ganador();

void mostrar_palabra_sin_resolver();
void mostrar_ahorcado_con_las_vidas();
void mostrar_palabra_seleccionada();
void imprimirArray();

void quitar_click_cursor();
void quitar_cursor();
void terminar_programa();

//Declaración de variables globales
char arrayPalabras[][50]={"CENTRIFUGADO","SUBMARINO","DIALOGUE","AUTORICE","ADULTERIO","NEUMATICO ","AUDITORES","PIJO","CLARO","MSX"};
char palabra_seleccionada[]="";
int numero_letras_palabra_seleccionada;
char palabra_sin_resolver[15];
int letras_acertadas;  
int vidas;


void main(void) 
{
    quitar_click_cursor();
    quitar_cursor();
    //Esta variable se utiliza para generar la palabra vacía que se irá descubriendo
    numero_letras_palabra_seleccionada=0;
    printf("\n\n\rBienvenido a ahorcado hecho en c para MSX utilizando SDCC");
    mostrar_fecha();
    printf("\n\nPulse una tecla para jugar.");
    //WaitKey es una función de fusion c que va a esperar que se presione una tecla para continuar
    WaitKey();
    //Deshabilitamos el cursor
    ChangeCap(1);
    inicializar_variables_juego();
    seleccionar_palabra_del_array();
    loop_principal();
}


















void quitar_click_cursor(){
    __asm
        xor	a		
        ld	(0xF3DB),a
    __endasm;
}

void quitar_cursor(){
    __asm
        ;ld a,#0x00	
        ;ld (0x0132),a
    __endasm;
}

//Esta función finaliza el programa, ver http://map.grauw.nl/resources/dos2_functioncalls.php
//y ver la sección 3.1 PROGRAM TERMINATE (00H), yan solo hay que poner en el registro c 0
void terminar_programa(){
    __asm
        ld c,#0x0
        call #0x0005
    __endasm;
}



void inicializar_variables_juego(){
    numero_letras_palabra_seleccionada=0;
    letras_acertadas=0;
    vidas=7;
    seleccionar_palabra_del_array();
}


void seleccionar_palabra_del_array(){
    //rand es una función que está dentro de la librería Math.c del core de c
    TIME time;
    GetTime(&time);
    srand(time.sec);
    int semilla=0;    
    semilla=rand()%10;
    //printf("semilla -->%i", semilla);
    strcpy(palabra_seleccionada,arrayPalabras[semilla]);
    numero_letras_palabra_seleccionada=StrLen(palabra_seleccionada);
    //Creamos una nueva palabra con las mismas letras pero con espacios en blanco
    int i;
    for(i=0;i<numero_letras_palabra_seleccionada;i++){
        palabra_sin_resolver[i]=' ';
        
    }
    //printf("Palabra seleccionada dentro de la funcion seleccionar: %s\n\r",palabra_seleccionada);
}


void loop_principal(){
    //Aunque las vidas sean 7 le restaremos una por defecto por la primera letra
    //Variables para el manejo de la entrada del teclado
    char letra_obtenida_del_teclado;
    char tecla_covertida_en_mayuscula='@';
    char tecla_ultima_vez;
    int la_tecla_es_o_no_es_la_misma_que_la_ultima_vez;
 
    //Bucle principal, se repetirá has que las vidas sean 0
    while(vidas>0){
        //Borramos la pantalla
        Cls();
        //1. Imagenes y textos
        mostrar_ahorcado_con_las_vidas();
        mostrar_palabra_sin_resolver();
               
        //La 1 vez no se mostrará el mensaje ya que omparamos con @ que se le asigna al principio
        if(StrCompare(tecla_covertida_en_mayuscula,'@')!=0){
            printf("\n\rTecla pulsada: %c",tecla_covertida_en_mayuscula);
        }
        
        //Esto es solo para fines de depuración
        //mostrar_palabra_seleccionada();
        //printf("\n\rEs la misma que la ultima vez?: %i",la_tecla_es_o_no_es_la_misma_que_la_ultima_vez);
        //printf("\n\rLa tecla está en la pantalla?: %i",la_tecla_esta_o_no_en_la_palabra);

        //Mensaje
        printf("\n\rEsperando que pulse una letra...");
        // 2.Pedimos una tecla del teclado y la almacenamos e la variable letra_obtenida_teclado
        letra_obtenida_del_teclado=WaitKey(); 
        // 3.Convertimos la letra de entrada a mayúscula para comprarla con las otras que también están en mayúscula
        tecla_covertida_en_mayuscula=convertir_letra_a_mayuscula(letra_obtenida_del_teclado);
        // 4.Comprobamos si la tecla es la misma que la ultima vez
        la_tecla_es_o_no_es_la_misma_que_la_ultima_vez=StrCompare(tecla_covertida_en_mayuscula,tecla_ultima_vez);
        // 5.Si es la misma que la ultima vez StrCompare dará 0 y no queremos que se vuelva a comprobar la misma
        if(la_tecla_es_o_no_es_la_misma_que_la_ultima_vez!=0){
            // 6.Comprobamos que esta o no en la palabra y le quitamos vidas o sumamos aciertos
            comprobar_que_la_letra_esta_en_la_palabra(tecla_covertida_en_mayuscula);  
            //7. Terminamos el programa si las vidas son 0 y mostramos un mensaje de si quiere repetir
            chequear_vidas();
            //8. Si el número de aciertos es el número de letras de la palabra gana
            comprobar_ganador();
        }        
        tecla_ultima_vez=tecla_covertida_en_mayuscula;
    }  
}
























char convertir_letra_a_mayuscula(char tecla){
    char tecla_obtenida;
    //Para entender esto mira la tabla ascii http://www.asciitable.com/
    //Se trata de restarle 32 para convertiralas todas en mayúsculas
    //printf("\n\rTecla obtenida: %c", tecla);
    if(tecla>96){
    tecla_obtenida=tecla-32;
    }else{
        tecla_obtenida=tecla;
    }
    return tecla_obtenida;
}

void chequear_vidas(){
    if(vidas==0){
        Cls();
        printf("\r\nHas perdido. Otra partida? s/n");
        mostrar_ahorcado_con_las_vidas();
        char otra_partida=WaitKey();
        int valor=0;
        //la funcion StrCompare está explicada en el Main
        valor=StrCompare(otra_partida, 's');
        if(valor==0){
            Cls();
            inicializar_variables_juego();
            loop_principal();
        }else if(StrCompare(otra_partida, "n")){
            Cls();
            terminar_programa();
        }
    }
}
 

//Esta función comprobará que el número de aciertos sea ingual al numero de letras de la palabra seleccionada
void comprobar_ganador(){
    if (letras_acertadas==numero_letras_palabra_seleccionada){
        Cls();
        Locate(10,10);
        printf("\r\nFelicidades!!!! \n\rhas ganado!!. \n\rOtra partida? s/n");
        mostrar_ahorcado_con_las_vidas();
        //char caracter='s';
        char otra_partida=WaitKey();
        int valor=0;
        //la funcion StrCompare está explicada en el Main
        valor=StrCompare(otra_partida, 's');
        if(valor==0){
            Cls();
            inicializar_variables_juego();
            loop_principal();
        }else if(StrCompare(otra_partida, "n")){
            Cls();
            printf("\r\n\n\n\nAdios.");
            terminar_programa();
        }
    }
}

/*
void imprimirArray(){
	int longitud =sizeof(palabra_sin_resolver)/sizeof(palabra_sin_resolver[0]);
	int i;
	for(i=0; i<longitud;i++){
		//strcpy(palabra,palabra_sin_resolver[i]);
		printf("%i: %c",i,palabra_sin_resolver[i]);
	}
}
*/







void comprobar_que_la_letra_esta_en_la_palabra(unsigned char letra){
    int posicion_letra_en_la_palabra=0;
    //La función StrChr de fusion c si la letra (el char) es encontrada dentro de palabra_seleccionada, sino devuelve -1
    posicion_letra_en_la_palabra=StrChr(palabra_seleccionada, letra);
    //printf("\n\rPalabra, %s, posicion letra: %i",palabra_seleccionada,posicion);
    if(posicion_letra_en_la_palabra!=-1){
        palabra_sin_resolver[posicion_letra_en_la_palabra]=letra;
        letras_acertadas +=1;
    }else{
        vidas -=1;
    }
}



void mostrar_palabra_sin_resolver(){
    int posicionX=7;
    for (int i=0;i<numero_letras_palabra_seleccionada;i++){
        Locate(posicionX,12);
        printf("%c ", palabra_sin_resolver[i]);
        Locate(posicionX,13);
        printf("_ "); 
        posicionX++;
    }
    Locate(0,0);
}


















/*******************
*******************
********UI*********
*******************
*******************/



void mostrar_ahorcado_con_las_vidas(){
    //Por favor mirar la tabla ascii que está en la carpeta images para saber como ponr las imágenes
    int posicionPoste=14;
    int posicionPosteArriba=29;
    //Escribimos las vidas arriba de la horca
    Locate(23,5);
    printf("Vidas: %i",vidas);

    //Escribimos los aciertos abajo de la horca
    Locate(23,16);
    printf("Acertadas: %i",letras_acertadas);

    //Pintamos el suelo
    Locate(27,15);
    PrintChar(223);
    Locate(28,15);
    PrintChar(223);
    Locate(29,15);
    PrintChar(223);
    Locate(30,15);
    PrintChar(223);
    Locate(31,15);
    PrintChar(223);


    //Pintamos el poste vertical
    for (posicionPoste; posicionPoste>7;posicionPoste--){
        Locate(29,posicionPoste);
        PrintChar(221);
    }
    //Pintamos el poste de arriba
    for (posicionPosteArriba;posicionPosteArriba>23;posicionPosteArriba--){
        Locate(posicionPosteArriba,7);
        PrintChar(221);
    }
  
    Locate(25,7);
    PrintChar(221);
    Locate(25,8);
    PrintChar(221);
    if(vidas<6){
        //Imagen de la cabeza
        Locate(25,9);
        PrintChar(0x99);
        if(vidas<5){
            //Imagen del cuerpo
            Locate(25,10);
            PrintChar(0xdb);
            if(vidas<4){
                //imagen del brazo izquierdo
                Locate(24,10);
                PrintChar(0xa9);
                if(vidas<3){
                    //Imagen del brazo derecho
                    Locate(26,10);
                    PrintChar(0xaa);
                    if(vidas<2){
                        //Imagen pierna izquierda
                        Locate(25,11);
                        PrintChar(0xc6);
                        //Imagen pierna derecha
                        if(vidas<1){
                            Locate(26,11);
                            PrintChar(0xc6);
                        }
                    }
                }
               
            }  
        }

    }

 
    /*
    switch (vidas){
        case 1:
            Locate(15,15);
            PrintChar();
            break;
    }
    */
    Locate(0,0);
}


void mostrar_palabra_seleccionada(){
    Locate(0,20);
    printf("\n\r%s",palabra_seleccionada);
    Locate(0,0);
}


void mostrar_fecha(){
    DATE date;
    GetDate(&date);
    printf("\n\n\r------------------\n\r ");
    printf("\nMSX claro pijo %d de %d del %d",date.day,date.month,date.year);
    printf("\n\n\r------------------\n\r ");
    printf("\n\n\n\n\n\r ");
}

/*******************
*******************
*****FIN DE UI*****
*******************
*******************/



























