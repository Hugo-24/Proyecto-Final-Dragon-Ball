#include "submarinojugador.h"
#include <QDebug>

const float IMPULSO = 0.5f;
const float RESISTENCIA = 0.95f; // desaceleración progresiva
const float LIMITE_VELOCIDAD = 5.0f;

SubmarinoJugador::SubmarinoJugador(QWidget* parent)
    : Entidad(parent) {

    QPixmap imagen(":/Sprites/Nave/Nave_Jugador.png");
    if (imagen.isNull()) {
        qDebug() << "Error: no se pudo cargar el sprite del submarino.";
        return;
    }

    QSize nuevoTamaño(86, 126 );  // ✅ Ajusta según lo que se vea mejor
    sprite->setFixedSize(nuevoTamaño);  // Cambia el tamaño del QLabel
    sprite->setPixmap(imagen.scaled(nuevoTamaño));  // Escala el sprite
    sprite->show();

    setPosicion(QVector2D(100, 300));
}


// Teclas: flechas o WASD
void SubmarinoJugador::procesarEntrada(const QSet<int>& teclasPresionadas) {
    QVector2D impulso(0, 0);

    if (teclasPresionadas.contains(Qt::Key_Up) || teclasPresionadas.contains(Qt::Key_W))
        impulso.setY(impulso.y() - IMPULSO);
    if (teclasPresionadas.contains(Qt::Key_Down) || teclasPresionadas.contains(Qt::Key_S))
        impulso.setY(impulso.y() + IMPULSO);
    if (teclasPresionadas.contains(Qt::Key_Left) || teclasPresionadas.contains(Qt::Key_A))
        impulso.setX(impulso.x() - IMPULSO);
    if (teclasPresionadas.contains(Qt::Key_Right) || teclasPresionadas.contains(Qt::Key_D))
        impulso.setX(impulso.x() + IMPULSO);

    velocidad += impulso;

    // Limitar la velocidad máxima
    if (velocidad.length() > LIMITE_VELOCIDAD)
        velocidad = velocidad.normalized() * LIMITE_VELOCIDAD;
}

void SubmarinoJugador::aplicarFisica() {
    // Aplicar resistencia (como agua)
    velocidad *= RESISTENCIA;
    posicion += velocidad;
}

void SubmarinoJugador::actualizar() {
    QWidget* contenedor = sprite->parentWidget();
    if (!contenedor) return;

    int maxX = contenedor->width() - sprite->width();
    int maxY = contenedor->height() - sprite->height();

    // Limitar los bordes
    if (posicion.x() < 0) posicion.setX(0);
    if (posicion.y() < 0) posicion.setY(0);
    if (posicion.x() > maxX) posicion.setX(maxX);
    if (posicion.y() > maxY) posicion.setY(maxY);

    // Mover el sprite visualmente
    sprite->move(posicion.toPoint());
    qDebug() << "Posición actual:" << posicion;

}

void SubmarinoJugador::recibirDanio(int cantidad) {
    vidaActual -= cantidad;
    if (vidaActual < 0) vidaActual = 0;
    // Aquí puedes agregar lógica para detectar "muerte" y reaccionar
    qDebug() << "Vida actual del jugador:" << vidaActual;
}



