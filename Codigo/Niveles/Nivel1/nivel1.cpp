#include "nivel1.h"
#include "roshi.h"
#include "lunch.h"
#include "proyectil.h"
#include "Enemigos/soldadopatrullarroja.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QLabel>
#include <QTimer>
#include <QDebug>

// Constructor: inicializa los atributos en el orden declarado para evitar warnings
Nivel1::Nivel1(QWidget* parent)
    : Nivel(parent),
    jugador(nullptr),
    timer(new QTimer(this)),
    enemigos(),
    fondoSeleccion(new QLabel(this)),
    selector(new QWidget(this)),
    fondoNivel(nullptr),
    offsetX(0)
{
    setFixedSize(800, 600);
    setFocusPolicy(Qt::StrongFocus); // Capturar teclado
}
void Nivel1::inicializarCorazones(int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        QLabel* corazon = new QLabel(this);
        corazon->setPixmap(QPixmap(":/Sprites/UI/corazon_lleno.png").scaled(32, 32));
        corazon->setGeometry(10 + i * 36, 10, 32, 32);
        corazon->show();
        corazones.append(corazon);
    }
}

void Nivel1::mostrarExplosion(const QVector2D& pos) {
    QLabel* expl = new QLabel(this);
    expl->setPixmap(QPixmap(":/Sprites/UI/explosion.png").scaled(80, 80));
    expl->move(pos.x() - offsetX, pos.y() - 20);
    expl->show();
    expl->raise();

    QTimer::singleShot(500, this, [expl]() {
        expl->hide();
        expl->deleteLater();
    });
}

void Nivel1::mostrarMensajeDerrota() {
    estaMuerto = true;

    if (jugador) {
        jugador->detenerAnimacionCaminar();
        jugador->setMuerto(true);
    }
    // Mostrar sprite de muerte del personaje según el tipo y dirección
    if (jugador && jugador->getSprite()) {
        QString rutaMuerte;

        // Determina el sprite correspondiente a la muerte
        if (dynamic_cast<Roshi*>(jugador)) {
            rutaMuerte = jugador->estaMirandoDerecha()
            ? ":/Sprites/Roshi/R_Roshi_Muerto.png"
            : ":/Sprites/Roshi/L_Roshi_Muerto.png";
        } else if (dynamic_cast<Lunch*>(jugador)) {
            rutaMuerte = jugador->estaMirandoDerecha()
            ? ":/Sprites/Lunch/R_Lunch_Muerta.png"
            : ":/Sprites/Lunch/L_Lunch_Muerta.png";
        }

        // Aplica el sprite de muerte y lo muestra con tamaño correcto
        jugador->getSprite()->setPixmap(QPixmap(rutaMuerte).scaled(100, 100));
    }

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
// Carga el nivel: muestra selección de personaje y prepara scroll
void Nivel1::cargarNivel() {
    // Fondo visible durante la selección de personaje
    fondoSeleccion->setPixmap(QPixmap(":/Sprites/Fondos/fondo_nivel1.jpg").scaled(800, 600));
    fondoSeleccion->setGeometry(0, 0, 800, 600);
    fondoSeleccion->show();
    // Música de selección de personaje (fase previa al juego)
    reproductorSeleccion = new QMediaPlayer(this);
    salidaAudioSeleccion = new QAudioOutput(this);

    reproductorSeleccion->setAudioOutput(salidaAudioSeleccion);
    salidaAudioSeleccion->setVolume(50);
    reproductorSeleccion->setSource(QUrl("qrc:/Sonidos/Nivel1-S/menu_nivel1.mp3")); // o usa el mismo mp3 del menú
    reproductorSeleccion->setLoops(QMediaPlayer::Infinite);
    reproductorSeleccion->play();

    // Contenedor de botones y texto de selección
    selector->setGeometry(0, 0, 800, 600);
    QVBoxLayout* layout = new QVBoxLayout(selector);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 124, 0, 0);

    // Texto introductorio
    QLabel* texto = new QLabel("¡Defiende la Kame House del ataque de la Patrulla Roja!\nSelecciona un personaje:", this);
    texto->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    texto->setAlignment(Qt::AlignCenter);
    texto->setWordWrap(true);

    // Botones de selección
    QPushButton* btnRoshi = new QPushButton("Jugar con Roshi", this);
    QPushButton* btnLunch = new QPushButton("Jugar con Launch", this);
    btnRoshi->setFixedSize(300, 60);
    btnLunch->setFixedSize(300, 60);

    QFont font = btnRoshi->font();
    font.setPointSize(16);
    btnRoshi->setFont(font);
    btnLunch->setFont(font);

    layout->addWidget(texto);
    layout->addWidget(btnRoshi);
    layout->addWidget(btnLunch);
    selector->setLayout(layout);
    selector->show();

    // Función para iniciar el juego con un personaje
    auto iniciarJuego = [&](Personaje* personaje) {
        fondoSeleccion->hide();
        selector->hide();

        // Fondo de juego con 4 secciones encadenadas (8000px total)
        fondoNivel = new QWidget(this);
        fondoNivel->setGeometry(0, 0, 8000, 600);
        fondoNivel->show();

        for (int i = 0; i < 4; ++i) {
            QLabel* fondo = new QLabel(fondoNivel);
            QString ruta = (i % 2 == 0)
                               ? ":/Sprites/Fondos/Fondo_scroll_largo.jpg"
                               : ":/Sprites/Fondos/Fondo_scroll_largo_2.jpg";
            fondo->setPixmap(QPixmap(ruta).scaled(2000, 600));
            fondo->setGeometry(i * 2000, 0, 2000, 600);
            fondo->show();
        }

        jugador = personaje;
        jugador->setPosicion(QVector2D(100, 420));
        jugador->getSprite()->show();
        jugador->getSprite()->raise();
        //Crear enemigos de la Patrulla Roja
        // Aparecen ya caminando desde el fondo hacia el jugador
        SoldadoPatrullaRoja* enemigo1 = new SoldadoPatrullaRoja(this, this, QVector2D(800, 420), jugador, true);
        enemigos.append(enemigo1);

        SoldadoPatrullaRoja* enemigo2 = new SoldadoPatrullaRoja(this, this, QVector2D(800, 420), jugador, true);
        enemigos.append(enemigo2);
        // Inicializa sistema de corazones (5 vidas por defecto)
        vidas = 5;
        inicializarCorazones(vidas);
        offsetX = 0;

        // Música de fondo para el nivel
        if (reproductorSeleccion) reproductorSeleccion->stop();
        reproductorNivel = new QMediaPlayer(this);
        salidaAudioNivel = new QAudioOutput(this);

        reproductorNivel->setAudioOutput(salidaAudioNivel);
        salidaAudioNivel->setVolume(50);  // Volumen entre 0 y 100

        reproductorNivel->setSource(QUrl("qrc:/Sonidos/Nivel1-S/nivel1-soundtrack.mp3")); // Ajusta la ruta si es diferente
        reproductorNivel->setLoops(QMediaPlayer::Infinite); // Repetir indefinidamente
        reproductorNivel->play();  // ¡Empieza la música!
        setFocus();

        if (!timer->isActive()) {
            timer->start(16); // ~60 FPS
        }
    };

    connect(btnRoshi, &QPushButton::clicked, this, [=]() {
        iniciarJuego(new Roshi(this));
    });

    connect(btnLunch, &QPushButton::clicked, this, [=]() {
        iniciarJuego(new Lunch(this));
    });

    // Bucle principal del juego
    connect(timer, &QTimer::timeout, this, [=]() {
        // No hacer nada si no hay jugador o fondo
        if (!jugador || !jugador->getSprite() || !fondoNivel) return;

        // No aplicar movimiento ni física si el jugador está muerto
        if (!estaMuerto) {
            // Movimiento horizontal (A / D)
            if (teclasPresionadas.contains(Qt::Key_A)) {
                jugador->moverIzquierda();
            } else if (teclasPresionadas.contains(Qt::Key_D)) {
                jugador->moverDerecha();
            } else {
                jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
            }

            // Aplicar gravedad y físicas
            jugador->aplicarFisica();
        }

        // Obtener posición actual del jugador
        QVector2D pos = jugador->getPosicion();

        // Scroll lateral: mover el fondo y mantener sprite visual del jugador fijo
        if (pos.x() > 400 && offsetX < 7200) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            if (!estaMuerto)
                jugador->getSprite()->move(400, pos.y());  // Mantener centrado
        } else if (pos.x() < 100 && offsetX > 0) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            if (!estaMuerto)
                jugador->getSprite()->move(100, pos.y());  // Mantener cerca del borde
        } else {
            // Si no hay scroll, actualizar normalmente el sprite
            if (!estaMuerto)
                jugador->actualizar();

            // Usamos método nuevo para corregir scroll visual
            jugador->moverSpriteConOffset(offsetX);
        }

        // === Actualizar enemigos (IA, movimiento, disparo) ===
        for (int i = 0; i < enemigos.size(); ++i) {
            SoldadoPatrullaRoja* e = enemigos[i];
            if (!e || e->estaMuerto()) continue;

            e->actualizar();  // Mueve la lógica primero

            // Usamos método nuevo que corrige su desplazamiento visual
            e->moverSpriteConOffset(offsetX);
        }
        // === Proyectiles (jugador + enemigos) ===
        for (int i = 0; i < proyectiles.size(); ++i) {
            Proyectil* p = proyectiles[i];

            // Validar proyectil nulo
            if (!p) {
                qDebug() << "¡Advertencia: proyectil nulo eliminado! Índice:" << i;
                proyectiles.removeAt(i);
                --i;
                continue;
            }

            QLabel* sprite = p->getSprite();
            if (!sprite) {
                qDebug() << "¡Advertencia: getSprite() devuelve nullptr! Eliminando proyectil índice:" << i;
                delete p;
                proyectiles.removeAt(i);
                --i;
                continue;
            }

            // Actualizar posición lógica
            p->actualizar();
            QVector2D posP = p->getPosicion();

            // Validar que el sprite siga siendo válido antes de mover
            if (sprite) {
                sprite->move(posP.x() - offsetX, posP.y());
            }

            // Rectángulo de colisión
            QRect rectProy(posP.toPoint(), sprite->size());
            bool impacto = false;

            if (p->esDelJugadorFunc()) {
                for (SoldadoPatrullaRoja*& enemigo : enemigos) {
                    if (!enemigo || enemigo->estaMuerto()) continue;
                    QRect rectEnem(enemigo->getPosicion().toPoint(), enemigo->getSprite()->size());
                    if (rectProy.intersects(rectEnem)) {
                        enemigo->recibirDanio(p->getDanio());
                        impacto = true;

                        // Solo si es proyectil tipo "lunch" (bazuka)
                        if (p->getTipo() == "lunch") {
                            // Mostrar explosión
                            mostrarExplosion(enemigo->getPosicion());

                            // Sonido de explosión bazuka
                            QMediaPlayer* efecto = new QMediaPlayer(this);
                            QAudioOutput* salida = new QAudioOutput(this);
                            efecto->setAudioOutput(salida);
                            efecto->setSource(QUrl("qrc:/Sonidos/Nivel1-S/bazuka-efecto.mp3")); // mismo que en nivel2
                            efecto->play();

                            connect(efecto, &QMediaPlayer::mediaStatusChanged, efecto, [=](QMediaPlayer::MediaStatus status) {
                                if (status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
                                    efecto->deleteLater();
                                    salida->deleteLater();
                                }
                            });
                        }

                        break;
                    }
                }
            } else {
                // Colisión con jugador
                QRect rectJugador(jugador->getPosicion().toPoint(), jugador->getSprite()->size());
                if (rectProy.intersects(rectJugador)) {
                    if (!estaMuerto) {
                        vidas--;
                        actualizarCorazones(vidas);
                        if (vidas <= 0) {
                            timer->stop();
                            mostrarMensajeDerrota();
                        }
                    }
                    impacto = true;
                }
            }

            // --- Eliminar proyectil si impactó o salió del nivel ---
            if (impacto || posP.x() < -100 || posP.x() > 8200) {
                if (sprite) sprite->hide();  // Ocultar sprite si existe
                delete p;                    // Liberar proyectil completamente
                proyectiles.removeAt(i);
                --i;
            }
        }

        // === Limpiar enemigos eliminados (muertos + ocultos) ===
        for (int i = 0; i < enemigos.size(); ++i) {
            SoldadoPatrullaRoja* e = enemigos[i];
            if (!e) continue;
            if (e->estaMuerto() && (!e->getSprite() || !e->getSprite()->isVisible())) {
                enemigos.removeAt(i);
                delete e;
                --i;
            }
        }
    });
}

// Evento: tecla presionada
void Nivel1::keyPressEvent(QKeyEvent* event) {
    // Si el jugador está muerto, no procesar ninguna entrada
    if (estaMuerto) return;

    teclasPresionadas.insert(event->key());
    if (!jugador) return;

    if (event->key() == Qt::Key_W) jugador->saltar();
    if (event->key() == Qt::Key_Space) jugador->atacar();

    // Disparo especial con R
    if (event->key() == Qt::Key_R) {
        if (!jugador->estaListoParaDisparar()) return;
        jugador->setPuedeDisparar(false);

        auto* l = dynamic_cast<Lunch*>(jugador);

        if (l && l->estaEnModoAgresiva()) {
            l->disparar();
            QTimer::singleShot(800, this, [=]() { jugador->setPuedeDisparar(true); });

            QTimer::singleShot(550, this, [=]() {
                QVector2D direccion = l->estaMirandoDerecha() ? QVector2D(1, 0) : QVector2D(-1, 0);
                QVector2D posicion = l->getPosicion() + QVector2D(0, 10);
                Proyectil* bala = new Proyectil(this, posicion, direccion, "lunch", 10.0f);
                bala->getSprite()->raise();
                proyectiles.append(bala);
            });
        } else if (!l) {
            jugador->lanzarEnergia();
            QTimer::singleShot(400, this, [=]() { jugador->setPuedeDisparar(true); });

            QVector2D direccion = jugador->estaMirandoDerecha() ? QVector2D(1, 0) : QVector2D(-1, 0);
            QVector2D posicion = jugador->getPosicion() + QVector2D(0, -5);
            Proyectil* bola = new Proyectil(this, posicion, direccion, "roshi", 10.0f);
            bola->getSprite()->raise();
            proyectiles.append(bola);
        }
    }

    // Subfusil con E
    if (event->key() == Qt::Key_E) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l && l->estaEnModoAgresiva()) {
            l->dispararSubfusil();
        }
    }

    // Transformación de Lunch con F
    if (event->key() == Qt::Key_F) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->transformar();
    }

    // Menú de pausa con Esc
    if (event->key() == Qt::Key_Escape) {
        if (!menuPausa || !menuPausa->isVisible()) {
            mostrarMenuPausa();
        } else {
            ocultarMenuPausa();
        }
    }

    // Temporal: recibir daño con tecla K (para testear el sistema de vida)
    if (event->key() == Qt::Key_K) {
        vidas--;
        actualizarCorazones(vidas);
        if (vidas <= 0) {
            timer->stop();
            mostrarMensajeDerrota();
        }
    }
}

// Evento: tecla soltada
void Nivel1::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}

// Agregar proyectiles desde Lunch
void Nivel1::agregarProyectil(Proyectil* p) {
    proyectiles.append(p);
}
void Nivel1::limpiarTodosLosEnemigos() {
    for (int i = 0; i < enemigos.size(); ++i) {
        SoldadoPatrullaRoja* e = enemigos[i];
        if (!e) continue;

        if (e->getSprite()) {
            e->getSprite()->hide();
        } else {
            qDebug() << "Advertencia: sprite de enemigo nulo al limpiar.";
        }

        delete e;
    }
    enemigos.clear(); // Elimina todos los punteros del QVector
}
void Nivel1::limpiarTodosLosProyectiles() {
    for (int i = 0; i < proyectiles.size(); ++i) {
        Proyectil* p = proyectiles[i];
        if (!p) continue;

        if (p->getSprite()) {
            p->getSprite()->hide();
        } else {
            qDebug() << "Advertencia: getSprite() devuelve nullptr al limpiar.";
        }

        delete p;
    }
    proyectiles.clear(); // Vacía el vector para evitar acceso a basura
}

void Nivel1::limpiarCorazones() {
    for (int i = 0; i < corazones.size(); ++i) {
        if (corazones[i]) {
            corazones[i]->deleteLater();
        }
    }
    corazones.clear();
}

void Nivel1::limpiarJugador() {
    if (jugador) {
        delete jugador;
        jugador = nullptr;
    }
}

void Nivel1::limpiarFondo() {
    if (fondoNivel) {
        fondoNivel->deleteLater();
        fondoNivel = nullptr;
    }
}



void Nivel1::actualizarCorazones(int nuevaVida) {
    for (int i = 0; i < corazones.size(); ++i) {
        if (i < nuevaVida) {
            corazones[i]->setPixmap(QPixmap(":/Sprites/UI/corazon_lleno.png").scaled(32, 32));
        } else {
            corazones[i]->setPixmap(QPixmap(":/Sprites/UI/corazon_vacio.png").scaled(32, 32));
        }
    }
}

void Nivel1::detenerMusica() {
    if (reproductorNivel) {
        reproductorNivel->stop();
        reproductorNivel->deleteLater();
        reproductorNivel = nullptr;
    }

    if (salidaAudioNivel) {
        salidaAudioNivel->deleteLater();
        salidaAudioNivel = nullptr;
    }
}

// Reiniciar el nivel (resetea completamente el estado)
void Nivel1::reiniciarNivel() {
    timer->stop();  // Detiene el bucle principal
    limpiarTodosLosProyectiles(); // Elimina proyectiles
    limpiarTodosLosEnemigos();    // Elimina enemigos
    limpiarCorazones();           // Elimina corazones
    limpiarJugador();             // Elimina jugador
    limpiarFondo();               // Elimina fondo

    // Crear nueva instancia
    Nivel1* nuevoNivel = new Nivel1(parentWidget());

    // Conecta la señal base correctamente (importante para volver al menú)
    connect(nuevoNivel, &Nivel::regresarAlMenu, this, [=]() {
        emit regresarAlMenu(); // Propaga correctamente al Juego
    });

    // Mostrar la nueva instancia
    nuevoNivel->setFixedSize(800, 600);
    nuevoNivel->show();
    nuevoNivel->cargarNivel();

    // Eliminar la instancia actual después de pasar el control
    this->deleteLater();
}
