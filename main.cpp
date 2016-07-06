#include "ventana.h"
#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	ventana w;

	w.show();
	return app.exec();
}
