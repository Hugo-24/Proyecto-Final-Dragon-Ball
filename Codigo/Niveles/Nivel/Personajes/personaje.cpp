#include "personaje.h"

Personaje::Personaje(QWidget* parent)
    : Entidad(parent), QObject(parent), mirandoDerecha(true), pasoAlternado(false)
{
    // Se inicializa el temporizador de animación
    animTimer = new QTimer(this);

    // Cada 200 ms, se alterna el paso para cambiar el sprite de caminar
    connect(animTimer, &QTimer::timeout, this, [=]() {
        pasoAlternado = !pasoAlternado;  // Alterna entre paso 1 y 2
        actualizarSprite();              // Actualiza el sprite actual según la dirección y el paso
    });

    animTimer->setInterval(200); // velocidad de cambio de sprite (ms)
}

// Mover a la izquierda
void Personaje::moverIzquierda() {
    QVector2D vel = getVelocidad(); // Obtiene la velocidad actual
    vel.setX(-4);                   // Velocidad negativa en X para moverse a la izquierda
    setVelocidad(vel);             // Aplica la nueva velocidad
    mirandoDerecha = false;       // Marca que el personaje mira a la izquierda
    iniciarAnimacionCaminar();    // Inicia animación de caminar
}

// Mover a la derecha
void Personaje::moverDerecha() {
    QVector2D vel = getVelocidad();
    vel.setX(4);                   // Velocidad positiva en X
    setVelocidad(vel);
    mirandoDerecha = true;
    iniciarAnimacionCaminar();
}

// Saltar (salto hacia arriba)
void Personaje::saltar() {
    if (!enElSuelo) return;       // Solo puede saltar si está en el suelo
    QVector2D vel = getVelocidad();
    vel.setY(-16);                // Velocidad vertical negativa (sube)
    setVelocidad(vel);
    enElSuelo = false;            // Ya no está en el suelo (evita saltos múltiples)
    detenerAnimacionCaminar();    // Opcional: pausa la animación al estar en el aire
}

// Retorna true si el personaje mira a la derecha
bool Personaje::estaMirandoDerecha() const {
    return mirandoDerecha;
}

// Inicia la animación de caminar si no está activa
void Personaje::iniciarAnimacionCaminar() {
    if (!animTimer->isActive())
        animTimer->start();
}

// Detiene la animación de caminar y actualiza el sprite
void Personaje::detenerAnimacionCaminar() {
    animTimer->stop();
    pasoAlternado = false;     // Se reinicia la animación
    actualizarSprite();        // Se actualiza al sprite estático (idle)
}

// Por defecto no hace nada, se redefine en Roshi / Lunch / Goku
void Personaje::actualizarSprite() {
    // En subclases se cargará el sprite correspondiente según dirección y paso
}
