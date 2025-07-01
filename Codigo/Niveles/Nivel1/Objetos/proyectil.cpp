#include "proyectil.h"
#include <QPixmap>
#include <QDebug>

Proyectil::Proyectil(QWidget* parent,
                     const QVector2D& posicion,
                     const QVector2D& direccion,
                     const QString& tipo,
                     float velocidad)
    : Objeto(parent, posicion),
    direccion(direccion.normalized()),
    velocidad(velocidad)
{
    QPixmap imagen;

    // Cargar imagen según tipo
    if (tipo == "roshi") {
        imagen.load(":/Sprites/Proyectiles/Bola_Energia.png");
        sprite->setFixedSize(80, 80);
    } else if (tipo == "lunch") {
        if (direccion.x() >= 0)
            imagen.load(":/Sprites/Proyectiles/R_BalaCohete.png");
        else
            imagen.load(":/Sprites/Proyectiles/L_BalaCohete.png");

        sprite->setFixedSize(80, 40);
    }

    // Validar imagen
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del proyectil tipo:" << tipo;
    } else {
        sprite->setPixmap(imagen.scaled(sprite->size()));
    }

    sprite->show();
}

// Actualiza la posición del proyectil
void Proyectil::actualizar() {
    // Movimiento lineal constante
    posicion += direccion * velocidad;
    sprite->move(posicion.toPoint());

    // No se aplica gravedad
    // El proyectil se elimina si sale de la pantalla
    QWidget* escena = dynamic_cast<QWidget*>(sprite->parent());
    if (escena) {
        if (posicion.x() < -sprite->width() || posicion.x() > escena->width() + 8000) {
            sprite->hide();
        }
    }
}

// Acción en colisión (a definir)
void Proyectil::interactuar(Entidad* otra) {
    if (!otra) return;
    qDebug() << "Proyectil impactó con una entidad.";
    sprite->hide(); // Por ahora solo se oculta al impactar
}
