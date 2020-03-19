#include "fusion-c/header/msx_fusion.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


//Declaración de funciones, en C esto es necesario (suelen meterse en un archivo.h)
void inicializar_variables_juego();
void loop_principal();
char obtener_tecla_del_teclado();
void pausar_hasta_que_se_pulsa_una_tecla();
void seleccionar_palabra_del_array();
char convertir_letra_a_mayuscula(char tecla);
void comprobar_que_la_letra_esta_en_la_palabra(char letra);
int comprobar_que_la_tecla_no_es_la_misma(char letra);
void mostrar_fecha();
void chequear_vidas();
void comprobar_ganador();
int obtener_segundos();
int obtener_tamanio_string(char[]);
void posicionar_cursor_en_pantalla(int x, int y);
void borrar_pantalla();
//void comprobar_repeticiones_letra(unsigned char letra);

void mostrar_palabra_sin_resolver();
void mostrar_ahorcado_con_las_vidas();
void mostrar_palabra_seleccionada();
void imprimirArray();

void quitar_click_cursor();
void quitar_cursor();
void terminar_programa();

//Declaración de variables globales
//char arrayPalabras[][50]={"CENTRIFUGADO","SUBMARINO","DIALOGUE","AUTORICE","ADULTERIO","NEUMATICO ","AUDITORES","PIJO","CLARO","MSX"};
//char arrayPalabras[][50]={"DESOXIRRI","CALEIDOSCOPIO","SONRISA","MSX CLARO PIJO","GALAXIA","UMBRELLA","MOUNTAIN","STEPDAUGHTER","COCODRILO","SOMBRERO"};
char arrayPalabras[][50]={"TORPEDO","GOOGLE","SANGUIJUELA","ONOMATOPELLA","POETISA","OMOPLATO","CINTURON","BOCADILLO","JARABE","COCHE",
                           "CENTRIFUGADO","SUBMARINO","DIALOGUE","ELECTROENCEFALOGRAFISTA ","ADULTERIO","NEUMATICO ","AUDITORES","PIJO","CLARO","MSX",
                           "DESOXIRRIBONUCLEICO","CALEIDOSCOPIO","SONRISA","EXTRATERRESTRE","GALAXIA","UMBRELLA","MOUNTAIN","STEPDAUGHTER","COCODRILO","SOMBRERO"};

#define NUM_PALABRAS_A_DESCIFRAR 30
          
char palabra_seleccionada[]="";
int numero_letras_palabra_seleccionada;
char palabra_sin_resolver[15];
int letras_acertadas;  
int vidas;
char tecla;







int main(void)
{
    tecla=' ';
    quitar_click_cursor();
    //quitar_cursor();
    //Esta variable se utiliza para generar la palabra vacía que se irá descubriendo
    numero_letras_palabra_seleccionada=0;
    printf("\n\n\rBienvenido a ahorcado hecho en c para MSX utilizando SDCC");
    mostrar_fecha();
    printf("\n\nPulse una tecla para jugar.");
    //WaitKey es una función de fusion c que va a esperar que se presione una tecla para continuar
    pausar_hasta_que_se_pulsa_una_tecla();

    //Deshabilitamos el cursor
    //ChangeCap(1);
    inicializar_variables_juego();
    seleccionar_palabra_del_array();
    loop_principal();

    return 0;
}








void loop_principal(){
    //Aunque las vidas sean 7 le restaremos una por defecto por la primera letra
    //Variables para el manejo de la entrada del teclado
    char letra_obtenida_del_teclado;
    char tecla_covertida_en_mayuscula='@';
    char tecla_ultima_vez=' ';

   

    //Bucle principal, se repetirá has que las vidas sean 0
    while(vidas>0){
        //Borramos la pantalla
        borrar_pantalla();
        //comprobar_repeticiones_letra(tecla_ultima_vez);
        // Imagenes y textos
        mostrar_ahorcado_con_las_vidas();
        mostrar_palabra_sin_resolver();
        // Este método solo se activará para fines de desarrollo
        //mostrar_palabra_seleccionada();
        //La 1 vez no se mostrará el mensaje ya que omparamos con @ que se le asigna al principio
        if(tecla_covertida_en_mayuscula!='@'){
            printf("\n\rTecla pulsada: %c",tecla_covertida_en_mayuscula);
        }
        

        //Mensaje
        printf("\n\rEsperando que pulse una letra...");
        // 2.Pedimos una tecla del teclado y la almacenamos e la variable letra_obtenida_teclado
        letra_obtenida_del_teclado=obtener_tecla_del_teclado();
        // 3.Convertimos la letra de entrada a mayúscula para comprarla con las otras que también están en mayúscula
        tecla_covertida_en_mayuscula=convertir_letra_a_mayuscula(letra_obtenida_del_teclado);
        // 4.Comprobamos si la tecla es la misma que la ultima vez
        if(tecla_covertida_en_mayuscula!=tecla_ultima_vez){
            //Comprobamos si la tecla fue resuleta
            if (StrChr(palabra_sin_resolver, tecla_covertida_en_mayuscula)==-1){
                 // 6.Comprobamos que esta o no en la palabra y le quitamos vidas o sumamos aciertos
                comprobar_que_la_letra_esta_en_la_palabra(tecla_covertida_en_mayuscula);  
                //Comprobamos si hay varias repeticiones de la letra
                //comprobar_repeticiones_letra(tecla_covertida_en_mayuscula);
                //7. Terminamos el programa si las vidas son 0 y mostramos un mensaje de si quiere repetir
                chequear_vidas();
                //8.El jugador ganará si ha resulto todas las letras
                comprobar_ganador();
            }            
        }
        tecla_ultima_vez=tecla_covertida_en_mayuscula;
    }  
}









void inicializar_variables_juego(){
    numero_letras_palabra_seleccionada=0;
    letras_acertadas=0;
    vidas=7;
    seleccionar_palabra_del_array();
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






void seleccionar_palabra_del_array(){
    //rand es una función que está dentro de la librería Math.c del core de c
    //Pero 1 tenemos que actualizar la semilla para que no nos genere siempre la misma secuencia
    srand(obtener_segundos());
    //printf("semilla -->%i", semilla);
    //strcpy(palabra_seleccionada,arrayPalabras[rand()%10]);
    //Prueba con 3 palabras en el arrar
    strcpy(palabra_seleccionada,arrayPalabras[rand()%NUM_PALABRAS_A_DESCIFRAR]);
    numero_letras_palabra_seleccionada=obtener_tamanio_string(palabra_seleccionada);
    //Creamos una nueva palabra con las mismas letras pero con espacios en blanco
    int i;
    for(i=0;i<numero_letras_palabra_seleccionada;i++){
        palabra_sin_resolver[i]=' ';    
    }
}

int obtener_segundos(){
    
    //Fusion-c
    TIME time;
    GetTime(&time);
    return time.sec;
    
    //C
    //time_t time_t;
    //return time(&time_t);
}


int obtener_tamanio_string(char string[]){
    //Fusion-c
    //return StrLen(string);
    //C
    return strlen(string);
}









char obtener_tecla_del_teclado(){
    //fusion-c
    return WaitKey();
    //C
    //scanf("%c", &tecla);
    //return tecla;
}
void pausar_hasta_que_se_pulsa_una_tecla(){
    //Fusion-c
    WaitKey();
    //C
    //scanf("%c",NULL);
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
        borrar_pantalla();
        printf("\r\nHas perdido. Otra partida? s/n");
        mostrar_ahorcado_con_las_vidas();
        char otra_partida=obtener_tecla_del_teclado();
        if(otra_partida=='s'){
            borrar_pantalla();
            inicializar_variables_juego();
            borrar_pantalla();
        }else if(otra_partida=='n'){
            borrar_pantalla();
            terminar_programa();
        }
    }
}
 

//Esta función comprobará que el número de aciertos sea ingual al numero de letras de la palabra seleccionada
void comprobar_ganador(){
    if (obtener_tamanio_string(palabra_seleccionada)-letras_acertadas==0){
        borrar_pantalla();
        posicionar_cursor_en_pantalla(10,8);
        printf("\r\nFelicidades!!!! \n\rhas ganado!!\n\rLa palabra era \n\r%s. \n\rOtra partida? s/n",palabra_seleccionada);
        mostrar_ahorcado_con_las_vidas();
        char otra_partida=WaitKey();
        if(otra_partida=='s'){
            borrar_pantalla();
            inicializar_variables_juego();
            loop_principal();
        }else if(otra_partida=='n'){
            borrar_pantalla();
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
    //int repeticiones=0;
     //Obtenemos el tamaño de la palabra para recorrerla
    for (int i=0;i<obtener_tamanio_string(palabra_seleccionada);i++)
    {
        //Si en esa posicion del string está la letra pulsada
        if(palabra_seleccionada[i]==letra){
            letras_acertadas++;
            palabra_sin_resolver[i]=letra;
        }
    }  
    //Esto es solo con fines de depuración 
    //posicionar_cursor_en_pantalla(20,17);
    //printf("Repeticiones: %d",repeticions);
    //Si se ha encontrado la letra en el string sumamos a letras acertadas sino le restamos vida
    //StrChr nos devolverá -1 si no la encuentra
    posicion_letra_en_la_palabra=StrChr(palabra_seleccionada, letra);
    //printf("\n\rPalabra, %s, posicion letra: %i",palabra_seleccionada,posicion);
    if(posicion_letra_en_la_palabra==-1){
         vidas -=1;
    }
}






void mostrar_palabra_sin_resolver(){
    int posicionX=2;
    for (int i=0;i<numero_letras_palabra_seleccionada;i++){
        posicionar_cursor_en_pantalla(posicionX,12);
        printf("%c ", palabra_sin_resolver[i]);
        posicionar_cursor_en_pantalla(posicionX,13);
        printf("_ ");
        posicionX++;
    }
    posicionar_cursor_en_pantalla(0,0);
}









void posicionar_cursor_en_pantalla(int x, int y){
    //Fusion-c
    Locate(x,y);
    /*
    //C
    //identificador único que identifica la venta, lo llamaremos handle console
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    //COORD es un struct con las coordenaadsas del sursor
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    //Tiene 2 parametrors ,
    //1.el handle que es el identficador que le dá una id a la ventana,
    //2. es un struct COORD definido en windows.h que tiene 2 atributos la x y la y (las coordenadas del cursor)
    SetConsoleCursorPosition(hCon, dwPos);
    */
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
    posicionar_cursor_en_pantalla(23,5);
    printf("Vidas: %i",vidas);

    //Escribimos los aciertos abajo de la horca
    posicionar_cursor_en_pantalla(12,16);
    printf("Acertadas: %i, quedan: %i",letras_acertadas, obtener_tamanio_string(palabra_seleccionada)-letras_acertadas);

    //Pintamos el suelo
    posicionar_cursor_en_pantalla(27,15);
    PrintChar(223);
    posicionar_cursor_en_pantalla(28,15);
    PrintChar(223);
    posicionar_cursor_en_pantalla(29,15);
    PrintChar(223);
    posicionar_cursor_en_pantalla(30,15);
    PrintChar(223);
    posicionar_cursor_en_pantalla(31,15);
    PrintChar(223);


    //Pintamos el poste vertical
    for (posicionPoste; posicionPoste>7;posicionPoste--){
        posicionar_cursor_en_pantalla(29,posicionPoste);
        PrintChar(221);
    }
    //Pintamos el poste de arriba
    for (posicionPosteArriba;posicionPosteArriba>23;posicionPosteArriba--){
        posicionar_cursor_en_pantalla(posicionPosteArriba,7);
        PrintChar(221);
    }

    posicionar_cursor_en_pantalla(25,7);
    PrintChar(221);
    posicionar_cursor_en_pantalla(25,8);
    PrintChar(221);
    if(vidas<6){
        //Imagen de la cabeza
        posicionar_cursor_en_pantalla(25,9);
        PrintChar(0x99);
        if(vidas<5){
            //Imagen del cuerpo
            posicionar_cursor_en_pantalla(25,10);
            PrintChar(0xdb);
            if(vidas<4){
                //imagen del brazo izquierdo
                posicionar_cursor_en_pantalla(24,10);
                PrintChar(0xa9);
                if(vidas<3){
                    //Imagen del brazo derecho
                    posicionar_cursor_en_pantalla(26,10);
                    PrintChar(0xaa);
                    if(vidas<2){
                        //Imagen pierna izquierda
                        posicionar_cursor_en_pantalla(25,11);
                        PrintChar(0xc6);
                        //Imagen pierna derecha
                        if(vidas<1){
                            posicionar_cursor_en_pantalla(26,11);
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
            posicionar_cursor_en_pantalla(15,15);
            PrintChar();
            break;
    }
    */
    posicionar_cursor_en_pantalla(0,0);
}


void mostrar_palabra_seleccionada(){
    posicionar_cursor_en_pantalla(0,20);
    printf("\n\r%s",palabra_seleccionada);
    posicionar_cursor_en_pantalla(0,0);
}


void mostrar_fecha(){
    DATE date;
    GetDate(&date);
    printf("\n\n\r------------------\n\r ");
    printf("\nMSX claro pijo %d de %d del %d",date.day,date.month,date.year);
    printf("\n\n\r------------------\n\r ");
    printf("\n\n\n\n\n\r ");
}





void borrar_pantalla(){
    //fusion-c
    Cls();
    //c
    /*
    for (int fila=0;fila<50;fila++){
        for (int columna=0; columna<120;columna++){
            posicionar_cursor_en_pantalla(fila,columna);
            printf(" ");
        }

    }
    */
}

/*******************
*******************
*****FIN DE UI*****
*******************
*******************/



























