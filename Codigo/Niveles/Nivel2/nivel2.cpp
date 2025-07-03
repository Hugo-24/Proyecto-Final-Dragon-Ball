#include "Nivel2.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QRectF>
#include <cmath>
#include <QDateTime>


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

    // Barra de vida del jugador
    barraVida = new QProgressBar(this);
    barraVida->setGeometry(10, 10, 200, 25);
    barraVida->setRange(0, submarino->getVidaMaxima());
    barraVida->setValue(submarino->getVida());
    barraVida->setStyleSheet(
        "QProgressBar {"
        "  border: 2px solid grey;"
        "  border-radius: 5px;"
        "  text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: #4CAF50;"
        "  width: 20px;"
        "}"
        );
    barraVida->show();

    // Minas
    agregarMina(QVector2D(400, 350));
    agregarMina(QVector2D(600, 200));
    agregarMina(QVector2D(250, 400));
    agregarMina(QVector2D(100, 150));

    // 🚨 Paso 1: Definir las oleadas
    oleadas.clear();
    oleadas.push_back({ QVector2D(600, 100), QVector2D(700, 300) }); // Primera oleada
    oleadas.push_back({ QVector2D(550, 250), QVector2D(750, 150) }); // Segunda oleada
    oleadas.push_back({ QVector2D(620, 200), QVector2D(720, 350) }); // Tercera oleada

    indiceOleadaActual = 0; // Reiniciar índice
    cargarOleadaActual();   // Cargar la primera oleada

    // ⏱️ Timer principal de juego
    connect(timerActualizacion, &QTimer::timeout, this, [=]() {
        // 1. Movimiento del jugador
        submarino->procesarEntrada(teclasPresionadas);
        submarino->aplicarFisica();
        submarino->actualizar();

        // 1. Disparo controlado
        ////////////////////////////////////////
        qint64 ahora = QDateTime::currentMSecsSinceEpoch();

        if (teclasPresionadas.contains(Qt::Key_Space) && ahora - ultimoDisparo >= intervaloDisparo) {
            QVector2D direccion(1, 0); // Por defecto: derecha

            if (teclasPresionadas.contains(Qt::Key_W)) direccion = QVector2D(0, -1);  // arriba
            else if (teclasPresionadas.contains(Qt::Key_S)) direccion = QVector2D(0, 1);  // abajo
            else if (teclasPresionadas.contains(Qt::Key_A)) direccion = QVector2D(-1, 0); // izquierda

            QVector2D origen = submarino->getPosicion();
            QSize tamañoSubmarino = submarino->getSprite()->size();

            if (direccion == QVector2D(1, 0)) {        // Derecha
                origen += QVector2D(tamañoSubmarino.width(), tamañoSubmarino.height() / 2);
            }
            else if (direccion == QVector2D(-1, 0)) {  // Izquierda
                origen += QVector2D(-20, tamañoSubmarino.height() / 2);
            }
            else if (direccion == QVector2D(0, -1)) {  // Arriba
                origen += QVector2D(tamañoSubmarino.width() / 2, -20);
            }
            else if (direccion == QVector2D(0, 1)) {   // Abajo
                origen += QVector2D(tamañoSubmarino.width() / 2, tamañoSubmarino.height());
            }


            agregarTorpedo(origen, direccion, true);
            ultimoDisparo = ahora;
        }


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

        // 4. Actualización de objetos hostiles
        for (Objeto* obj : objetosHostiles) {
            obj->actualizar();
        }

        // 5. Verificación de colisiones
        verificarColisiones();

        // 6. Verificación de derrota
        if (submarino->getVida() <= 0) {
            qDebug() << "¡Jugador derrotado!";
            timerActualizacion->stop();
            mostrarMensajeDerrota();

            QTimer::singleShot(4000, this, [this]() {
                emit regresarAlMenu();
            });
        }

        // 7. Verificación de oleadas
        bool todosDestruidos = std::all_of(enemigos.begin(), enemigos.end(), [](SubmarinoEnemigo* e) {
            return e->estaDestruido();
        });

        if (!enemigos.empty() && todosDestruidos) {
            qDebug() << "Oleada completada.";
            limpiarEnemigos();

            ++indiceOleadaActual;         // ← Aquí sí debe ir el incremento
            cargarOleadaActual();
        }


    });


    timerActualizacion->start(16); // ~60 FPS
    this->setFocus();

    // Botón salir al menú
    btnSalir = new QPushButton("Salir al menú", this);
    btnSalir->setGeometry(650, 10, 120, 30);
    connect(btnSalir, &QPushButton::clicked, this, [=]() {
        emit regresarAlMenu();
    });
    btnSalir->show();
}


// Evento: presionar tecla
void Nivel2::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());

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
            barraVida->setValue(submarino->getVida());
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


void Nivel2::mostrarMensajeDerrota() {
    QLabel* mensaje = new QLabel("¡Has perdido!", this);
    mensaje->setStyleSheet("color: red; font-size: 30px; font-weight: bold;");
    mensaje->adjustSize();
    mensaje->move(width() / 2 - mensaje->width() / 2, height() / 2 - mensaje->height() / 2);
    mensaje->show();
}


void Nivel2::cargarOleadaActual() {
    if (indiceOleadaActual >= oleadas.size()) {
        iniciarFase2();  // ← Aquí transicionas cuando se terminen las 3 oleadas
        return;
    }

    const QVector<QVector2D>& posiciones = oleadas[indiceOleadaActual];
    for (const QVector2D& pos : posiciones) {
        SubmarinoEnemigo* enemigo = new SubmarinoEnemigo(this, pos);
        enemigo->setObjetivo(submarino);
        enemigos.push_back(enemigo);

        // 🔧 Recuperar conexión de disparo
        connect(enemigo, &SubmarinoEnemigo::torpedoDisparado, this, [=](QVector2D pos, QVector2D dir) {
            agregarTorpedo(pos, dir, false); // false porque es un enemigo
        });

    }

    ++indiceOleadaActual;
}



void Nivel2::limpiarEnemigos() {
    for (int i = enemigos.size() - 1; i >= 0; --i) {
        if (enemigos[i]->estaDestruido()) {
            enemigos[i]->deleteLater();
            enemigos.removeAt(i);
        }
    }
}

void Nivel2::iniciarFase2() {
    qDebug() << "Fase 2 iniciada (aquí va la lógica de transición)";
    // Aquí va la lógica para cambiar a la segunda fase del nivel
}







