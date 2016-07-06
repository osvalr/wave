#ifndef OPSTRAMA_H
#define OPSTRAMA_H

#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <cstdlib>
#include <cstring>
#define CRC_32 0x104C11DB7LLU   // Definición del Polinomio Generador
#define BIT_33 1LL<<32          // Acceso al bit en la posicion 33
#define BANDERA_INI 480         // Bandera de inicio de trama
#define BANDERA_FIN	500         // Bandera de fin de trama
#define INI_T 6200               // Bandera de Inicio de transmision de msg
#define FIN_T 540               // Bandera de Fin de transmision de msg
#define ACK 560                 // ACK
#define NAK 580                 // NAK
#define INI_T_F 600             // Bandera de inicio de transmision de archivo
#define CRC_BYTES 4             // Cantidad de bytes que ocupará el residuo del CRC en la trama
#define PAYLOAD 256		        // Cantidad de bytes que ocupa el PAYLOAD + CRC + RELLENO + 2 banderas

typedef uint8_t byte_t;
typedef byte_t trama_t[PAYLOAD + CRC_BYTES];

	uint32_t crc_division( trama_t p, int num_bytes);
	void mostrar_byte( byte_t b );
	void crc_32( trama_t p ,int num_bytes);
	void mostrar_secuencia_bits( trama_t a, int num_bytes );
	void extraer_mensaje( trama_t src, int *num_bytes );
	
#endif
