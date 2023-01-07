
# Descripción

Ahorcado es un juego escrito en C para:

MSX-DOS2 que se ha escrito utilizando la librería fusion-c.

Windows

Sistemas unix


Cuando se trabaja con el lenguaje C es interesante hacer varias versiones para por ejemplo depurar en linux.

Puedes descargar el [dsk pinchando aquí](https://github.com/kikemadrigal/ahorcado/blob/master/resources/ahorcado.zip)

Puedes ver la release [pinchando aquí](https://github.com/kikemadrigal/ahorcado/releases/tag/0.0.1)



## Versión Windows
<img src="img/6.PNG" width="400" />

## Versión MSX
<!--![Imagen programa 1](img/2.PNG)-->
<img src="img/2.PNG" width="400" />


El paquete lleva incluido el emulador openMSX para que tan solo tengas que poner en la ventanade comandos de windows "compil.bat main":

<!--![Imagen programa 1](img/1.PNG)-->
<img src="img/1.PNG" width="400" />

## Probándo el juego con blueMSX

Necesitarás tener [está carpeta](https://github.com/kikemadrigal/ahorcado/blob/master/resources/MSX2-MSX-DOS2.zip) copiada y pegada en el directorio "machines" de tu bluemsx que si lo has instalado estrá en "C:\Program Files (x86)\blueMSX\Machines". Después selecciona el emulador MSX2 - MSX-DOS2 y por último el archivo ahorcado.dsk descrito al principio, presiona Shift(o mayúsculas izquierda)+f4 para el modo turb:

<img src="img/3.PNG" width="400" />
<img src="img/4.PNG" width="400" />

## Probándo el juego con openMSX
Necesitarás tener copiados y pegados todos los archivos de [este carpeta](https://github.com/kikemadrigal/ahorcado/blob/master/resources/systemroms.zip) en la carpeta systemroms de openmsx, si lo tienes instalado estará en "C:\Program Files\openMSX\share", después deberás de elegir la extensión msxdos2 de tu catapult-openmsx y un emulador con disquetera por ejemplo SONY HB-F700P, debrás seleccionar el archivo ahorcado.dsk descrito al principio, presion F9 para el modo turbo:

<img src="img/5.PNG" width="400" />

# Reqisitos si quieres probar el código fuente

Necesitarás tener instalado el compilador sdcc en tu equipo [SDCC.es](http://sdcc.sourceforge.net/index.php#Download)


# Recursos

Aquí tienes un enlace donde están explicadas todas las funciones de fusion-c: [Fusion c](https://github.com/ericb59/Fusion-C-v1.2/blob/master/FUSION-C-Quick%20A4%201.2.pdf)