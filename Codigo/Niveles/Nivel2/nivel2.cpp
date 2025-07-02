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
    // Fondo submarino
    mapa = new Mapa(":/Sprites/Fondos/fondo_nivel2.jpg", this);
    mapa->setGeometry(0, 0, 800, 600);
    mapa->show();

    // Submarino del jugador
    submarino = new SubmarinoJugador(this);
    submarino->getSprite()->raise();

    barraVida = new QProgressBar(this);
    barraVida->setGeometry(10, 10, 200, 25);              // posición y tamaño
    barraVida->setRange(0, submarino->getVidaMaxima());  // rango de la barra
    barraVida->setValue(submarino->getVida());           // valor inicial
    barraVida->show();



    // Objetos hostiles (minas)
    agregarMina(QVector2D(400, 350));
    agregarMina(QVector2D(600, 200));
    agregarMina(QVector2D(250, 400));
    agregarMina(QVector2D(100, 150));

    // Submarinos enemigos
    QVector<QVector2D> posicionesEnemigos = {
        QVector2D(600, 100),
        QVector2D(700, 300)
    };

    for (const QVector2D& pos : posicionesEnemigos) {
        SubmarinoEnemigo* enemigo = new SubmarinoEnemigo(this, pos);
        enemigos.push_back(enemigo);

        // 🎯 Comportamiento de ataque cuando jugador está en rango
        QTimer* ataqueTimer = new QTimer(enemigo);
        connect(ataqueTimer, &QTimer::timeout, this, [=]() {
            if (!enemigo->getSprite()->isVisible() || !submarino) return;

            float distanciaX = std::abs(enemigo->getPosicion().x() - submarino->getPosicion().x());

            // Si el jugador está en rango horizontal
            if (distanciaX < 300) {
                static int contador = 0;

                if (contador < 8 * (1000 / 800)) { // ~8 segundos con paso cada 800ms
                    QVector2D direccion = (submarino->getPosicion() - enemigo->getPosicion()).normalized();
                    agregarTorpedo(enemigo->getPosicion(), direccion, false);
                    ++contador;
                } else {
                    contador = 0;
                }
            }
        });
        ataqueTimer->start(800); // Verifica cada 800 ms


    }

    // ⏱️ Timer principal de juego
    connect(timerActualizacion, &QTimer::timeout, this, [=]() {
        // 1. Movimiento del jugador
        submarino->procesarEntrada(teclasPresionadas);
        submarino->aplicarFisica();
        submarino->actualizar();

        // 2. Movimiento de torpedos
        for (int i = 0; i < torpedos.size(); ++i) {
            Torpedo* t = torpedos[i];
            t->actualizar();

            if (t->getPosicion().x() > width() || t->getPosicion().x() < 0) {
                t->getSprite()->hide();
                delete t;
                torpedos.removeAt(i);
                --i;
            }
        }

        // 3. Movimiento de enemigos
        for (SubmarinoEnemigo* enemigo : enemigos) {
            enemigo->actualizar();
        }

        // 4. Actualización de objetos
        for (Objeto* obj : objetosHostiles) {
            obj->actualizar();
        }

        // 5. Verificación de colisiones
        verificarColisiones();
    });

    timerActualizacion->start(16); // ~60 FPS
    this->setFocus();
}


// Evento: presionar tecla
void Nivel2::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());

    if (event->key() == Qt::Key_Space && submarino) {
        QVector2D posicionInicial = submarino->getPosicion() + QVector2D(80, 55); // un poco al frente
        QVector2D direccion = QVector2D(1, 0); // hacia la derecha
        agregarTorpedo(posicionInicial, direccion, true);
    }
}
// Evento: soltar tecla
void Nivel2::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}

// Colisiones entre submarino y objetos hostiles
void Nivel2::verificarColisiones() {




    for (Objeto* obj : objetosHostiles) {

        if (!obj->getSprite()->isVisible()) continue;

        obj->actualizar(); // En caso de que el objeto se mueva o cambie
        // Ignorar colisiones con torpedos lanzados por el jugador
        if (dynamic_cast<Torpedo*>(obj)) {
            continue;
        }

    }

    for (int i = 0; i < objetosHostiles.size(); ++i) {
        Objeto* obj1 = objetosHostiles[i];

        if (!obj1->getSprite()->isVisible()) continue;

        // Colisión con el jugador
        if (colisiona(submarino->getPosicion(), submarino->getSprite()->size(),
                      obj1->getPosicion(), obj1->getSprite()->size())) {
            mostrarExplosion(obj1->getPosicion());
            obj1->interactuar(submarino);

            submarino->recibirDanio(10);
        }

        // Colisión entre objetos (torpedo vs mina)
        for (int j = i + 1; j < objetosHostiles.size(); ++j) {
            Objeto* obj2 = objetosHostiles[j];

            if (!obj2->getSprite()->isVisible()) continue;

            if (colisiona(obj1->getPosicion(), obj1->getSprite()->size(),
                          obj2->getPosicion(), obj2->getSprite()->size())) {
                mostrarExplosion(obj1->getPosicion());
                obj1->interactuar(obj2);
                obj2->interactuar(obj1);
            }
        }

        for (Torpedo* torpedo : torpedosJugador) {
            if (!torpedo->getSprite()->isVisible()) continue;

            for (Entidad* entidad : enemigos) { // Asegúrate de tener un QVector<Entidad*> enemigos;
                if (!entidad->getSprite()->isVisible()) continue;

                if (colisiona(
                        torpedo->getPosicion(), torpedo->getSprite()->size(),
                        entidad->getPosicion(), entidad->getSprite()->size())) {

                    torpedo->interactuar(entidad);

                    // Intenta hacer cast a SubmarinoEnemigo y aplicar daño
                    SubmarinoEnemigo* enemigo = dynamic_cast<SubmarinoEnemigo*>(entidad);
                    if (enemigo) {
                        enemigo->recibirDaño(10);  // Ajusta el daño a lo que desees
                    }
                    mostrarExplosion(torpedo->getPosicion());
                }
            }
        }


        obj1->actualizar();
    }
}

// Añadir mina al mapa
void Nivel2::agregarMina(const QVector2D& pos) {
    Mina* mina = new Mina(this, pos);
    objetosHostiles.push_back(mina);
}



// Añadir torpedo al mapa
void Nivel2::agregarTorpedo(const QVector2D& pos, const QVector2D& dir, bool delJugador) {
    Torpedo* torpedo = new Torpedo(this, pos, dir);
    objetosHostiles.push_back(torpedo);

    if (delJugador)
        torpedosJugador.push_back(torpedo);
}


void Nivel2::agregarSubmarinoEnemigo(const QVector2D& pos) {
    SubmarinoEnemigo* enemigo = new SubmarinoEnemigo(this, pos);
    enemigo->getSprite()->setParent(this);  // Por si no lo hiciste en el constructor
    enemigo->getSprite()->move(pos.toPoint());
    enemigo->getSprite()->show();

    enemigos.append(enemigo);
}

void Nivel2::mostrarExplosion(const QVector2D& posicion) {
    QLabel* explosion = new QLabel(this);
    QPixmap imagen(":/Sprites/Nave/explosion.png");

    explosion->setPixmap(imagen.scaled(40, 40)); // Tamaño opcional
    explosion->setAttribute(Qt::WA_TranslucentBackground);
    explosion->move(posicion.toPoint());
    explosion->show();

    // Oculta y destruye la explosión después de un tiempo
    QTimer::singleShot(400, explosion, [explosion]() {
        explosion->hide();
        explosion->deleteLater();
    });
}




