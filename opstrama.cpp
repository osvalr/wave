#include "opstrama.h"

uint32_t crc_division( trama_t t, int num_bytes )
{
	uint64_t residuo;
	int i, j;
	residuo = t[0]<<24 | t[1]<<16 | t[2]<<8 | t[3];    // Se asignan los primeros 32 bits a residuo para comenzar la división

	//Comienza la división	
	for( i = 4; i < num_bytes; i++ )    // i comienza en 4, porque ya estan en residuo los primeros 4 bytes de la cadena
		for( j = 7; j >= 0; j-- )
		{
			residuo <<= 1;

			if( t[i] & 1<<j )   // Si el bit en la posicion i(contando de izq a der) es uno se suma al residuo(es como si se bajara un 1 en la div)
				residuo += 1;

			if( residuo & BIT_33 )	// Si el bit mas significativo de los 33 es 1, se procede con la operación XOR.
				residuo ^= CRC_32;
		}	
	return (uint32_t) residuo;
}

void crc_32( trama_t t, int num_bytes )
{
	uint32_t residuo;
	int i, index;
	uint8_t *n;

	index = num_bytes;
	t[index] = t[index+1] = t[index+2] = t[index+3] = 0;	// Se agregan los ceros al final de la cadena por el grado del polinomio , en 4 posiciones ( siendo 32 posiciones )
	residuo = crc_division(t,num_bytes+4);

	if( residuo )//Si el residuo de la division es distinto a 0(cero) se suma el residuo a la cadena
	{
		n = ( uint8_t * ) &residuo;
		for ( i = 3; i >= 0; i--, index++ )
			t[index] = n[i];
	}
}

void mostrar_byte( byte_t b )
{
	for( int i = 0; i < 8; )
	 	b & 1<<i++ ? std::cout << 1 : std::cout << 0;
	std::cout << std::endl;
}

void mostrar_secuencia_bits( trama_t t , int num_bytes )
{
	for( int size_cad = num_bytes, i = 0, pos_cad = 0; pos_cad < size_cad; i = i == 7 ? ( std::cout << ' ' , pos_cad++ ), 0 : i+1 )
		t[pos_cad] & 1<<i ? std::cout << 1 : std::cout << 0;
	std::cout << std::endl;
}


void extraer_mensaje( trama_t src , int *num_bytes )
{
	src[*num_bytes-4] = 0;
}


