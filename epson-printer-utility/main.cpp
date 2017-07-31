#include "epsonprinterutility.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	epsonprinterutility w;
	w.show();
	return a.exec();
}
