#include "nivel3.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QDebug>

// Constructor
Nivel3::Nivel3(QWidget* parent)
    : Nivel(parent),
    jugador(nullptr),
    jefe(nullptr),
    timer(new QTimer(this)),
    musicaFondo(nullptr),
    salidaAudio(nullptr),
    sonidoVictoria(nullptr),
    salidaVictoria(nullptr),
    fondo(nullptr),
    offsetX(0)
{
    setFixedSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);
}

void Nivel3::cargarNivel() {
    // Fondo de cueva estático
    fondo = new QLabel(this);
    fondo->setPixmap(QPixmap(":/Sprites/Fondos/fondo_cueva.png").scaled(800, 600));
    fondo->setGeometry(0, 0, 800, 600);
    fondo->show();

    // Crear jugador Goku
    jugador = new Goku(this);
    jugador->setPosicion(QVector2D(100, 444));
    jugador->getSprite()->show();

    // Crear jefe General Blue
    jefe = new GeneralBlue(this, this, QVector2D(600, 444), jugador);
    jefe->getSprite()->show();

    // Barra de vida del jefe (visible arriba del sprite)
    barraVidaJefe = new QProgressBar(this);
    barraVidaJefe->setGeometry(jefe->getSprite()->x(), jefe->getSprite()->y() - 20, 100, 10);
    barraVidaJefe->setRange(0, 250);
    barraVidaJefe->setValue(250);
    barraVidaJefe->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    barraVidaJefe->show();

    // Corazones del jugador
    vidas = 3;
    inicializarCorazones(vidas);

    // Música de fondo
    musicaFondo = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);
    musicaFondo->setAudioOutput(salidaAudio);
    musicaFondo->setSource(QUrl("qrc:/Sonidos/Nivel3-S/nivel3-soundtrack.mp3"));
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    salidaAudio->setVolume(5);
    musicaFondo->play();

    // Bucle principal del juego (~60fps)
    connect(timer, &QTimer::timeout, this, [=]() {
       if (!jugador || estaMuerto || !controlesHabilitados || !jefe || !jugador->getPuedeMoverse()) return;

        // Movimiento del jugador
        actualizarMovimientoJugador();
        // Movimiento jefe
        jefe->actualizar();

        // Actualizar barra de vida del jefe
        if (jefe && barraVidaJefe) {
            barraVidaJefe->move(jefe->getSprite()->x(), jefe->getSprite()->y() - 20);
            barraVidaJefe->setValue(jefe->getVida());
        }

        // Actualizar proyectiles
        for (int i = 0; i < proyectiles.size(); ++i) {
            Proyectil* p = proyectiles[i];
            if (!p || !p->getSprite()) {
                delete p;
                proyectiles.removeAt(i--);
                continue;
            }

            p->actualizar();
            p->getSprite()->move(p->getPosicion().x(), p->getPosicion().y());

            QRect rProy(p->getPosicion().toPoint(), p->getSprite()->size());

            // Colisión con jefe
            if (p->esDelJugadorFunc() && jefe && !jefe->estaDerrotado()) {
                QRect rJefe(jefe->getPosicion().toPoint(), jefe->getSprite()->size());
                if (rProy.intersects(rJefe)) {
                    int vidaAntes = jefe->getVida();
                    jefe->recibirDanio(p->getDanio());
                    if (vidaAntes / 12 > jefe->getVida() / 12) {
                        Corazon* c = new Corazon(this, jefe->getPosicion(), this);
                        corazonesCurativos.append(c);
                    }
                    p->getSprite()->hide();
                    delete p;
                    proyectiles.removeAt(i--);
                    continue;
                }
            }

            // Colisión con jugador
            if (!p->esDelJugadorFunc()) {
                QRect rJugador(jugador->getPosicion().toPoint(), jugador->getSprite()->size());
                if (rProy.intersects(rJugador)) {
                    if (vidas > 0) {
                        vidas--;
                        actualizarCorazones(vidas);
                        if (vidas <= 0) {
                            timer->stop();
                            mostrarMensajeDerrota();
                        }
                    }
                    p->getSprite()->hide();
                    delete p;
                    proyectiles.removeAt(i--);
                    continue;
                }
            }
        }

        // Corazones curativos
        for (int i = 0; i < corazonesCurativos.size(); ++i) {
            Corazon* c = corazonesCurativos[i];
            if (!c || !c->getSprite()) continue;
            QRect rC(c->getSprite()->pos(), c->getSprite()->size());
            QRect rJ(jugador->getSprite()->pos(), jugador->getSprite()->size());

            if (rC.intersects(rJ)) {
                c->getSprite()->hide();
                if (vidas < 3) {
                    vidas++;
                    actualizarCorazones(vidas);
                }
                corazonesCurativos.removeAt(i--);
                delete c;
            }
        }

        // Victoria
        if (jefe && jefe->estaDerrotado()) {
            timer->stop();
            mostrarMensajeVictoria();
        }
    });
    // Habilita controles del jugador
    controlesHabilitados = true;
    // Asegura que el widget tenga el foco del teclado
    this->setFocus();
    // Inicia el bucle principal del juego
    timer->start(16);
    // Diálogo introductorio con General Blue
    timer->stop();               // Pausa el juego mientras se muestra el diálogo
    controlesHabilitados = false;
    jugador->setPuedeMoverse(false);

    QLabel* dialogo = new QLabel(this);
    dialogo->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 180); color: white; font: bold 16px; padding: 10px; border-radius: 5px; }");
    dialogo->setAlignment(Qt::AlignCenter);
    dialogo->setGeometry(100, 450, 600, 60);
    dialogo->show();
    // Línea 1
    dialogo->setText("General Blue: ¡Detente, Goku! Ahora estas en manos de la Red Ribbon.");

    QTimer::singleShot(1500, this, [=]() {
        dialogo->setText("General Blue: Entrégame el radar del dragón o te arrepentirás.");

        QTimer::singleShot(1500, this, [=]() {
            dialogo->setText("Goku: ¡Ni hablar! No permitiré que usen las esferas para hacer el mal.");

            QTimer::singleShot(1000, this, [=]() {
                dialogo->setText("General Blue: ¡Entonces prepárate para enfrentarme!");

                QTimer::singleShot(500, this, [=]() {
                    dialogo->hide();
                    dialogo->deleteLater();

                    controlesHabilitados = true;
                    jugador->setPuedeMoverse(true);
                    timer->start(16); // ▶️ Reanudar juego después del diálogo
                });
            });
        });
    });
}

// Entrada de teclado
void Nivel3::keyPressEvent(QKeyEvent* event) {
    if (estaMuerto || !jugador || !controlesHabilitados) return;

    teclasPresionadas.insert(event->key());

    // Movimiento izquierda/derecha
    if (event->key() == Qt::Key_A) {
        jugador->moverIzquierda();
    } else if (event->key() == Qt::Key_D) {
        jugador->moverDerecha();
    }

    // Salto
    if (event->key() == Qt::Key_W) {
        jugador->saltar();
    }

    // Disparo
    if (event->key() == Qt::Key_R) {
        if (jugador->estaListoParaDisparar()) {
            jugador->lanzarEnergia();
            jugador->setPuedeDisparar(false);
            QTimer::singleShot(400, this, [=]() {
                jugador->setPuedeDisparar(true);
            });
        }
    }

    // Pausa
    if (event->key() == Qt::Key_Escape) {
        if (!menuPausa || !menuPausa->isVisible()) {
            mostrarMenuPausa();
            timer->stop(); // Pausa
            controlesHabilitados = false;
            jugador->setPuedeMoverse(false);
        } else {
            ocultarMenuPausa();
            controlesHabilitados = true;
            jugador->setPuedeMoverse(true);
            timer->start(16); // Reanuda
        }
    }
}

// Tecla soltada
void Nivel3::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());

    if (!jugador || !controlesHabilitados) return;

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
    }
}
// Agrega un proyectil al nivel
void Nivel3::agregarProyectil(Proyectil* p) {
    proyectiles.append(p);
}

// Inicializa corazones visuales del jugador
void Nivel3::inicializarCorazones(int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        QLabel* c = new QLabel(this);
        c->setPixmap(QPixmap(":/Sprites/UI/corazon_lleno.png").scaled(32, 32));
        c->setGeometry(10 + i * 36, 10, 32, 32);
        c->show();
        corazones.append(c);
    }
}

// Actualiza corazones según cantidad actual de vida
void Nivel3::actualizarCorazones(int nuevaVida) {
    for (int i = 0; i < corazones.size(); ++i) {
        corazones[i]->setPixmap(QPixmap(i < nuevaVida
                                            ? ":/Sprites/UI/corazon_lleno.png"
                                            : ":/Sprites/UI/corazon_vacio.png").scaled(32, 32));
    }
}

// Derrota del jugador
void Nivel3::mostrarMensajeDerrota() {
    estaMuerto = true;

    if (jugador) {
        jugador->detenerAnimacionCaminar();
        jugador->setMuerto(true);
    }
    // Mostrar sprite de muerte del personaje según el tipo y dirección
    if (jugador && jugador->getSprite()) {
        QString rutaMuerte;

        // Determina el sprite correspondiente a la muerte
        rutaMuerte = jugador->estaMirandoDerecha()
        ? ":/Sprites/Goku/R_Goku_Muerto.png"
        : ":/Sprites/Goku/L_Goku_Muerto.png";

        // Aplica el sprite de muerte y lo muestra con tamaño correcto
        jugador->getSprite()->setPixmap(QPixmap(rutaMuerte).scaled(100, 100));
    }

    // Sonido de muerte
    detenerMusica();
    QMediaPlayer* efectoMuerte = new QMediaPlayer(this);
    QAudioOutput* salidaMuerte = new QAudioOutput(this);
    efectoMuerte->setAudioOutput(salidaMuerte);
    efectoMuerte->setSource(QUrl("qrc:/Sonidos/Nivel1-S/muerte-sonido.mp3"));
    salidaMuerte->setVolume(10);
    efectoMuerte->play();

    // Muestra mensaje visual de derrota en pantalla
    QLabel* mensaje = new QLabel("¡Has muerto!", this);
    mensaje->setStyleSheet("color: red; font-size: 30px; font-weight: bold;");
    mensaje->adjustSize();
    mensaje->move(width() / 2 - mensaje->width() / 2, height() / 2 - mensaje->height() / 2);
    mensaje->show();

    // Espera unos segundos y luego regresa al menú principal
    QTimer::singleShot(4000, this, [this]() {
        reiniciarNivel();
    });
}

// Victoria del jugador
void Nivel3::mostrarMensajeVictoria() {
    // Detiene el timer principal para congelar el juego
    timer->stop();
    controlesHabilitados = false;

    // Muestra mensaje de victoria en el centro
    QLabel* mensaje = new QLabel("¡Victoria! Has derrotado al General Blue", this);
    mensaje->setStyleSheet("color: white; font-size: 26px; background-color: black; padding: 10px;");
    mensaje->adjustSize();
    mensaje->move(width() / 2 - mensaje->width() / 2, height() / 2 - mensaje->height() / 2);
    mensaje->show();

    // Detener música
    detenerMusica();

    // Sonido de victoria
    sonidoVictoria = new QMediaPlayer(this);
    salidaVictoria = new QAudioOutput(this);
    sonidoVictoria->setAudioOutput(salidaVictoria);
    sonidoVictoria->setSource(QUrl("qrc:/Sonidos/efecto-victoria.mp3"));
    salidaVictoria->setVolume(100);
    sonidoVictoria->play();

    // Elimina proyectiles, corazones curativos y barra de vida del jefe
    limpiarProyectiles();
    limpiarCorazones();

    if (barraVidaJefe) {
        barraVidaJefe->hide();
        barraVidaJefe->deleteLater();
        barraVidaJefe = nullptr;
    }

    // El jefe mantiene sprite de muerte visible (no se elimina)

    // Después de 5 segundos, volver al menú
    QTimer::singleShot(5000, this, [=]() {
        emit regresarAlMenu();
    });
}

// Lógica de movimiento continuo del jugador (WASD)
void Nivel3::actualizarMovimientoJugador() {
    if (!jugador || estaMuerto || !controlesHabilitados) return;

    // Movimiento lateral continuo con A/D
    if (teclasPresionadas.contains(Qt::Key_A)) {
        jugador->moverIzquierda();
    } else if (teclasPresionadas.contains(Qt::Key_D)) {
        jugador->moverDerecha();
    } else {
        jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
    }

    // Gravedad y físicas
    jugador->aplicarFisica();

    // Lógica de animación y posición
    jugador->actualizar();

    // Sin scroll, mueve sprite directamente
    jugador->getSprite()->move(jugador->getPosicion().toPoint());
}

void Nivel3::agregarCorazon(Corazon* c) {
    corazonesCurativos.append(c);
}

// Limpieza de proyectiles
void Nivel3::limpiarProyectiles() {
    for (Proyectil*& p : proyectiles) {
        if (p) delete p;
    }
    proyectiles.clear();
}

// Limpieza de corazones
void Nivel3::limpiarCorazones() {
    for (Corazon*& c : corazonesCurativos) {
        if (c) delete c;
    }
    corazonesCurativos.clear();
}

// Limpieza del jugador
void Nivel3::limpiarJugador() {
    if (jugador) {
        delete jugador;
        jugador = nullptr;
    }
}

// Limpieza del jefe
void Nivel3::limpiarJefe() {
    if (jefe) {
        delete jefe;
        jefe = nullptr;
    }
}

// Limpieza del fondo
void Nivel3::limpiarFondo() {
    if (fondo) {
        fondo->deleteLater();
        fondo = nullptr;
    }
}

// Reinicia el nivel por completo
void Nivel3::reiniciarNivel() {
    timer->stop();
    limpiarProyectiles();
    limpiarCorazones();
    limpiarJugador();
    limpiarJefe();
    limpiarFondo();

    Nivel3* nuevo = new Nivel3(parentWidget());
    connect(nuevo, &Nivel3::regresarAlMenu, this, [=]() {
        emit regresarAlMenu();
    });
    nuevo->setFixedSize(800, 600);
    nuevo->show();
    nuevo->cargarNivel();

    deleteLater();
}

// Detiene música de fondo
void Nivel3::detenerMusica() {
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
