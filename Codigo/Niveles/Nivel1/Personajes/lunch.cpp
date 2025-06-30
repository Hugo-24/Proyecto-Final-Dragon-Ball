#include "lunch.h"
#include <QTimer>
#include <QDebug>

Lunch::Lunch(QWidget* parent)
    : Personaje(parent), transformada(false) {
    // Establece sprite inicial (BlueLunch derecha)
    setSprite(":/Sprites/Lunch/R_Idle_BlueLunch.png");

    // Posición inicial
    setPosicion(QVector2D(100, 450));
    sprite->show();
    sprite->raise();
}

void Lunch::actualizarSprite() {
    QString ruta;
    if (transformada) {
        ruta = mirandoDerecha
                   ? ":/Sprites/Lunch/R_Idle_Launch.png"
                   : ":/Sprites/Lunch/L_Idle_Launch.png";
    } else {
        ruta = mirandoDerecha
                   ? ":/Sprites/Lunch/R_Idle_BlueLunch.png"
                   : ":/Sprites/Lunch/L_Idle_BlueLunch.png";
    }

    setSprite(ruta);
}

void Lunch::transformar() {
    transformada = !transformada;
    qDebug() << "Lunch se transforma: " << (transformada ? "Modo agresiva" : "Modo tranquila");
    actualizarSprite();
}

void Lunch::atacar() {
    if (!transformada) return;

    QString rutaGolpe = mirandoDerecha
                            ? ":/Sprites/Lunch/R_Punching_Launch.png"
                            : ":/Sprites/Lunch/L_Punching_Launch.png";

    setSprite(rutaGolpe);

    QLabel* spriteLabel = getSprite();
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Lunch/R_Idle_Launch.png"
                           : ":/Sprites/Lunch/L_Idle_Launch.png";

    QTimer::singleShot(350, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });
}

void Lunch::disparar() {
    if (!transformada) return;

    QLabel* spriteLabel = getSprite();

    QStringList frames = mirandoDerecha
                             ? QStringList {
                                   ":/Sprites/Lunch/R_Bazooka1_Launch.png",
                                   ":/Sprites/Lunch/R_Bazooka2_Launch.png",
                                   ":/Sprites/Lunch/R_Bazooka3_Launch.png",
                                   ":/Sprites/Lunch/R_Bazooka4_Launch.png"
                               }
                             : QStringList {
                                   ":/Sprites/Lunch/L_Bazooka1_Launch.png",
                                   ":/Sprites/Lunch/L_Bazooka2_Launch.png",
                                   ":/Sprites/Lunch/L_Bazooka3_Launch.png",
                                   ":/Sprites/Lunch/L_Bazooka4_Launch.png"
                               };

    // Cambia los sprites con un poco de retardo entre cada uno
    int delay = 200; // entre cada frame
    int totalDuracion = delay * frames.size(); // 600 ms

    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(delay * i, this, [spriteLabel, frame = frames[i]]() {
            spriteLabel->setPixmap(QPixmap(frame));
        });
    }

    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Lunch/R_Idle_Launch.png"
                           : ":/Sprites/Lunch/L_Idle_Launch.png";

    // Cambiar a idle después de que termine la animación
    QTimer::singleShot(totalDuracion + 50, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });

    // Más adelante aquí puedes crear el objeto proyectil y lanzarlo
}
