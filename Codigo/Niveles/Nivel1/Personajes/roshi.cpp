#include "roshi.h"
#include <QTimer>
#include <QPixmap>
#include <QDebug>

Roshi::Roshi(QWidget* parent) : Personaje(parent) {
    setSprite(":/Sprites/Roshi/R_Idle_Roshi.png");
    setPosicion(QVector2D(100, 450));
}

// Cambia sprite según dirección actual
void Roshi::actualizarSprite() {
     if (estaMuerto) return;
    static bool alternar = false; // alternar entre walk1 y walk2
    QString ruta;

    if (getVelocidad().x() != 0) { // Si se está moviendo
        if (mirandoDerecha) {
            ruta = alternar
                       ? ":/Sprites/Roshi/R_Walk1_Roshi.png"
                       : ":/Sprites/Roshi/R_Walk2_Roshi.png";
        } else {
            ruta = alternar
                       ? ":/Sprites/Roshi/L_Walk1_Roshi.png"
                       : ":/Sprites/Roshi/L_Walk2_Roshi.png";
        }
        alternar = !alternar;
    } else {
        ruta = mirandoDerecha
                   ? ":/Sprites/Roshi/R_Idle_Roshi.png"
                   : ":/Sprites/Roshi/L_Idle_Roshi.png";
    }

    setSprite(ruta);
}

void Roshi::atacar() {
    qDebug() << "Roshi ataca";

    // Cambiar sprite a golpe
    QString rutaGolpe = mirandoDerecha
                            ? ":/Sprites/Roshi/R_Punching_Roshi.png"
                            : ":/Sprites/Roshi/L_Punching_Roshi.png";

    setSprite(rutaGolpe);

    QLabel* spriteLabel = getSprite();
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Roshi/R_Idle_Roshi.png"
                           : ":/Sprites/Roshi/L_Idle_Roshi.png";

    // AUMENTAMOS a 400 ms para que se vea mejor
    QTimer::singleShot(400, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });
}

void Roshi::lanzarEnergia() {
    qDebug() << "Roshi lanza energía";

    QString rutaLanzando = mirandoDerecha
                               ? ":/Sprites/Roshi/R_Launching_Roshi.png"
                               : ":/Sprites/Roshi/L_Launching_Roshi.png";

    setSprite(rutaLanzando);

    QLabel* spriteLabel = getSprite();
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Roshi/R_Idle_Roshi.png"
                           : ":/Sprites/Roshi/L_Idle_Roshi.png";

    // AUMENTAMOS a 500 ms para que se vea mejor el disparo
    QTimer::singleShot(500, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });

}
