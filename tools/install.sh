#!/bin/bash
HOMEUSER="/home/$USER"           #Directorio del usuario
HOMEDESK="$HOMEUSER/Escritorio"  #Directorio Escritorio
WAVECONF="$HOMEUSER/.wave"       #Directorio de configuracion de wave
clear

if [ -d $HOMEDESK ]; then
	DIR=$HOMEDESK;
else
	DIR=$HOMEUSER/Desktop;
fi

echo -n "Creando directorio de configuracion.. "
	if [ ! -d $WAVECONF ]; then 
		mkdir $WAVECONF > /dev/null
		echo "OK"
	else
		echo "FALLÓ"
	fi

echo -n "Copiando archivos de configuracion.. "
	if [ -f $WAVECONF/wave ] && [ -d $WAVECONF/images ] && [ -f $WAVECONF/wave.conf ]; then
		echo "YA EXISTEN"
	else
		cp -a wave images/ wave.conf $WAVECONF > /dev/null
		if [ -f $WAVECONF/wave ] && [ -d $WAVECONF/images ] && [ -f $WAVECONF/wave.conf ]; then
			chmod -R 766 $WAVECONF
			echo "OK"
		else
			echo "FALLÓ"
		fi
	fi

echo -n "Creando acceso directo.. "
	if [ -f $DIR/wave.desktop ]; then
		echo "YA EXISTE"
	else
		echo "[Desktop Entry]\nVersion=1.0\nName=Wave\nGenericName=Wave\nComment=Cliente de IM que usa la tarjeta de sonido\nExec=$HOMEUSER/.wave/./wave\nTerminal=false\nIcon=$HOMEUSER/.wave/images/icon.png\nType=Application\nCategories=Sound;Media;\nComment[es_VE.utf8]=Cliente de IM que usa la tarjeta de sonido" > $DIR/wave.desktop 
		chmod 744 $DIR/wave.desktop
		echo "OK"
	fi

#echo "reinicializando la terminal.. en 3 segundos"
#sleep 3 && reset

