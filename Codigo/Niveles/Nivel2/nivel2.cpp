#include "Nivel2.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QRectF>
#include <cmath>
#include <QDateTime>
#include <QPropertyAnimation>


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
    oleadas.push_back({ QVector2D(600, 100), QVector2D(700, 300) });
    oleadas.push_back({ QVector2D(550, 250), QVector2D(750, 150) });
    oleadas.push_back({ QVector2D(620, 200), QVector2D(720, 350) });

    indiceOleadaActual = 0;
    cargarOleadaActual();

    // Pausar el juego para el diálogo inicial
    timerActualizacion->stop();
    controlesHabilitados = false;

    QLabel* dialogo = new QLabel(this);
    dialogo->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 180); color: white; font: bold 16px; padding: 10px; border-radius: 5px; }");
    dialogo->setAlignment(Qt::AlignCenter);
    dialogo->setGeometry(100, 450, 600, 60);
    dialogo->show();

    dialogo->setText("Son Goku: Hemos llegado a la zona enemiga...");

    QTimer::singleShot(3000, this, [=]() {
        dialogo->setText("Soldado RR: ¡No saldrás de aquí con vida!");

        QTimer::singleShot(3000, this, [=]() {
            dialogo->setText("Son Goku: ¡Veremos quién gana esta batalla!");

            QTimer::singleShot(3000, this, [=]() {
                dialogo->hide();
                dialogo->deleteLater();
                controlesHabilitados = true;
                timerActualizacion->start(16); // ▶️ Reanudar juego después del diálogo
            });
        });
    });

    // Timer principal de juego
    connect(timerActualizacion, &QTimer::timeout, this, [=]() {
        if (!fase2Activa && controlesHabilitados) {
            bool detenerMovimiento = teclasPresionadas.contains(Qt::Key_Space) && (
                                         teclasPresionadas.contains(Qt::Key_A) ||
                                         teclasPresionadas.contains(Qt::Key_W) ||
                                         teclasPresionadas.contains(Qt::Key_S)
                                         );
            submarino->procesarEntrada(teclasPresionadas, !detenerMovimiento);
            submarino->aplicarFisica();
            submarino->actualizar();
        }

        // Disparo controlado
        qint64 ahora = QDateTime::currentMSecsSinceEpoch();

        if (controlesHabilitados &&
            teclasPresionadas.contains(Qt::Key_Space) &&
            ahora - ultimoDisparo >= intervaloDisparo) {

            QVector2D direccion(1, 0);
            if (teclasPresionadas.contains(Qt::Key_W)) direccion = QVector2D(0, -1);
            else if (teclasPresionadas.contains(Qt::Key_S)) direccion = QVector2D(0, 1);
            else if (teclasPresionadas.contains(Qt::Key_A)) direccion = QVector2D(-1, 0);

            QVector2D origen = submarino->getPosicion();
            QSize tamañoSubmarino = submarino->getSprite()->size();

            if (direccion == QVector2D(1, 0)) origen += QVector2D(tamañoSubmarino.width(), tamañoSubmarino.height() / 2);
            else if (direccion == QVector2D(-1, 0)) origen += QVector2D(-20, tamañoSubmarino.height() / 2);
            else if (direccion == QVector2D(0, -1)) origen += QVector2D(tamañoSubmarino.width() / 2, -20);
            else if (direccion == QVector2D(0, 1)) origen += QVector2D(tamañoSubmarino.width() / 2, tamañoSubmarino.height());

            agregarTorpedo(origen, direccion, true);
            ultimoDisparo = ahora;
        }

        // Movimiento de torpedos
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

        // Movimiento de enemigos
        for (SubmarinoEnemigo* enemigo : enemigos) {
            enemigo->actualizar();
        }

        // Objetos hostiles
        for (Objeto* obj : objetosHostiles) {
            obj->actualizar();
        }

        // Colisiones
        verificarColisiones();

        // Derrota
        if (submarino->getVida() <= 0) {
            qDebug() << "¡Jugador derrotado!";
            timerActualizacion->stop();
            mostrarMensajeDerrota();

            QTimer::singleShot(4000, this, [this]() {
                emit regresarAlMenu();
            });
        }

        // Verificación de oleadas
        bool todosDestruidos = std::all_of(enemigos.begin(), enemigos.end(), [](SubmarinoEnemigo* e) {
            return e->estaDestruido();
        });

        if (!enemigos.empty() && todosDestruidos) {
            qDebug() << "Oleada completada.";
            limpiarEnemigos();
            cargarOleadaActual(); // ← incremento dentro del método
        }
    });

    this->setFocus();

    // Música de fondo
    musicaFondo = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);
    musicaFondo->setAudioOutput(salidaAudio);

    musicaFondo->setSource(QUrl("qrc:/Sonidos/Nivel2-S/nivel2-soundtrack.mp3"));
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    salidaAudio->setVolume(0.6);  // Puedes ajustar el volumen (0 a 100)

    musicaFondo->play();

}


// Evento: presionar tecla
void Nivel2::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());

    //Menú de pausa con Esc
    if (event->key() == Qt::Key_Escape) {
        if (!menuPausa || !menuPausa->isVisible()) {
            mostrarMenuPausa();
            timerActualizacion->stop(); // Pausar juego
        } else {
            ocultarMenuPausa();
            timerActualizacion->start(16); // Reanudar
        }
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
        obj->actualizar();

        // Ignorar colisiones con torpedos lanzados por el jugador
        if (dynamic_cast<Torpedo*>(obj)) continue;
    }

    for (int i = 0; i < objetosHostiles.size(); ++i) {
        Objeto* obj1 = objetosHostiles[i];
        if (!obj1->getSprite()->isVisible()) continue;

        // Colisión con el jugador
        if (colisiona(submarino->getPosicion(), submarino->getSprite()->size(),
                      obj1->getPosicion(), obj1->getSprite()->size())) {
            mostrarExplosion(obj1->getPosicion());

            // 🔊 Sonido de impacto
            QMediaPlayer* efecto = new QMediaPlayer(this);
            QAudioOutput* salida = new QAudioOutput(this);
            efecto->setAudioOutput(salida);
            efecto->setSource(QUrl("qrc:/Sonidos/Nivel2-S/torpedo-efecto.mp3"));
            efecto->play();

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

                // 🔊 Sonido de impacto
                QMediaPlayer* efecto = new QMediaPlayer(this);
                QAudioOutput* salida = new QAudioOutput(this);
                efecto->setAudioOutput(salida);
                efecto->setSource(QUrl("qrc:/Sonidos/Nivel2-S/torpedo-efecto.mp3"));
                efecto->play();

                obj1->interactuar(obj2);
                obj2->interactuar(obj1);
            }
        }

        // Torpedos del jugador vs enemigos
        for (Torpedo* torpedo : torpedosJugador) {
            if (!torpedo->getSprite()->isVisible()) continue;

            for (Entidad* entidad : enemigos) {
                if (!entidad->getSprite()->isVisible()) continue;

                if (colisiona(
                        torpedo->getPosicion(), torpedo->getSprite()->size(),
                        entidad->getPosicion(), entidad->getSprite()->size())) {

                    torpedo->interactuar(entidad);

                    // Si es un submarino enemigo, aplicar daño
                    SubmarinoEnemigo* enemigo = dynamic_cast<SubmarinoEnemigo*>(entidad);
                    if (enemigo) {
                        enemigo->recibirDaño(10);
                    }

                    mostrarExplosion(torpedo->getPosicion());

                    // 🔊 Sonido de impacto
                    QMediaPlayer* efecto = new QMediaPlayer(this);
                    QAudioOutput* salida = new QAudioOutput(this);
                    efecto->setAudioOutput(salida);
                    efecto->setSource(QUrl("qrc:/Sonidos/Nivel2-S/torpedo-efecto.mp3"));
                    efecto->play();
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

    QPixmap original = torpedo->getSprite()->pixmap();
    QPixmap rotado = original;

    if (delJugador) {
        if (dir == QVector2D(-1, 0)) {
            rotado = original.transformed(QTransform().scale(-1, 1));  // Izquierda
        } else if (dir == QVector2D(0, -1)) {
            rotado = original.transformed(QTransform().rotate(-90));   // Arriba
        } else if (dir == QVector2D(0, 1)) {
            rotado = original.transformed(QTransform().rotate(90));    // Abajo
        }
        // Derecha (1, 0) no requiere transformación
        torpedo->getSprite()->setPixmap(rotado.scaled(torpedo->getSprite()->size()));
    } else {
        // Misiles enemigos van a la izquierda por defecto
        rotado = original.transformed(QTransform().scale(-1, 1));
        torpedo->getSprite()->setPixmap(rotado.scaled(torpedo->getSprite()->size()));
    }

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
    QPixmap imagen(":/Sprites/UI/explosion.png");

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
    if (musicaFondo) musicaFondo->stop();

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
    for (SubmarinoEnemigo* enemigo : enemigos) {
        enemigo->destruir();  //  forma correcta
    }
    enemigos.clear();  // Elimina referencias
}



void Nivel2::iniciarFase2() {
    // 1. Limpiar enemigos y torpedos
    limpiarEnemigos();
    for (Torpedo* t : torpedosJugador) {
        t->getSprite()->hide();
        delete t;
    }
    torpedosJugador.clear();
    teclasPresionadas.clear();

    // 2. Detener control del jugador
    controlesHabilitados = false;
    timerActualizacion->stop();  // Detener el juego durante el diálogo

    // 3. Mostrar diálogo final antes del escape
    QLabel* dialogoFinal = new QLabel(this);
    dialogoFinal->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 180); color: white; font: bold 16px; padding: 10px; border-radius: 5px; }");
    dialogoFinal->setAlignment(Qt::AlignCenter);
    dialogoFinal->setGeometry(100, 450, 600, 60);
    dialogoFinal->show();

    dialogoFinal->setText("Son Goku: ¡Lo logramos... pero debemos salir de aquí!");

    QTimer::singleShot(3000, this, [=]() {
        dialogoFinal->setText("Son Goku: La cueva está justo delante, ¡vamos!");

        QTimer::singleShot(3000, this, [=]() {
            dialogoFinal->hide();
            dialogoFinal->deleteLater();

            //  iniciar secuencia hacia la cueva
            QLabel* cueva = new QLabel(this);
            QPixmap spriteCueva(":/Sprites/Fondos/cueva.png");
            spriteCueva = spriteCueva.transformed(QTransform().scale(-1, 1));  // Invertido horizontal
            cueva->setPixmap(spriteCueva.scaled(250, 250)); // Más grande
            cueva->move(width(), 300);
            cueva->show();

            QPropertyAnimation* animCueva = new QPropertyAnimation(cueva, "pos");
            animCueva->setDuration(3000);
            animCueva->setStartValue(QPoint(width(), 300));
            animCueva->setEndValue(QPoint(width() - 200, 300));
            animCueva->start(QAbstractAnimation::DeleteWhenStopped);

            QVector2D objetivoCueva(width() - 150, 300);  // Posición final de la cueva
            QVector2D direccion = (objetivoCueva - submarino->getPosicion()).normalized();

            QTimer* movimientoAuto = new QTimer(this);
            connect(movimientoAuto, &QTimer::timeout, this, [=]() mutable {
                QVector2D pos = submarino->getPosicion();
                pos += direccion * 2.5f;  // Velocidad (ajusta si quieres que sea más lenta o rápida)

                submarino->setPosicion(pos);
                submarino->getSprite()->move(pos.toPoint());

                // Si está suficientemente cerca, finaliza
                if ((pos - objetivoCueva).length() <= 5.0f) {
                    movimientoAuto->stop();
                    mostrarMensajeVictoria();
                }
            });
            movimientoAuto->start(16); // ~60 FPS

        });
    });
}


void Nivel2::reiniciarNivel() {
    // Detener el juego
    timerActualizacion->stop();
    // Cerrar esta instancia del nivel
    this->close();
    // Crear nueva instancia del nivel con el mismo parent
    Nivel2* nuevoNivel = new Nivel2(parentWidget());
    // Conectar la señal para regresar al menú desde el nuevo nivel
    connect(nuevoNivel, &Nivel2::regresarAlMenu, this, [=]() {
        emit regresarAlMenu(); // Reenvía la señal al juego
    });
    // Mostrar la nueva instancia y cargar el nivel
    nuevoNivel->show();
    nuevoNivel->setFixedSize(800, 600); // Asegura tamaño completo
    nuevoNivel->cargarNivel();
}

void Nivel2::mostrarMensajeVictoria() {
    QLabel* mensaje = new QLabel("¡Victoria!", this);
    mensaje->setStyleSheet("QLabel { color: white; font-size: 36px; font-weight: bold; background-color: rgba(0,0,0,150); padding: 10px; }");
    mensaje->adjustSize();
    mensaje->move((width() - mensaje->width()) / 2, (height() - mensaje->height()) / 2);
    mensaje->show();

    QTimer::singleShot(3000, this, [=]() {
        if (musicaFondo) musicaFondo->stop();
        emit regresarAlMenu();  // ← Este ya está conectado al botón también
    });
}

void Nivel2::detenerMusica() {
    if (musicaFondo) {
        musicaFondo->stop();
        musicaFondo->deleteLater();
        musicaFondo = nullptr;
    }

    if (salidaAudio) {
        salidaAudio->deleteLater();
        salidaAudio = nullptr;
    }
}
