#!/bin/bash
#Para trabajar wn windows con git bash+script shell


APP="main"
SO="windows"
CC="GCC"

function createExe(){
    #al poner la opción -g le estamos diciendo que estamos preparando el programa para gdb
    #Comados gdb
    #abrir gdb: nombre_programa.exe
    #q:sale
    #list: lee las 10 primeras lineas
    #list nombre_duncion: lista las lineas de la funcion
    #run: ejecuta el programa
    #kill:termina
    #break nombre_funcion: pone un punto de ruptura y el programa parará cuando lo ejecutemos con run
    #info b: muestra la lista d los breakpoints
    #del b: borra el breakpint
    #next: muestra la linea
    #print nombre_variable: nos muestra el valor de las variables
    #ptype nombre_variable: muestra el tipo
    #continue:sale del modo del breakpoint
    #step: se mete dentro de la función, se trabaja con up, down y finish
    gcc main.c -include defines.h -g -o ahorcado.exe
    #gdb ahorcado.exe
    start ahorcado.exe
}



function createAndOpenDisk(){
    #disk
    SDCC --code-loc 0x106 --data-loc 0x0 --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size  fusion.lib -Lfusion-c/lib/ fusion-c/include/crt0_msxdos.rel ${APP}.c
    ./tools/hex2bin/hex2bin.exe -e com ${APP}.ihx
    mv ${APP}.com dsk/
    ./tools/openMSX/openmsx.exe -machine Philips_NMS_8255 -diska dsk/ 
    clean
}

function createAndOpenRomLinkAndCompile(){
    #rom compilación y enzalado a la vez
    sdasz80 -o ./fusion-c/include/crt0msx_rom.rel ./fusion-c/source/crt0_rom.asm
    #sdasz80 -o -l main.rel main.s
    sdcc -mz80 --debug --constseg DUMMY -c ${APP}.c -o ${APP}.rel
    SDCC --code-loc 0x4000 --data-loc 0xc000 --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size fusion.lib -Lfusion-c/lib/ fusion-c/include/crt0msx_rom.rel ${APP}.c
    ./tools/hex2bin/hex2bin.exe -e com ${APP}.ihx
    mv ${APP}.com dsk/
    ./tools/openMSX/openmsx.exe -machine Philips_NMS_8255 -diska dsk/ 
    clean
}



function createAndOpenDSKLinkAndCompile(){
    #dsk compilación y enlazado por separado
    sdasz80 -o ./fusion-c/include/crt0msx_rom.rel ./fusion-c/source/crt0msx_rom.s
    sdcc -mz80 --debug --constseg DUMMY -c ${APP}.c -o ${APP}.rel
    sdcc -mz80 --debug --constseg DUMMY --no-std-crt0 --code-loc 0x106 --data-loc 0x0 --out-fmt-ihx -o ${APP}.ihx fusion.lib -L ./fusion-c/lib/ ./fusion-c/include/crt0_msxdos.rel ${APP}.rel
    # Creando una carpeta para mover nuestros archivos
    if [ $SO == "windows" ]
    then
        tools/hex2bin/hex2bin.exe -e com ${APP}.ihx
        mv  ${APP}.com dsk
        tools/openMSX/openmsx.exe -machine Philips_NMS_8255 -diska dsk/ &
    else
        ./tools/hex2bin/hex2bin -e com ${APP}.ihx
        mv ${APP}.com dsk/
        ./tools/openMSX/openmsx -machine Philips_NMS_8255 -diska dsk/ &
        #gcc -g  ${APP}.c -o ${APP}
        #ddd& --trace --debugger ./openmsxgdb.py ${APP} 2> ddd_openmsx.log
        %nombrePrograma%.c
    fi
}




##rom
#Cuando arrancas el MSX con un cartucho metido esto es lo que ve el MSX
#|       RAM           |
#|       RAM           |
#|       ROM Cartucho  |
#|       Bios          |
#16k
#Para decirle que el 3 slot sea ROM cartucho tenemos que hacerlo desde ensamblador
#sdasz80 -o ./fusion-c/include/crt0msx_rom.rel ./fusion-c/source/crt0_rom16.asm
#sdasz80 -l -o ${APP}.rel main-asm-16k.s
#sdcc -mz80 --debug --constseg DUMMY -c ${APP}.c -o ${APP}.rel
#sdcc -mz80 --debug --constseg DUMMY -c main-simple-asm.c -o ${APP}.rel
#./tools/fillfile/fillfile ${APP}.rom 16384 

#32k
#sdasz80 -o ./fusion-c/include/crt0msx_rom.rel ./fusion-c/source/crt0_rom32.asm
#sdasz80 -l -o ${APP}.rel main-asm-32k.s
#sdcc -mz80 --debug --constseg DUMMY --no-std-crt0 --code-loc 0x4100 --data-loc 0x7800 --out-fmt-ihx -o ${APP}.ihx fusion.lib -L ./fusion-c/lib/ ./fusion-c/include/crt0msx_rom.rel ${APP}.rel
#./tools/hex2bin/hex2bin.exe -e rom  ${APP}.ihx
#./tools/fillfile/fillfile ${APP}.rom 32768  


#Abriendo el emulador con un rom
#./tools/openMSX/openmsx.exe -carta ${APP}.rom -machine Philips_NMS_8255 &

#Depurando
#gcc -g  fusion-c/lib/fusion.lib -I. ${APP}.c -o ${APP}
#ddd& --trace --debugger ./openmsxgdb.py ${APP} 2> ddd_openmsx.log


function clean(){
    rm -f *.adb
    rm -f *.asm
    rm -f *.cdb
    rm -f *.ihx
    rm -f *.lib
    rm -f *.lk
    rm -f *.lst
    rm -f *.map
    rm -f *.noi
    rm -f *.rel
    # rm -f *.rom
    rm -f *.sym
    rm -f *.log
}


## Check parameter number
if [ $# != 1 ]
then
    if [ $CC == "SDCC" ]
    then
        createAndOpenDisk
    else
        createExe
    fi
fi

PARAM=${1^^}   ## Uppercase
case "$PARAM" in
   "CLEAN")
      clean
   ;;
esac
case "$PARAM" in
    "GCC")
        createExe
    ;;
esac
case "$PARAM" in
    "SDCC")
        createAndOpenDisk
    ;;
esac

