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
    sprite->setFixedSize(26, 10); // Tamaño del contenedor
    sprite->setPixmap(imagen.scaled(sprite->size())); // Ajusta el QPixmap al QLabel
    sprite->show();
}


void Torpedo::actualizar() {
    // Movimiento horizontal constante
    posicion.setX(posicion.x() + direccion.normalized().x() * velocidad);

    // Movimiento vertical senoidal (basado en tiempo, no en X)
    tiempoTotal += 0.05f;  // Puedes ajustar este valor para controlar frecuencia
    float offsetY = amplitud * std::sin(frecuencia * tiempoTotal);
    posicion.setY(posicionInicialY + offsetY);

    sprite->move(posicion.toPoint());

    // Lógica para eliminar si sale de la pantalla
    QWidget* escena = dynamic_cast<QWidget*>(sprite->parent());
    if (escena) {
        if (posicion.x() > escena->width() || posicion.x() < -sprite->width()) {
            sprite->hide(); // Desaparece al salir de la escena
        }
    }
}





void Torpedo::interactuar(Entidad* entidad) {
    if (!entidad) return;

    // Ejemplo: aplicar daño si es un enemigo
    qDebug() << "Torpedo colisionó con una entidad.";
    sprite->hide(); // O eliminarlo del escenario
}
