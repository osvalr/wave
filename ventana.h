#ifndef VENTANA_H
#define VENTANA_H

#include <QMainWindow>
#include <QEventLoop>
#include <QKeyEvent>
#include <QVariant>
#include <QString>
#include <QTimer>
#include <QEvent>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <string>
#include "transceptor.h"

namespace Ui 
{
    class ventana;
}

typedef enum
{
	NADIE = -1,
	USUARIO,
	CONTACTO
} token; //token
enum
{
	LONG_MENSAJE,
	VOLUME
}; //opciones de wave.conf

class ventana : public QMainWindow
{
    Q_OBJECT

	public:
		ventana(QWidget *parent = 0);      // Constructor
		~ventana();                        // Destructor
		transceptor *t;
		int long_mensaje;                  // Longitud maxima del mensaje a enviar
		QString max_frecuencia;            // 
		QTimer *timer;                     // timer para la actualizacion
		token tok;
	protected:
		bool eventFilter(QObject *obj, QEvent *event);

	private:
		Ui::ventana *ui;
		void leer_configuracion();

	public slots:
		void actualizar_mensaje();

	private slots:
		void on_boton_salir_clicked();
		void on_boton_enviar_clicked();
		void on_mensaje_textChanged();
};

#endif // VENTANA_H
