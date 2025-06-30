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
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Roshi/R_Idle_Roshi.png"
                           : ":/Sprites/Roshi/L_Idle_Roshi.png";

    setSprite(rutaIdle);
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

    // Usar versión corta con solo 2 parámetros (sin contexto)
    QTimer::singleShot(250, this, [spriteLabel, rutaIdle]() {
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

    QTimer::singleShot(400, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });

    // Aquí luego se podrá generar la bola de energía como objeto
}
