#include <QApplication>
//#include <cstdio> <- Descomentar esta linea al finalizar el desarrollo y Debbuging

#include "mainWindow.h"


int main(int argc, char *argv[]) {

    // Silenciar los mensajes de stderr de FFmpeg
    //freopen("NUL", "w", stderr); // <- Descoementar esta linea al finalizar el desarrollo y Debugging // Solo en Windows

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
