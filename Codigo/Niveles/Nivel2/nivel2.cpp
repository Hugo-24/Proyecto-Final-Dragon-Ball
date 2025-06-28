#include "Nivel2.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QRectF>
#include <cmath>

#include "objeto.h"
#include "mina.h"
#include "torpedo.h"

// Función auxiliar para detectar colisión por intersección de áreas
static bool colisiona(QVector2D pos1, QSize size1, QVector2D pos2, QSize size2) {
    QRectF r1(pos1.toPoint(), size1);
    QRectF r2(pos2.toPoint(), size2);
    return r1.intersects(r2);
}

Nivel2::Nivel2(QWidget* parent) : Nivel(parent) {
    submarino = nullptr;
    timerActualizacion = new QTimer(this);
    setFocusPolicy(Qt::StrongFocus); // Habilita captura de teclado
}

void Nivel2::cargarNivel() {
    // 🌊 Fondo submarino
    mapa = new Mapa(":/Sprites/Fondos/fondo_nivel2.jpg", this);
    mapa->setGeometry(0, 0, 800, 600);
    mapa->show();

    // 🚢 Submarino del jugador
    submarino = new SubmarinoJugador(this);
    submarino->getSprite()->raise(); // Mostrar sobre el fondo

    // 💣 Objetos hostiles (de prueba por ahora)
    agregarMina(QVector2D(400, 350));
    agregarTorpedo(QVector2D(700, 150), QVector2D(-1, 0)); // torpedo enemigo que va hacia la izquierda

    // 🔁 Timer para actualizar el juego
    connect(timerActualizacion, &QTimer::timeout, this, [=]() {
        // 1. Movimiento del jugador
        submarino->procesarEntrada(teclasPresionadas);
        submarino->aplicarFisica();
        submarino->actualizar();

        // 2. Movimiento de torpedos
        for (int i = 0; i < torpedos.size(); ++i) {
            Torpedo* t = torpedos[i];
            t->actualizar();

            // 🚫 Eliminar si el torpedo sale de la pantalla
            if (t->getPosicion().x() > width() || t->getPosicion().x() < 0) {
                t->getSprite()->hide();
                delete t;
                torpedos.removeAt(i);
                --i;
            }
        }

        // 3. Actualización de objetos (como minas)
        for (Objeto* obj : objetosHostiles) {
            obj->actualizar();
        }

        // 4. Verificar colisiones
        verificarColisiones();
    });

    timerActualizacion->start(16); // Aproximadamente 60 FPS
    this->setFocus(); // Captura del teclado
}


// 🎮 Evento: presionar tecla
void Nivel2::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());

    if (event->key() == Qt::Key_Space && submarino) {
        QVector2D posicionInicial = submarino->getPosicion() + QVector2D(50, 0); // un poco al frente
        QVector2D direccion = QVector2D(1, 0); // hacia la derecha
        agregarTorpedo(posicionInicial, direccion);
    }
}
// 🎮 Evento: soltar tecla
void Nivel2::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}

// 🚨 Colisiones entre submarino y objetos hostiles
void Nivel2::verificarColisiones() {
    for (Objeto* obj : objetosHostiles) {
        obj->actualizar(); // En caso de que el objeto se mueva o cambie
        if (colisiona(
                submarino->getPosicion(), submarino->getSprite()->size(),
                obj->getPosicion(), obj->getSprite()->size())) {

            obj->interactuar(submarino);
        }
    }
}

// ➕ Añadir mina al mapa
void Nivel2::agregarMina(const QVector2D& pos) {
    Mina* mina = new Mina(this, pos);
    objetosHostiles.push_back(mina);
}



// ➕ Añadir torpedo al mapa
void Nivel2::agregarTorpedo(const QVector2D& pos, const QVector2D& dir) {
    Torpedo* torpedo = new Torpedo(this, pos, dir);
    objetosHostiles.push_back(torpedo);
}


