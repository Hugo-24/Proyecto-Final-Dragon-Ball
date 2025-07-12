#include "corazon.h"
#include "nivel.h"
#include <QTimer>
#include <QDebug>

/**
 * Constructor del Corazón curativo.
 * Crea el sprite, lo posiciona y arranca un temporizador para expirar tras 10 segundos.
 */
Corazon::Corazon(QWidget* parent, const QVector2D& posicion, Nivel* nivel)
    : Objeto(parent, posicion), nivel(nivel)
{
    // Crear y mostrar sprite
    QPixmap pix(":/Sprites/UI/corazon_lleno.png");
    sprite->setPixmap(pix.scaled(32, 32)); // Escala el sprite a tamaño fijo
    sprite->setFixedSize(32, 32);
    sprite->move(posicion.x(), posicion.y()); // Posiciona visualmente
    sprite->show();

    // Crear temporizador para expirar después de 10 segundos (sin usar "this" como padre)
    timerDesaparicion = new QTimer(parent); // Se asocia al contenedor visual del juego
    timerDesaparicion->setSingleShot(true); // Se dispara solo una vez

    // Programar lambda de expiración (Nivel1 verificará expirado)
    QObject::connect(timerDesaparicion, &QTimer::timeout, [=]() {
        expirado = true; // Marca para que el nivel lo elimine aunque no sea recogido
    });

    timerDesaparicion->start(10000); // 10 segundos
}

/**
 * Destructor del Corazón: limpia el temporizador.
 */
Corazon::~Corazon() {
    if (timerDesaparicion) {
        timerDesaparicion->stop();
        timerDesaparicion->deleteLater();
        timerDesaparicion = nullptr;
    }

    if (sprite) {
        sprite->deleteLater();
        sprite = nullptr;
    }
}

/**
 * Marca el corazón como recogido al colisionar con el jugador.
 */
void Corazon::interactuar(Entidad* /*otra*/) {
    recogido = true; // No se destruye directamente: Nivel1 lo elimina
}

/**
 * Desplaza el sprite visual con el scroll.
 */
void Corazon::moverConScroll(float offsetX) {
    if (sprite) {
        sprite->move(posicion.x() - offsetX, posicion.y());
    }
}

/**
 * Devuelve si el corazón ya fue recogido por el jugador.
 */
bool Corazon::estaRecogido() const {
    return recogido;
}

/**
 * Devuelve si el corazón debe desaparecer por tiempo.
 */
bool Corazon::debeDesaparecer() const {
    return expirado;
}
