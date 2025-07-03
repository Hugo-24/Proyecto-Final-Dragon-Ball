#include "torpedo.h"
#include <QDebug>
#include <cmath>

Torpedo::Torpedo(QWidget* parent, const QVector2D& posicion, const QVector2D& direccion)
    : Objeto(parent, posicion), direccion(direccion), velocidad(7.0f),
    tiempoTotal(0.0f), amplitud(30.0f), frecuencia(2.0f), posicionInicialY(posicion.y()) {

    QPixmap imagen(":/Sprites/Nave/torpedo_jugador.png");
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del torpedo";
    }

    // 👉 Establece primero el tamaño del QLabel (sprite)
    sprite->setFixedSize(30, 12); // Tamaño del contenedor
    sprite->setPixmap(imagen.scaled(sprite->size())); // Ajusta el QPixmap al QLabel
    sprite->show();
}


void Torpedo::actualizar() {
    tiempoTotal += 0.05f;

    if (direccion.x() != 0 && direccion.y() == 0) {
        // Movimiento horizontal + oscilación vertical
        float offsetY = amplitud * std::sin(frecuencia * tiempoTotal);
        posicion.setX(posicion.x() + direccion.normalized().x() * velocidad);
        posicion.setY(posicionInicialY + offsetY);
    } else {
        // Movimiento diagonal o vertical puro
        posicion += direccion.normalized() * velocidad;
    }

    sprite->move(posicion.toPoint());

    // Borrado si sale de escena
    QWidget* escena = dynamic_cast<QWidget*>(sprite->parent());
    if (escena) {
        if (posicion.x() < -sprite->width() || posicion.x() > escena->width() ||
            posicion.y() < -sprite->height() || posicion.y() > escena->height()) {
            sprite->hide();
        }
    }
}





void Torpedo::interactuar(Entidad* entidad) {
    if (!entidad) return;

    // Ejemplo: aplicar daño si es un enemigo
    qDebug() << "Torpedo colisionó con una entidad.";
    sprite->hide(); // O eliminarlo del escenario
}
