#include "nivel1.h"
#include "mapa.h"
#include <QKeyEvent>

// Constructor: inicializa el nivel y configura el temporizador principal
Nivel1::Nivel1(QWidget* parent) : Nivel(parent) {
    jugador = nullptr;
    timer = new QTimer(this);

    // Habilita la captura de eventos del teclado en esta vista
    setFocusPolicy(Qt::StrongFocus);
}

// Configura los elementos del nivel al cargarlo
void Nivel1::cargarNivel() {
    // Fondo del nivel (Kame House + playa)
    mapa = new Mapa(":/Sprites/Fondos/fondo_nivel1.jpg", this);
    mapa->setGeometry(0, 0, 800, 600); // tamaño de la ventana
    mapa->show();

    // Crear al jugador (Maestro Roshi)
    jugador = new Roshi(this);
    jugador->getSprite()->raise(); // Mostrar por encima del fondo

    // Timer principal del bucle del juego (~60 FPS = cada 16ms)
    connect(timer, &QTimer::timeout, this, [=]() {
        // Movimiento horizontal
        if (teclasPresionadas.contains(Qt::Key_Left)) {
            jugador->moverIzquierda();
        } else if (teclasPresionadas.contains(Qt::Key_Right)) {
            jugador->moverDerecha();
        } else {
            // Si no se está presionando ninguna tecla de movimiento, detener desplazamiento horizontal
            jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
        }

        // Aplicar gravedad y actualizar la posición en pantalla
        jugador->aplicarFisica();
        jugador->actualizar();
    });

    timer->start(16); // 60 FPS
    setFocus();       // Asegura captura de teclas
}

// Evento: se presiona una tecla
void Nivel1::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());

    // Salto si se presiona flecha arriba
    if (event->key() == Qt::Key_Up && jugador) {
        jugador->saltar();
    }

    // Ataque si se presiona barra espaciadora
    if (event->key() == Qt::Key_Space && jugador) {
        jugador->atacar();
    }

    if (event->key() == Qt::Key_R && jugador) {
        jugador->lanzarEnergia();
    }
}

// Evento: se suelta una tecla
void Nivel1::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}
