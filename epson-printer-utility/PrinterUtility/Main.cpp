#include <QApplication>
#include "EPUGlobal.h"
#include <QTranslator>
#include <QLocale>

int main(int argc, char** argv)
{

    QApplication app(argc, argv);

    app.setApplicationName("EpsonPrinterUtility");

    QTranslator translator;


    QString locale;
    locale = setlocale(LC_CTYPE, NULL);

    QString current_langfile = "/opt/epson-printer-utility/resource/Languages/";

//    QString current_langfile = "Languages/";   //    For Debug


    current_langfile += "utilitytr_";
//    current_langfile += QLocale::system().name();
    current_langfile += locale;

//	printf("%s \n", qPrintable(current_langfile));


    translator.load(current_langfile);
    app.installTranslator(&translator);



    EPUMainController mainController;
    SetMainController(&mainController);
    mainController.ShowMainWindow();

    return app.exec();

}

