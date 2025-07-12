#include "roshi.h"
#include <QTimer>
#include <QPixmap>
#include <QDebug>

/**
 * Constructor de Roshi.
 * Inicializa su sprite y posición en el nivel.
 */
Roshi::Roshi(QWidget* parent) : Personaje(parent) {
    setSprite(":/Sprites/Roshi/R_Idle_Roshi.png");
    setPosicion(QVector2D(100, 450));
}

/**
 * Destructor.
 */
Roshi::~Roshi() {
    // No requiere liberación manual de recursos
}

/**
 * Actualiza el sprite de Roshi dependiendo de su movimiento y dirección.
 */
void Roshi::actualizarSprite() {
    if (estaMuerto) return;

    static bool alternar = false;
    QString ruta;

    if (getVelocidad().x() != 0) {
        ruta = mirandoDerecha
                   ? (alternar ? ":/Sprites/Roshi/R_Walk1_Roshi.png" : ":/Sprites/Roshi/R_Walk2_Roshi.png")
                   : (alternar ? ":/Sprites/Roshi/L_Walk1_Roshi.png" : ":/Sprites/Roshi/L_Walk2_Roshi.png");
        alternar = !alternar;
    } else {
        ruta = mirandoDerecha
                   ? ":/Sprites/Roshi/R_Idle_Roshi.png"
                   : ":/Sprites/Roshi/L_Idle_Roshi.png";
    }

    setSprite(ruta);
}

/**
 * Ejecuta un ataque cuerpo a cuerpo con animación de puñetazo.
 */
void Roshi::atacar() {
    qDebug() << "Roshi ataca";

    QString rutaGolpe = mirandoDerecha
                            ? ":/Sprites/Roshi/R_Punching_Roshi.png"
                            : ":/Sprites/Roshi/L_Punching_Roshi.png";

    setSprite(rutaGolpe);

    QLabel* spriteLabel = getSprite();
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Roshi/R_Idle_Roshi.png"
                           : ":/Sprites/Roshi/L_Idle_Roshi.png";

    QTimer::singleShot(400, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });
}

/**
 * Ejecuta un ataque de energía con animación de carga/disparo.
 */
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

    QTimer::singleShot(500, this, [spriteLabel, rutaIdle]() {
        spriteLabel->setPixmap(QPixmap(rutaIdle));
    });
}
