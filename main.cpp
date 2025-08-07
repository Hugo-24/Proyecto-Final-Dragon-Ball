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

/*
 Desarrollado por:
    Emmanuel López Ramírez(Entropy) - [@EmmaNuel-Network](https://github.com/EmmaNuel-Network) correo: entropy.casp9@gmail.com

    Hugo Esteban Barrero García(HGB)- [@Hugo-24](https://github.com/Hugo-24) correo: hugo.barrerog@udea.edu.co
*/
