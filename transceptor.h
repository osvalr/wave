#ifndef TRANSCEPTOR_H
#define TRANSCEPTOR_H


//librerias necesarias para el programa
#include <pthread.h>		// Manejo de hilos POSIX
#include <iostream>			// Flujos de entrada y salida
#include <cstdlib>	 		// Funciones estandares
#include <cstring>			// Funciones para el manejo de cadenas
#include <alsa/asoundlib.h>	// Funciones y estructuras de sonido de ALSA
#include <cmath>			// Funciones matemáticas
#include <limits.h>			// Definiciones de valores mínimos y máximos.
#include <stdint.h>         // Definiciones de enteros
#include <fftw3.h>
#include "opstrama.h"

using namespace std;


typedef enum 
{
	ERROR = -1,
	RECIBIDO,
	INICIA,
	RECIBE,
	RECIBE_ARCHIVO,
	DETENIDO,
	FIN_TRM,
	INI_TRM
} estado_receptor;

typedef enum
{
	ARCHIVO,
	MENSAJE
} tipo_transmision;


//atributos de clase transceptor para la modulación
class transceptor
{
	public:
	
//atributos de clase transceptor para la modulación
	
		//modulador( desmodulador * );                                   // Constructor
		//~modulador();                                  // Destructor
		transceptor();
		~transceptor();
		int modulador();                     // Apertura el dispositivo
		int iniciar_dispositivo_m();                     // Apertura el dispositivo
		static void *generar_frecuencias( void *p );   // Genera frecuencias a lo largo de la ejecución del programa
		static void *modular_caracteres( void *p );    // Modula los caracteres de un mensaje
		void enviar_mensaje( char *s, int n );         // Ejecuta la modulacion de caracteres
		void enviar_senal( int );                      // Envia una señal {ACK, NAK, ..revisar opstrama.h}
		void detener_transmision();                    // Ajusta la frecuencia para que sea tomada por detenida
		void ajustar_volume( string volume );          // Ajusta el volumen por el mezclador de ALSA
		int porcentaje_volume;                         // Porcentaje volume
		int rate,rate_exacto;                          // Tasa de muestreo
		pthread_t thread_m;                            // Hilos que se encarga de la generacion y modulacion
		uint8_t *buffer_m;                           // Buffer de valores de 8bits
		double faseMax;                                // Dimension maxima del periodo
		float porcentaje;                              // Porcentaje del volumen
		snd_pcm_t           *pcm_handle_m;             // Estructuras ALSA
		snd_pcm_hw_params_t *hwparams_m;
		snd_pcm_sw_params_t *swparams_m;
		
		double step;
		uint8_t *cadena;
		int long_cadena;
		
		//bool mensaje_correcto;

//atributos de clase transceptor para la desmodulación

		
		char* buffer_d;	// array de datos tipo caracter
		int cantidad_muestras,frecuencia;
		string mensaje;
		trama_t trm;
		pthread_t thread_d; 
		estado_receptor e;
		tipo_transmision t;
		// Datos de audio del dispositivo	
		bool mensaje_listo;
		bool mensaje_correcto;	
		bool trama_lista;
		snd_pcm_t *pcm_handle_d;  // Controlador para el  dispositivo PCM 
		
		//Fourier(fftw3) 
		fftw_complex	*amplitudes, *fft_result;
		fftw_plan	plan_forward;

		/* Estructura que contiene informacion acerca del  
		hardware y de como puede ser usado para especificar la     
		configuracion que se utilizara para el flujo PCM . */ 
		snd_pcm_hw_params_t *hwparams_d;		
		static void *captura( void *p );
		int iniciar_dispositivo_d();
		bool es_valida( int );
		int index_trm;
//Atributos de transceptor
	
		int dimension_buffer, dimension_exacta_buffer; // Cantidad de valores en el buffer
		int periodos;                                  // cantidad de periodos en el buffer
		snd_pcm_uframes_t dimension_periodo; 	       // Tamanio del periodo (muestras)                                
		int bytes_por_muestra;//cantidad bytes por frame
};

#endif 
