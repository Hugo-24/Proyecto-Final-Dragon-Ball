#include <QApplication>
#include <cstdio>

#include "mainWindow.h"


int main(int argc, char *argv[]) {

    // Silenciar los mensajes de stderr de FFmpeg
    freopen("NUL", "w", stderr);

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
