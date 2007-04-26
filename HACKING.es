###############################################

    Informacion sobre modificacion de TCOS

###############################################

 TCOS es un conjunto de scripts que copia binarios
 y liberias en una imagen de arranque, es posible 
 añadir más utilidaddes según se vayan necesitando.
 
 Usar como plantilla /etc/tcos/hacking/template

 Forma de añadir soporte a un programa:

 Ejemplo, añadir en la imagen soporte para display 
  ( visualizador de imagenes => ImageMagick )

 Crea un archivo en /etc/tcos/hacking/

 ####### /etc/tcos/hacking/display ##############
 # script TCOS para soporte del comando display

 cpifexists $(pathof display) /usr/bin

 ##############################################

 Ayudantes que podemos usar:

 * pathof PROGRAMA
     Devuelve la ruta completa (path) de una aplicación

     Es usado para conocer las rutas y almacenar su valor en
     otra variable o invocar a la función cpifexists.

     Ejemplos:
       dhclientbin=$(pathof dhclient)
       # La variable dhclientbin valdrá /sbin/dhclient

 * cpifexits [BINARIO] [RUTA_DESTINO]

    Copia un BINARIO dado por su ruta completa en
    RUTA_DESTINO dentro del initramfs 

    Esta función copia las _dependencias_, es decir 
    las librerías de las que depende el binario => man ldd
    
    Ejemplos:
      cpifexists $(pathof fdisk) /sbin/

   Internamente se ejecuta:
   $ ldd /sbin/fdisk
        linux-gate.so.1 =>  (0xffffe000)
        libc.so.6 => /lib/tls/libc.so.6 (0xb7e3e000)
        /lib/ld-linux.so.2 (0xb7f8f000)

  Y se copian las librerias enlazadas.
 NOTA: Se intentará no copiar nada que tenga que ver con /lib/tls/

 Tambien se pueden copiar archivos normales:
      cpifexists /etc/hosts.allow /etc/
 

 * copydir [DIR]  [RUTA_DESTINO]
    
    Copia un directorio dentro de la imagen initramfs
    equivale a ejecutar cp -ra DIR $INITRAMFS/$DESTINO
    
    Ejemplos:
       copydir /etc/dropbear/ /etc/

 * stat_before

     Crea un punto de ajuste de tamaño, vease stat_after

 * stat_after "TEXTO"

    Lee el punto de ajuste de tamaño creado por stat_before,
    calcula la diferencia en Kb y lo imprime por pantalla

    Ejemplo completo:

       stat_before
       cpifexists $(pathof display) /usr/bin/
       stat_after "ImageMagick display"

     Esto copia el binario (y librerías) en el initramfs
     y calcula el tamaño en Kb que hemos añadido a la imagen con 
     esta accion, por lo que imprimiría por pantalla:

     => Package ImageMagick display get XXX Kb.
