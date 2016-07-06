#include "ventana.h"
#include "ui_ventana.h"

ventana::ventana(QWidget *parent) : QMainWindow(), ui(new Ui::ventana)
{
	parent = 0;
	system("clear");
	cout << "Iniciando WAVE... " << endl;
	t = new transceptor();
    leer_configuracion();
    tok = NADIE;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(actualizar_mensaje()));
    timer->start(200);
	ui->setupUi(this);
	ui->mensaje->setFocus();
	ui->mensaje->installEventFilter(this);
}

ventana::~ventana()
{
    delete ui;
}

void ventana::actualizar_mensaje()
{
	if( t->mensaje_listo )
	{
		t->mensaje_listo = false;
		if ( tok == NADIE )
			tok = USUARIO;
		if ( tok == USUARIO )
		{
			ui->conversacion->appendPlainText( QString("\nContacto dice:") );
			tok = CONTACTO;
		}
		ui->conversacion->appendPlainText( QString::fromUtf8( t->mensaje.c_str() ));
	}
}

void ventana::on_boton_enviar_clicked()
{
	if (!ui->mensaje->toPlainText().isEmpty())
	{
		QByteArray aEnviar = ui->mensaje->toPlainText().toUtf8();
		if ( tok == NADIE )
			tok = CONTACTO;
		if ( tok == CONTACTO )
		{
			ui->conversacion->appendPlainText((QString)"\nUsuario dice: ");
			tok = USUARIO;
		}
		ui->conversacion->appendPlainText(ui->mensaje->toPlainText());
		ui->mensaje->clear();
		t->enviar_mensaje( strdup( aEnviar.data() ), strlen( aEnviar.data() ) );
	}
}

void ventana::on_boton_salir_clicked()
{
    exit(EXIT_SUCCESS);
}

void ventana::on_mensaje_textChanged()
{
	if ( ui->mensaje->toPlainText().length() > long_mensaje )
		ui->mensaje->textCursor().deletePreviousChar();
}

bool ventana::eventFilter(QObject *obj, QEvent *event)
{
	if ( obj == ui->mensaje )
		if ( event->type() == QEvent::KeyPress )
			switch (((QKeyEvent *)event)->key())
			{
				case Qt::Key_Return :
					on_boton_enviar_clicked();
					return true;
				break;

				case Qt::Key_Tab :
					ui->boton_enviar->setFocus();
					return true;
			}
	return QMainWindow::eventFilter(obj, event);
}

void ventana::leer_configuracion()
{
	ifstream conf("wave.conf");
	int i;
	string linea;
	char *valores;
	cout << "Archivo de configuracion... ";
	if ( conf.good() )
	{
		for ( i = 0; !conf.eof(); )
		{
			getline( conf, linea );
			if ( linea[0] == '#' )
				continue;
			else
			{
				valores = strtok(strdup(linea.c_str()), " =");
				valores = strtok(NULL, " =");				
				switch( i++ )
				{
					case LONG_MENSAJE:
						long_mensaje = atoi(valores);
					break;
					case VOLUME:
						t->ajustar_volume( valores );
				}
			}
		}
		cout << "OK" << endl;
		conf.close();
	}
	else
		cout << "FAIL" << endl;
}
