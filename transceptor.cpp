#include "transceptor.h"
#include "opstrama.h"
transceptor::transceptor( )
{
//Inicialización atributos generales 
	bytes_por_muestra = 1;//bytes por muestra  1, 8 bits
	dimension_periodo = 2208;
	periodos = 2;
	dimension_buffer=dimension_exacta_buffer = (dimension_periodo*periodos);
	rate = 44100;
	
//Inicialización atributos modulador
	step = 1 / (double) rate; // dt
	faseMax = 0.0; // Calcula el maximo valor de la fase
	if( iniciar_dispositivo_m() < 0 )
		exit(EXIT_FAILURE);
	buffer_m = (uint8_t *) malloc( periodos * dimension_periodo * sizeof (uint8_t) ); // Reserva memoria para el buffer
	if( buffer_m == NULL ) // si no se pudo reservar el espacio necesario
		exit(EXIT_FAILURE);	

//Inicialización atributos desmodulador
	cantidad_muestras = 2205;//Tamaño del vector de buffer	
	buffer_d = new char[cantidad_muestras * bytes_por_muestra];//se alamceana las lecturas del dispositivo de entrada
	/*Definición de las variables para fftw3*/
	amplitudes = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * cantidad_muestras ); //Definición del vector de amplitudes usado por fftw3
	fft_result  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * cantidad_muestras );//Definición del vector de resultados generados por fftw3
	plan_forward  = fftw_plan_dft_1d( cantidad_muestras, amplitudes, fft_result, FFTW_FORWARD, FFTW_ESTIMATE );//Definición del plan de la fftw3 para 																													//aplicar la transformada de fourier
	mensaje_listo = false;
	if( iniciar_dispositivo_d() < 0 )
		exit(EXIT_FAILURE);
	pthread_create(&thread_d, NULL, &captura, this); //Se inicia el hilo de Captura de Audio	
}

void transceptor::ajustar_volume(string volume)
{
	system(("amixer set Master " + volume + "% unmute >> /dev/null").c_str()); //asegura que el volumen especificado esté al 100% y que no esté en silencio
	system(("amixer set PCM " + volume + "% unmute >> /dev/null").c_str());
}


transceptor::~transceptor()
{
	//libera el buffer del dispositivo modulador
	snd_pcm_drop(pcm_handle_m);
	//libera el buffer del dispositivo desmodulador
	snd_pcm_drop(pcm_handle_d);
	//libera las variables 
	fftw_destroy_plan( plan_forward );
	fftw_free( amplitudes );
	fftw_free( fft_result);
}

//Funciones y Procedimientos del modulador
	
void transceptor::enviar_mensaje( char *s, int n )
{
	long_cadena = n;
	cadena = ( uint8_t * ) malloc( (n + 1) * sizeof(uint8_t) );
	strcpy( ( char * ) cadena, s );
	pthread_create(&thread_m, NULL, &generar_frecuencias, this);
}


void *transceptor::generar_frecuencias( void *p )
{
	transceptor *t = reinterpret_cast<transceptor *>(p);//Hace cast al puntero para tener acceso a la clase
	int cptr, i, j, k,l, n;
	double fase;
	unsigned char *ptr;
	int err;
	trama_t mensaje;
	snd_pcm_prepare(t->pcm_handle_m);
	t->enviar_senal(INI_T); // Se envia la señal de inicio de transmision
	for ( i = 0; i < t->long_cadena; i += (t->long_cadena-i)/t->long_cadena? PAYLOAD : (t->long_cadena-i)%PAYLOAD )
	{
		n = (t->long_cadena-i)/PAYLOAD? PAYLOAD : (t->long_cadena-i)%PAYLOAD;
		strncpy((char*)mensaje, (char*)t->cadena+i , n);
		crc_32(mensaje, n);
		t->mensaje_correcto = false;
		t->enviar_senal(BANDERA_INI); // Se envia la señal de inicio de trama
		for ( k = 0; k < n+4; )
		{
			for ( l = 0; l < t->periodos && k < n+4; l++, k++) //Genera y calcula los valores de las muestras para cada tipo de funcion, segun haya elegido el usuario	
			{
				t->faseMax = 1 / ((double) mensaje[k]*20 + 1000);
				for ( fase = 0, j = 0; j < t->dimension_periodo; j++ )
				{
					t->buffer_m[j] = sin( ( fase * 2 * M_PI )/ t->faseMax - M_PI ) * CHAR_MAX;
					fase += t->step; // Incrementa la posicion de la muestra a tomar
					if (fase >= t->faseMax) // si supera la longitud del período
						fase -= t->faseMax;
				}
				cptr = t->dimension_periodo;
				ptr = t->buffer_m;
				while( cptr > 0 )
				{
					err = snd_pcm_writei(t->pcm_handle_m, ptr, cptr);
					ptr += snd_pcm_frames_to_bytes(t->pcm_handle_m, err);
					cptr -= err;
				}
			}
		}
		t->enviar_senal(BANDERA_FIN); // Se envia la señal de inicio de trama		
	}
	t->enviar_senal(FIN_T); // Se envia la señal de fin de transmision
	free(t->cadena);
	return NULL;
}


void transceptor::enviar_senal( int SENAL )
{
	int cptr, err, j, i;
	unsigned char *ptr;
	double fase;
	faseMax = 1 / ((double) SENAL);
	for ( fase = 0, j = 0; j < dimension_periodo; j++ )
	{
		buffer_m[j] = sin( ( fase * 2 * M_PI )/ faseMax - M_PI ) * CHAR_MAX;
		fase += step; // Incrementa la posicion de la muestra a tomar
		if (fase >= faseMax) // si supera la longitud del período
			fase -= faseMax;
	}
	cptr = dimension_periodo;
	ptr = buffer_m;
	while( cptr > 0 )
	{
		err = snd_pcm_writei(pcm_handle_m, ptr, cptr);
		ptr += snd_pcm_frames_to_bytes(pcm_handle_m, err);
		cptr -= err;
	}
}


// Inicia el dispositivo con los parametros necesarios para la modulación
int transceptor::iniciar_dispositivo_m()
{
	snd_pcm_hw_params_alloca(&hwparams_m); // Ubica los parametros del dispositivo en memoria
	snd_pcm_sw_params_alloca(&swparams_m);

	//Abrir PCM (Codigo de pulso modulado),  El último parámetro de 
	//esta función es el modo. Si esto se establece en 0, el modo estándar es utilizado. 
	//Otros posibles valores son SND_PCM_NONBLOCK y SND_PCM_ASYNC. Si SND_PCM_NONBLOCK
	//acceden a leer y escribir en el dispositivo PCM.
	//Si SND_PCM_ASYNC se especifica, SIGIO se emitirá cada vez que un
	//período ha sido completamente procesados por la tarjeta de sonido. 
	if( snd_pcm_open(&pcm_handle_m, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, 0) < 0 )
	{
		cerr << "Error al abrir el dispositivo PCM " << endl;
		return(-1);
	}

	//Inicializamos hwparams con el espacio de configuración completa
	if( snd_pcm_hw_params_any(pcm_handle_m, hwparams_m) < 0 )
	{
		cerr << "No puede ser configurado este dispositivo PCM" << endl;
		return(-1);
	}

	//Establecer el tipo de acceso. Esto puede ser 
	//SND_PCM_ACCESS_RW_INTERLEAVED o SND_PCM_ACCESS_RW_NONINTERLEAVED.
	if( snd_pcm_hw_params_set_access(pcm_handle_m, hwparams_m, SND_PCM_ACCESS_RW_INTERLEAVED) < 0 )
	{
		cerr << "Error estableciendo el acceso." << endl;
		return(-1);
	}

	// Establecer formato de muestra
	if( snd_pcm_hw_params_set_format(pcm_handle_m, hwparams_m, SND_PCM_FORMAT_S8) < 0 )
	{
		cerr << "Error estableciendo el formato." << endl;
		return(-1);
	}

	// Establecer la frecuencia de muestreo. Si la frecuencia exacta no es soportada
	// por el hardware, se usara la frecuencia mas cercana posible.
	rate_exacto = rate;
	
	if( snd_pcm_hw_params_set_rate_near(pcm_handle_m, hwparams_m, (uint*)&rate_exacto, 0) < 0 )
	{
		cerr << "Error configurando la frecuencia." << endl;
		return -1;
	}
	
	if (rate != rate_exacto)
		cerr << "La frecuencia " << rate << "Hz no es soportada por tu hadware."<< endl << " ==> En su lugar usa " << rate_exacto << " Hz" << endl;

	// Establecer numero de canales
	if( snd_pcm_hw_params_set_channels(pcm_handle_m, hwparams_m, 1) < 0 )
	{
		cerr << "Error estableciendo los canales." << endl;
		return -1;
	}

	//Establecer el número de períodos. Periodos usados son llamados fragmentos.
	if( snd_pcm_hw_params_set_periods(pcm_handle_m, hwparams_m, periodos, 0) < 0 )
	{
		cerr << "Error estableciendo los periodos." << endl;
		return -1;
	}
	
	// Establecer el tamaño del búfer (en marcos). La latencia resultante está dada por
	// latency = periodsize * periods / (rate * bytes_per_frame)
	if( snd_pcm_hw_params_set_buffer_size_near(pcm_handle_m, hwparams_m, (snd_pcm_uframes_t*)&dimension_exacta_buffer) < 0 )
	{
		cerr << "Error estableciendo el tamanio del buffer." << endl;
		return -1;
	}
	
	if( dimension_buffer != dimension_exacta_buffer )
		cerr << "El tamanio del Buffer "<< dimension_buffer << " no es soportado, En su lugar usar " << dimension_exacta_buffer << endl;
	
	//Ajusta parametros de HW al dispositivo PCM y prepara el dispositivo
	if( snd_pcm_hw_params(pcm_handle_m, hwparams_m) < 0 )
	{
		cerr << "Error estableciendo los parametros de HW." << endl;
		return -1;
	}
	
	return 0;
}


//Funciones y Procedimientos del Desmodulador

bool transceptor::es_valida( int f )
{
	return (f >=1000 && f <= 6200) || (f >= 500 && f <= 600) ;
}

void *transceptor::captura( void *p ) //Hilo Captura de Audio
{
	transceptor *t = ( transceptor * ) p;
	int i,n, valor;
	int max_amp,index_frecuencia;
	t->e = DETENIDO;
	while( true )
	{
		
			while((n = snd_pcm_readi(t->pcm_handle_d, t->buffer_d, t->cantidad_muestras*t->bytes_por_muestra)) < 0 )
				snd_pcm_prepare(t->pcm_handle_d);	
			for( i = 0; i < t->cantidad_muestras; i++ )
			{
				t->amplitudes[i][0]= (float)t->buffer_d[i];
				t->amplitudes[i][1]=0.0;
			}
			fftw_execute( t->plan_forward );//Transformada Rapida de Fourier
			max_amp=valor=0;
			for( i = 1 ; i < t->cantidad_muestras ; i++ )
			{
				valor =  sqrt(t->fft_result[i][0]*t->fft_result[i][0] + t->fft_result[i][1]*t->fft_result[i][1]) ;
				if(valor>max_amp)
				{
					max_amp=valor;
					index_frecuencia=i;
				}
			}
			valor = t->frecuencia = (int) (index_frecuencia*t->rate)/(t->cantidad_muestras);
		if ( t->es_valida( valor ) )
		{
			switch ( valor )
			{
				case ACK:
					cout << " ACK RECIBIDO " << endl;
//					t->m->mensaje_correcto = true;
				break;

				case NAK:
					cout << " NAK RECIBIDO " << endl;
					t->e = RECIBE;

				break;

				case INI_T:
					if ( t->e == DETENIDO )
					{
						t->t = MENSAJE;
						t->e = RECIBE;
						t->mensaje.clear();
						t->index_trm = 0;
						t->mensaje_listo = false;
						
					}
				break;

				case FIN_T:
					if ( t->e == RECIBIDO )
					{
						t->e = DETENIDO;
						if ( t->t == MENSAJE )
						{
                                                       
							t->mensaje_listo = true;
							
						}
						else //ARCHIVO
						{}
					}
				break;

				case INI_T_F:
					if ( t->e == DETENIDO )
					{
						t->t = ARCHIVO;
						t->e = RECIBE;
					}
				break;		

				case BANDERA_INI:
					if ( t->e == RECIBE )
					{
						if ( t->t == MENSAJE )
						{
							for ( i = 0; i < PAYLOAD + CRC_BYTES; ++i )
								t->trm[i] = 0;
							t->index_trm = 0;
							cout << "BANDERA DE INICIO" <<endl;
						}
						else //ARCHIVO
						{}
					}
				break;

				case BANDERA_FIN:
					if ( t->e == RECIBE )
					{
						if ( t->t == MENSAJE )
						{
							if ( !crc_division( t->trm, t->index_trm ) )
							{
								cout << "Correcto" << endl;
								t->e = RECIBIDO;
								snd_pcm_prepare(t->pcm_handle_m);
								t->enviar_senal(ACK);
								extraer_mensaje( t->trm , &(t->index_trm) );
								t->mensaje += strdup((char *)t->trm);
							}
							else
							{
								cout << "Incorrecto!" << endl;
								t->e = RECIBE;

								for ( i = 0; i < PAYLOAD + CRC_BYTES; i++ )
									t->trm[i] = 0;
								t->index_trm = 0;
								snd_pcm_prepare(t->pcm_handle_m);
								t->enviar_senal(NAK);
							}
						}
						else //ARCHIVO
						{}
					}
				break;

				default:
					if ( t->e == RECIBE )
						if ( t->t == MENSAJE )
						{
							t->trm[t->index_trm++] = ( valor - 1000 ) / 20;
						}
						else //ARCHIVO
						{}
			}
		}
	}
	return NULL;
}

// Inicia el dispositivo con los parametros necesarios para la desmodulación
int transceptor::iniciar_dispositivo_d()
{
	snd_pcm_hw_params_alloca(&hwparams_d);// Asignar la estructura snd_pcm_hw_params_t en la pila

	//Abrir PCM (Codigo de pulso modulado),  El último parámetro de 
	//esta función es el modo. Si esto se establece en 0, el modo estándar es utilizado. 
	//Otros posibles valores son SND_PCM_NONBLOCK y SND_PCM_ASYNC. Si SND_PCM_NONBLOCK
	//acceden a leer y escribir en el dispositivo PCM.
	//Si SND_PCM_ASYNC se especifica, SIGIO se emitirá cada vez que un
	//período ha sido completamente procesados por la tarjeta de sonido. 
	if( snd_pcm_open(&pcm_handle_d, "plughw:0,0", SND_PCM_STREAM_CAPTURE, 0) < 0 )
	{
		cerr << "Error al abrir el dispositivo PCM " << endl;
		cout << "<<<<<"<< endl;
		return(-1);
	}

	//Inicializamos hwparams con el espacio de configuración completa
	if( snd_pcm_hw_params_any(pcm_handle_d, hwparams_d) < 0 )
	{
		cerr << "No puede ser configurado este dispositivo PCM" << endl;
		return(-1);
	}

	//Establecer el tipo de acceso. Esto puede ser 
	//SND_PCM_ACCESS_RW_INTERLEAVED o SND_PCM_ACCESS_RW_NONINTERLEAVED.
	if( snd_pcm_hw_params_set_access(pcm_handle_d, hwparams_d, SND_PCM_ACCESS_RW_INTERLEAVED) < 0 )
	{
		cerr << "Error estableciendo el acceso." << endl;
		return(-1);
	}

	// Establecer formato de muestra
	if( snd_pcm_hw_params_set_format(pcm_handle_d, hwparams_d, SND_PCM_FORMAT_S8) < 0 )
	{
		cerr << "Error estableciendo el formato." << endl;
		return(-1);
	}

	// Establecer la frecuencia de muestreo. Si la frecuencia exacta no es soportada
	// por el hardware, se usara la frecuencia mas cercana posible.
	rate_exacto = rate;
	
	if( snd_pcm_hw_params_set_rate_near(pcm_handle_d, hwparams_d, (uint*)&rate_exacto, 0) < 0 )
	{
		cerr << "Error configurando la frecuencia." << endl;
		return -1;
	}
	
	if (rate != rate_exacto)
		cerr << "La frecuencia " << rate << "Hz no es soportada por tu hadware."<< endl << " ==> En su lugar usa " << rate_exacto << " Hz" << endl;

	// Establecer numero de canales
	if( snd_pcm_hw_params_set_channels(pcm_handle_d, hwparams_d, 1) < 0 )
	{
		cerr << "Error estableciendo los canales." << endl;
		return -1;
	}

	//Establecer el número de períodos. Periodos usados son llamados fragmentos.
	if( snd_pcm_hw_params_set_periods(pcm_handle_d, hwparams_d, periodos, 0) < 0 )
	{
		cerr << "Error estableciendo los periodos." << endl;
		return -1;
	}
	
	// Establecer el tamaño del búfer (en marcos). La latencia resultante está dada por
	// latency = periodsize * periods / (rate * bytes_per_frame)
	if( snd_pcm_hw_params_set_buffer_size_near(pcm_handle_d, hwparams_d, (snd_pcm_uframes_t*)&dimension_exacta_buffer) < 0 )
	{
		cerr << "Error estableciendo el tamanio del buffer." << endl;
		return -1;
	}
	
	if( dimension_buffer != dimension_exacta_buffer )
		cerr << "El tamanio del Buffer "<< dimension_buffer << " no es soportado, En su lugar usar " << dimension_exacta_buffer << endl;
	
	//Ajusta parametros de HW al dispositivo PCM y prepara el dispositivo
	if( snd_pcm_hw_params(pcm_handle_d, hwparams_d) < 0 )
	{
		cerr << "Error estableciendo los parametros de HW." << endl;
		return -1;
	}
	
	return 0;
}

