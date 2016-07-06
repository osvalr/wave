#!/bin/bash
HOMEUSER="/home/$USER"           #Directorio del usuario
HOMEDESK="$HOMEUSER/Escritorio"  #Directorio Escritorio
WAVECONF="$HOMEUSER/.wave"       #Directorio de configuracion de wave

if [ -d $HOMEDESK ]; then
	DIR=$HOMEDESK;
else
	DIR=$HOMEUSER/Desktop;
fi

echo -n "Eliminando archivos de configuracion.. "
	if [ -d $WAVECONF ]; then
		rm -r $WAVECONF > /dev/null
		echo "OK"
	else
		echo "FALLÓ"
	fi

echo -n "Eliminando enlaces.. "
	if [ -f $DIR/wave.desktop ]; then
		rm $DIR/wave.desktop > /dev/null
		echo "OK"
	else
		echo "FALLÓ"
	fi

#echo "reinicializando la terminal.. en 3 segundos"
#sleep 3 && reset
