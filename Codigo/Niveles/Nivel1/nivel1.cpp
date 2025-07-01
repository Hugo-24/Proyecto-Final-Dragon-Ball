#include "nivel1.h"
#include "roshi.h"
#include "lunch.h"
#include "proyectil.h"

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
    fondoSeleccion(new QLabel(this)),
    selector(new QWidget(this)),
    fondoNivel(nullptr),
    offsetX(0)
{
    setFixedSize(800, 600);
    setFocusPolicy(Qt::StrongFocus); // Capturar teclado
}

// Carga el nivel: muestra selección de personaje y prepara scroll
void Nivel1::cargarNivel() {
    // Fondo visible durante la selección de personaje
    fondoSeleccion->setPixmap(QPixmap(":/Sprites/Fondos/fondo_nivel1.jpg").scaled(800, 600));
    fondoSeleccion->setGeometry(0, 0, 800, 600);
    fondoSeleccion->show();

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

        // Alternar entre dos fondos en cada segmento
        for (int i = 0; i < 4; ++i) {
            QLabel* fondo = new QLabel(fondoNivel);
            QString ruta = (i % 2 == 0)
                               ? ":/Sprites/Fondos/Fondo_scroll_largo.jpg"
                               : ":/Sprites/Fondos/Fondo_scroll_largo_2.jpg";

            fondo->setPixmap(QPixmap(ruta).scaled(2000, 600));
            fondo->setGeometry(i * 2000, 0, 2000, 600);
            fondo->show();
        }

        // Crear al jugador seleccionado
        jugador = personaje;
        jugador->setPosicion(QVector2D(100, 420));
        jugador->getSprite()->show();
        jugador->getSprite()->raise();

        offsetX = 0;
        setFocus();

        // Iniciar el bucle del juego
        if (!timer->isActive()) {
            timer->start(16); // ~60 FPS
        }
    };

    // Selección de personaje: Roshi
    connect(btnRoshi, &QPushButton::clicked, this, [=]() {
        iniciarJuego(new Roshi(this));
    });

    // Selección de personaje: Launch
    connect(btnLunch, &QPushButton::clicked, this, [=]() {
        iniciarJuego(new Lunch(this));
    });

    // Lógica principal de juego: físicas, scroll lateral y proyectiles
    connect(timer, &QTimer::timeout, this, [=]() {
        if (!jugador || !jugador->getSprite() || !fondoNivel) return;

        // Movimiento horizontal
        if (teclasPresionadas.contains(Qt::Key_A)) {
            jugador->moverIzquierda();
        } else if (teclasPresionadas.contains(Qt::Key_D)) {
            jugador->moverDerecha();
        } else {
            jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
        }

        // Aplicar físicas (incluye gravedad y salto)
        jugador->aplicarFisica();

        // Obtener posición lógica del jugador actualizada
        QVector2D pos = jugador->getPosicion();

        // Scroll lateral: mover fondo si el personaje se acerca a los bordes visibles
        if (pos.x() > 400 && offsetX < 7200) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(400, pos.y());  // Visual: mantenerlo en centro
        }
        else if (pos.x() < 100 && offsetX > 0) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(100, pos.y());  // Visual: mantenerlo a la izquierda
        }
        else {
            // Cuando no hay scroll, mover sprite normalmente según posición lógica
            jugador->getSprite()->move(pos.x() - offsetX, pos.y());
        }

        // Actualizar proyectiles
        for (int i = 0; i < proyectiles.size(); ++i) {
            Proyectil* p = proyectiles[i];
            p->actualizar();

            QVector2D posP = p->getPosicion();
            p->getSprite()->move(posP.x() - offsetX, posP.y());

            if (posP.x() < -100 || posP.x() > 8200) {
                p->getSprite()->hide();
                delete p;
                proyectiles.remove(i);
                --i;
            }
        }
    });
}

// Evento: tecla presionada
void Nivel1::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());
    if (!jugador) return;

    if (event->key() == Qt::Key_W) jugador->saltar();
    if (event->key() == Qt::Key_Space) jugador->atacar();

    // Ataques especiales
    if (event->key() == Qt::Key_R) {
        if (!jugador || !jugador->getSprite()) return;

        // Verificamos si está en cooldown
        if (!jugador->estaListoParaDisparar()) return;
        jugador->setPuedeDisparar(false);

        auto* l = dynamic_cast<Lunch*>(jugador);

        if (l && l->estaEnModoAgresiva()) {
            l->disparar();

            // Esperar el tiempo total de la animación para permitir nuevo disparo
            QTimer::singleShot(800, this, [=]() { jugador->setPuedeDisparar(true); });

            // Disparo después del retardo (animación sincronizada)
            QTimer::singleShot(550, this, [=]() {
                QVector2D direccion = l->estaMirandoDerecha() ? QVector2D(1, 0) : QVector2D(-1, 0);
                QVector2D posicion = l->getPosicion() + QVector2D(0, 10);

                Proyectil* bala = new Proyectil(this, posicion, direccion, "lunch", 10.0f);
                bala->getSprite()->raise();
                proyectiles.append(bala);
            });
        }
        else if (!l) {
            jugador->lanzarEnergia();

            QTimer::singleShot(500, this, [=]() { jugador->setPuedeDisparar(true); });

            QVector2D direccion = jugador->estaMirandoDerecha() ? QVector2D(1, 0) : QVector2D(-1, 0);
            QVector2D posicion = jugador->getPosicion() + QVector2D(0, -5);

            Proyectil* bola = new Proyectil(this, posicion, direccion, "roshi", 10.0f);
            bola->getSprite()->raise();
            proyectiles.append(bola);
        }
    }

    // NUEVO: subfusil por toque (E)
    if (event->key() == Qt::Key_E) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l && l->estaEnModoAgresiva()) {
            l->dispararSubfusil(); // Nuevo método de disparo rápido
        }
    }

    // Transformación de Launch
    if (event->key() == Qt::Key_F) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->transformar();
    }
}

// Evento: tecla soltada
void Nivel1::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}

// NUEVO: Método para que Lunch pueda agregar proyectiles a la lista
void Nivel1::agregarProyectil(Proyectil* p) {
    proyectiles.append(p);
}
