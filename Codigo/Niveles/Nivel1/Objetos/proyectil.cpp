#include "proyectil.h"
#include <QPixmap>
#include <QDebug>

Proyectil::Proyectil(QWidget* parent, const QVector2D& posicion, const QVector2D& direccion, const QString& tipo)
    : Objeto(parent, posicion), direccion(direccion.normalized()), velocidad(6.0f), tipo(tipo)
{
    QPixmap imagen;

    if (tipo == "roshi") {
        imagen.load(":/Sprites/Proyectiles/Bola_Energia.png");
        sprite->setFixedSize(100, 100);  // Tamaño para la bola de energía
    }
    else if (tipo == "lunch") {
        if (direccion.x() > 0) {
            imagen.load(":/Sprites/Proyectiles/R_BalaCohete.png");
        } else {
            imagen.load(":/Sprites/Proyectiles/L_BalaCohete.png");
        }
        sprite->setFixedSize(100, 50);  // Tamaño para el cohete
    }

    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del proyectil de tipo:" << tipo;
    } else {
        sprite->setPixmap(imagen.scaled(sprite->size()));
        sprite->show();
    }
}

void Proyectil::actualizar()
{
    // Movimiento lineal del proyectil
    posicion += direccion * velocidad;
    sprite->move(static_cast<int>(posicion.x()), static_cast<int>(posicion.y()));

    // Verificar si el proyectil se sale de la pantalla
    QWidget* escena = dynamic_cast<QWidget*>(sprite->parent());
    if (escena) {
        int limitePantalla = escena->width();

        if (posicion.x() > limitePantalla || posicion.x() < -sprite->width()) {
            sprite->hide(); // Ocultar proyectil si sale de la escena
        }
    }
}

void Proyectil::interactuar(Entidad* entidad)
{
    if (!entidad) return;

    // Aquí puedes implementar lógica futura como quitar vida
    qDebug() << "Proyectil de tipo" << tipo << "impactó una entidad.";

    sprite->hide(); // Desaparecer proyectil tras impacto
}
