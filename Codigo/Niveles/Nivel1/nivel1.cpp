#include "nivel1.h"
#include "roshi.h"
#include "lunch.h"
#include "proyectil.h"
#include "corazon.h"
#include "soldadopatrullarroja.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QLabel>
#include <QTimer>
#include <QDebug>

// Constructor del nivel: configura el tamaño de la ventana, foco de teclado y objetos clave.
Nivel1::Nivel1(QWidget* parent)
    : Nivel(parent),
    jugador(nullptr),
    fondoNivel(nullptr),
    fondoSeleccion(new QLabel(this)),
    selector(new QWidget(this)),
    offsetX(0),
    enemigos(),
    timer(new QTimer(this))
{
    setFixedSize(800, 600);                 // Dimensiones fijas de la escena
    setFocusPolicy(Qt::StrongFocus);       // Permitir captura de eventos de teclado
}

// Inicializa la barra de vidas (corazones llenos al comenzar)
void Nivel1::inicializarCorazones(int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        QLabel* corazon = new QLabel(this);
        corazon->setPixmap(QPixmap(":/Sprites/UI/corazon_lleno.png").scaled(32, 32));
        corazon->setGeometry(10 + i * 36, 10, 32, 32);  // Posición con separación
        corazon->show();
        corazones.append(corazon);
    }
}

// Agrega un enemigo en la posición deseada, con dirección inicial
void Nivel1::agregarEnemigoEnPosicion(const QVector2D& pos, bool haciaDerecha) {
    SoldadoPatrullaRoja* enemigo = new SoldadoPatrullaRoja(this, this, pos, jugador, haciaDerecha);
    enemigos.append(enemigo);
}

// Genera los primeros enemigos cerca del jugador (antes del spawn progresivo)
void Nivel1::agregarEnemigosIniciales() {
    for (int i = 0; i < 5; ++i) {
        float x = 400 + i * 50; // Se posicionan en fila cerca del inicio
        bool derecha = (i % 2 == 0); // Alternan dirección inicial
        agregarEnemigoEnPosicion(QVector2D(x, 420), derecha);
        totalEnemigosCreados++;
    }
}

// Inicia el temporizador que genera enemigos en tandas cada 10 segundos
void Nivel1::iniciarSpawningDeEnemigos() {
    timerSpawnEnemigos = new QTimer(this);
    connect(timerSpawnEnemigos, &QTimer::timeout, this, [=]() {
        if (totalEnemigosCreados >= maxEnemigosNivel) {
            timerSpawnEnemigos->stop(); // No generar más si ya se alcanzó el límite
            return;
        }

        // Crea hasta 15 enemigos nuevos por tanda
        int cantidad = qMin(15, maxEnemigosNivel - totalEnemigosCreados);
        for (int i = 0; i < cantidad; ++i) {
            float xRandom = 500 + rand() % 7000;  // Posición horizontal aleatoria
            bool derecha = (rand() % 2 == 0);
            agregarEnemigoEnPosicion(QVector2D(xRandom, 420), derecha);
            totalEnemigosCreados++;
        }
    });

    timerSpawnEnemigos->start(10000); // Repite cada 10 segundos
}

// Muestra una explosión visual en la posición indicada, luego la elimina
void Nivel1::mostrarExplosion(const QVector2D& pos) {
    QLabel* expl = new QLabel(this); // Crea un nuevo QLabel para representar la explosión
    expl->setPixmap(QPixmap(":/Sprites/UI/explosion.png").scaled(80, 80)); // Asigna el sprite
    expl->move(pos.x() - offsetX, pos.y() - 20); // Posiciona con corrección de scroll
    expl->show();
    expl->raise(); // Asegura que se vea encima

    // Oculta y elimina la explosión tras 500 ms
    QTimer::singleShot(500, this, [expl]() {
        expl->hide();
        expl->deleteLater();
    });
}

// Muestra secuencia de derrota al perder todas las vidas
void Nivel1::mostrarMensajeDerrota() {
    estaMuerto = true;

    // Detiene animación y marca como muerto al jugador
    if (jugador) {
        jugador->detenerAnimacionCaminar();
        jugador->setMuerto(true);
    }

    // Oculta contador de enemigos
    if (contadorEnemigosLabel) {
        contadorEnemigosLabel->hide();
        contadorEnemigosLabel->deleteLater();
        contadorEnemigosLabel = nullptr;
    }

    // Determina sprite de muerte según el personaje y su dirección
    if (jugador && jugador->getSprite()) {
        QString rutaMuerte;

        if (dynamic_cast<Roshi*>(jugador)) {
            rutaMuerte = jugador->estaMirandoDerecha()
            ? ":/Sprites/Roshi/R_Roshi_Muerto.png"
            : ":/Sprites/Roshi/L_Roshi_Muerto.png";
        } else if (dynamic_cast<Lunch*>(jugador)) {
            rutaMuerte = jugador->estaMirandoDerecha()
            ? ":/Sprites/Lunch/R_Lunch_Muerta.png"
            : ":/Sprites/Lunch/L_Lunch_Muerta.png";
        }

        jugador->getSprite()->setPixmap(QPixmap(rutaMuerte).scaled(100, 100)); // Aplica sprite
    }

    // Sonido de muerte con volumen bajo
    detenerMusica();
    QMediaPlayer* efectoMuerte = new QMediaPlayer(this);
    QAudioOutput* salidaMuerte = new QAudioOutput(this);
    efectoMuerte->setAudioOutput(salidaMuerte);
    efectoMuerte->setSource(QUrl("qrc:/Sonidos/Nivel1-S/muerte-sonido.mp3"));
    salidaMuerte->setVolume(10);
    efectoMuerte->play();

    // Muestra mensaje de derrota visual centrado
    QLabel* mensaje = new QLabel("\u00a1Has muerto!", this);
    mensaje->setStyleSheet("color: red; font-size: 30px; font-weight: bold;");
    mensaje->adjustSize();
    mensaje->move(width() / 2 - mensaje->width() / 2, height() / 2 - mensaje->height() / 2);
    mensaje->show();

    // Espera 4 segundos antes de reiniciar el nivel
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
    salidaAudioSeleccion->setVolume(10);
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
        // Crear contador visual de enemigos derrotados (solo en juego)
        contadorEnemigosLabel = new QLabel(this);
        contadorEnemigosLabel->setText("Enemigos derrotados: 0 / 30");
        contadorEnemigosLabel->setStyleSheet("color: white; background-color: rgba(0,0,0,150); font: bold 16px; padding: 5px;");
        contadorEnemigosLabel->setGeometry(10, 550, 250, 30);
        contadorEnemigosLabel->show();

        //Set Jugador
        jugador = personaje;
        jugador->setPosicion(QVector2D(100, 420));
        jugador->getSprite()->show();
        jugador->getSprite()->raise();
        //Crear enemigos de la Patrulla Roja
        // Aparecen ya caminando desde el fondo hacia el jugador
        agregarEnemigosIniciales();  // Enemigos iniciales cercanos
        iniciarSpawningDeEnemigos(); // Tandas
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

    //Dialogo explicando el nivel
    QLabel* dialogo = new QLabel(this);
    dialogo->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 180); color: white; font: bold 16px; padding: 10px; border-radius: 5px; }");
    dialogo->setAlignment(Qt::AlignCenter);
    dialogo->setGeometry(100, 450, 600, 60);
    dialogo->show();

    dialogo->setText("Narrador: La Patrulla Roja avanza hacia la Kame House...");

    QTimer::singleShot(3000, this, [=]() {
        dialogo->setText("Maestro Roshi: ¡Debemos detenerlos antes de que sea demasiado tarde!");

        QTimer::singleShot(3000, this, [=]() {
            dialogo->setText("Objetivo: Derrota a 30 enemigos para proteger la Kame House.");
        });
    });

    // Bucle principal del juego
    connect(timer, &QTimer::timeout, this, [=]() {
        // Verificación inicial: si no hay jugador o fondo, salir del bucle
        if (!jugador || !jugador->getSprite() || !fondoNivel) return;

        // === Movimiento y físicas del jugador ===
        if (!estaMuerto) {
            if (teclasPresionadas.contains(Qt::Key_A)) {
                jugador->moverIzquierda();
            } else if (teclasPresionadas.contains(Qt::Key_D)) {
                jugador->moverDerecha();
            } else {
                jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
            }

            jugador->aplicarFisica();
        }

        // === Scroll lateral: mantiene al jugador centrado ===
        QVector2D pos = jugador->getPosicion();
        if (pos.x() > 400 && offsetX < 7200) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            if (!estaMuerto) jugador->getSprite()->move(400, pos.y());
        } else if (pos.x() < 100 && offsetX > 0) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            if (!estaMuerto) jugador->getSprite()->move(100, pos.y());
        } else {
            if (!estaMuerto) jugador->actualizar();
            jugador->moverSpriteConOffset(offsetX);
        }

        // === Enemigos: IA y desplazamiento visual ===
        for (int i = 0; i < enemigos.size(); ++i) {
            SoldadoPatrullaRoja* e = enemigos[i];
            if (!e || e->estaMuerto()) continue;

            e->actualizar();                 // Actualiza lógica del enemigo
            e->moverSpriteConOffset(offsetX); // Mueve sprite con el scroll
        }

        // === Proyectiles: movimiento, colisiones y eliminación ===
        // === Proyectiles: actualización, colisiones y eliminación segura ===
        for (int i = 0; i < proyectiles.size(); ++i) {
            Proyectil* p = proyectiles[i];

            // Validación: proyectil nulo
            if (!p) {
                qDebug() << "¡Advertencia: proyectil nulo eliminado! Índice:" << i;
                proyectiles.removeAt(i--);
                continue;
            }

            QLabel* sprite = p->getSprite();

            // Validación: sprite nulo
            if (!sprite) {
                qDebug() << "¡Advertencia: getSprite() devuelve nullptr! Eliminando proyectil índice:" << i;
                delete p;
                proyectiles.removeAt(i--);
                continue;
            }

            // === Movimiento del proyectil ===
            p->actualizar();  // Actualiza posición lógica
            QVector2D posP = p->getPosicion();
            sprite->move(posP.x() - offsetX, posP.y()); // Corrige scroll visual

            QRect rectProy(posP.toPoint(), sprite->size());
            bool impacto = false;

            // === Colisiones con enemigos (si es del jugador) ===
            if (p->esDelJugadorFunc()) {
                for (SoldadoPatrullaRoja*& enemigo : enemigos) {
                    if (!enemigo || enemigo->estaMuerto()) continue;

                    QLabel* spriteEnem = enemigo->getSprite();
                    if (!spriteEnem) {
                        qDebug() << "Advertencia: sprite de enemigo nulo en colisión.";
                        continue;
                    }

                    QRect rectEnem(enemigo->getPosicion().toPoint(), spriteEnem->size());
                    if (rectProy.intersects(rectEnem)) {
                        int vidaAntes = enemigo->getVida();
                        enemigo->recibirDanio(p->getDanio());
                        impacto = true;

                        // Si es proyectil tipo "lunch" (bazuka), mostrar explosión y sonido
                        if (p->getTipo() == "lunch") {
                            mostrarExplosion(enemigo->getPosicion());

                            QMediaPlayer* efecto = new QMediaPlayer(this);
                            QAudioOutput* salida = new QAudioOutput(this);
                            efecto->setAudioOutput(salida);
                            efecto->setSource(QUrl("qrc:/Sonidos/Nivel1-S/bazuka-efecto.mp3"));
                            efecto->play();
                            connect(efecto, &QMediaPlayer::mediaStatusChanged, efecto, [=](QMediaPlayer::MediaStatus status) {
                                if (status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
                                    efecto->deleteLater();
                                    salida->deleteLater();
                                }
                            });
                        }

                        // === Contador de enemigos derrotados ===
                        if (vidaAntes > 0 && enemigo->getVida() <= 0) {
                            enemigosDerrotados++;
                            if (contadorEnemigosLabel)
                                contadorEnemigosLabel->setText(QString("Enemigos derrotados: %1 / 30").arg(enemigosDerrotados));
                            // Cada 6 enemigos derrotados aparece un Corazon curativo
                            if (enemigosDerrotados % 5 == 0 && enemigosDerrotados < 30) {
                                Corazon* c = new Corazon(this, enemigo->getPosicion(), this);
                                corazonesCurativos.append(c);
                            }
                        }
                        // Si se derrotan 30 enemigos, se gana el nivel
                        if (enemigosDerrotados >= 30 && !reproductorVictoria) {
                            if (timerSpawnEnemigos && timerSpawnEnemigos->isActive()) {
                                timerSpawnEnemigos->stop();  // Detener aparición de enemigos
                            }
                            limpiarTodosLosEnemigos();    // Elimina enemigos
                            limpiarCorazones();           // Elimina corazones
                            //Para quitar el contador
                            if (contadorEnemigosLabel) {
                                contadorEnemigosLabel->hide();
                                contadorEnemigosLabel->deleteLater();
                                contadorEnemigosLabel = nullptr;
                            }
                            // Mensaje visual de victoria
                            QLabel* mensaje = new QLabel("¡Has ganado!\nVenciste al Ejército de la Patrulla Roja\n¡La Kame House está a salvo!", this);
                            mensaje->setStyleSheet("color: white; font-size: 28px; font-weight: bold; background-color: black; padding: 10px; border: 2px solid black;");
                            mensaje->setAlignment(Qt::AlignCenter);
                            mensaje->adjustSize();
                            mensaje->move(width() / 2 - mensaje->width() / 2, height() / 2 - mensaje->height() / 2);
                            mensaje->show();

                            // Sonido de victoria
                            detenerMusica();
                            QMediaPlayer* victoria = new QMediaPlayer(this);
                            QAudioOutput* salida = new QAudioOutput(this);
                            victoria->setAudioOutput(salida);
                            victoria->setSource(QUrl("qrc:/Sonidos/efecto-victoria.mp3"));
                            victoria->play();
                            reproductorVictoria = victoria;
                            salidaAudioVictoria = salida;

                            // Regresar al menú principal tras 8 segundos
                            QTimer::singleShot(8000, this, [this]() {
                                emit regresarAlMenu();
                            });
                        }

                        break;
                    }
                }

            } else {
                // === Colisión del proyectil enemigo con el jugador ===
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

            // === Eliminación del proyectil si impactó o salió del mapa ===
            if (impacto || posP.x() < -100 || posP.x() > 8200) {
                sprite->hide(); // Validado previamente que no es nullptr
                delete p;
                proyectiles.removeAt(i--);
            }
        }
        // === Verificar colisión con corazones curativos ===
        for (int i = 0; i < corazonesCurativos.size(); ++i) {
            Corazon* c = corazonesCurativos[i];
            if (!c || !c->getSprite() || !c->getSprite()->isVisible()) continue;

            QRect rectC(c->getSprite()->pos(), c->getSprite()->size());
            QRect rectJugador(jugador->getSprite()->pos(), jugador->getSprite()->size());

            if (rectC.intersects(rectJugador) && vidas < 5) {
                c->getSprite()->hide();
                vidas++;
                actualizarCorazones(vidas);
                corazonesCurativos.removeAt(i--);
                delete c;
            } else {
                c->moverSpriteConOffset(offsetX); // Corrige scroll visual
            }
        }

        // === Eliminar enemigos muertos y ocultos ===
        for (int i = 0; i < enemigos.size(); ++i) {
            SoldadoPatrullaRoja* e = enemigos[i];
            if (!e || (e->estaMuerto() && (!e->getSprite() || !e->getSprite()->isVisible()))) {
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
            // Sonido disparo Roshi
            QMediaPlayer* sfxRoshi = new QMediaPlayer(this);
            QAudioOutput* outRoshi = new QAudioOutput(this);
            sfxRoshi->setAudioOutput(outRoshi);
            sfxRoshi->setSource(QUrl("qrc:/Sonidos/Nivel1-S/disparo-roshi.mp3"));
            outRoshi->setVolume(80);
            sfxRoshi->play();
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
// Elimina todos los enemigos del nivel, ocultando primero sus sprites si existen.
// Se asegura de evitar crashes por enemigos nulos o sin sprite.
void Nivel1::limpiarTodosLosEnemigos() {
    for (SoldadoPatrullaRoja*& e : enemigos) {
        if (e) delete e;
    }
    enemigos.clear();
}
// Elimina todos los proyectiles activos del nivel, validando punteros antes de actuar.
// Se evita acceder a sprites nulos para prevenir crashes.
void Nivel1::limpiarTodosLosProyectiles() {
    for (Proyectil*& p : proyectiles) {
        if (p) delete p;
    }
    proyectiles.clear();
}

// Elimina todos los corazones del HUD (barra de vidas) con deleteLater para evitar fugas.
// Este método solo se encarga de los corazones visuales (no los curativos).
void Nivel1::limpiarCorazones() {
    for (int i = 0; i < corazones.size(); ++i) {
        if (corazones[i]) {
            corazones[i]->deleteLater(); // Elimina de forma segura con Qt
        }
    }
    corazones.clear();
}

// Libera al jugador actual y pone el puntero a nullptr
void Nivel1::limpiarJugador() {
    if (jugador) {
        delete jugador;
        jugador = nullptr;
    }
}

// Libera el widget de fondo del nivel si existe
void Nivel1::limpiarFondo() {
    if (fondoNivel) {
        fondoNivel->deleteLater(); // Qt se encarga de eliminar sus hijos
        fondoNivel = nullptr;
    }
}

// Actualiza la barra de corazones según la vida actual del jugador.
// Muestra corazones llenos hasta nuevaVida, el resto vacíos.
void Nivel1::actualizarCorazones(int nuevaVida) {
    for (int i = 0; i < corazones.size(); ++i) {
        if (i < nuevaVida) {
            corazones[i]->setPixmap(QPixmap(":/Sprites/UI/corazon_lleno.png").scaled(32, 32));
        } else {
            corazones[i]->setPixmap(QPixmap(":/Sprites/UI/corazon_vacio.png").scaled(32, 32));
        }
    }
}

// Detiene la música de fondo del nivel y libera reproductores
void Nivel1::detenerMusica() {
    if (reproductorNivel) {
        reproductorNivel->stop();
        reproductorNivel->deleteLater(); // Elimina el objeto QMediaPlayer
        reproductorNivel = nullptr;
    }

    if (salidaAudioNivel) {
        salidaAudioNivel->deleteLater(); // Elimina la salida de audio
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
    estaMuerto = false;          //Porque no esta muerto

    emit reiniciarEsteNivel(); // <- Deja que el contenedor (como Juego) se encargue de la nueva instancia
}
