#include "nivel1.h"
#include "mapa.h"
#include "roshi.h"
#include "lunch.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QLabel>
#include <QDebug>

Nivel1::Nivel1(QWidget* parent)
    : Nivel(parent),
    jugador(nullptr),
    offsetX(0),
    fondoNivel(nullptr),
    mapa1(nullptr),
    mapa2(nullptr),
    fondoSeleccion(new QLabel(this)),
    selector(new QWidget(this)),
    timer(new QTimer(this))
{
    setFixedSize(800, 600);
    setFocusPolicy(Qt::StrongFocus); // Capturar teclado
}

void Nivel1::cargarNivel() {
    // Fondo visible durante la selección de personaje
    fondoSeleccion->setPixmap(QPixmap(":/Sprites/Fondos/fondo_nivel1.jpg").scaled(800, 600));
    fondoSeleccion->setGeometry(0, 0, 800, 600);
    fondoSeleccion->show();

    // Contenedor para los botones de personaje y mensaje
    selector->setGeometry(0, 0, 800, 600);
    QVBoxLayout* layout = new QVBoxLayout(selector);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 124, 0, 0); // separarlo un poco desde arriba

    // Texto introductorio
    QLabel* texto = new QLabel("¡Defiende la Kame House del ataque de la Patrulla Roja!\nSelecciona un personaje:", this);
    texto->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    texto->setAlignment(Qt::AlignCenter);
    texto->setWordWrap(true);

    // Botones
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

    // Función común para iniciar el juego con un personaje
    auto iniciarJuego = [&](Personaje* personaje) {
        fondoSeleccion->hide();
        selector->hide();

        // Contenedor para 4 fondos encadenados (4 * 2000 = 8000 px)
        fondoNivel = new QWidget(this);
        fondoNivel->setGeometry(0, 0, 8000, 600);
        fondoNivel->show();

        // Cargar 4 mapas alternando entre fondo1 y fondo2
        for (int i = 0; i < 4; ++i) {
            QLabel* fondo = new QLabel(fondoNivel);
            QString ruta = (i % 2 == 0)
                               ? ":/Sprites/Fondos/Fondo_scroll_largo.jpg"
                               : ":/Sprites/Fondos/Fondo_scroll_largo_2.jpg";

            fondo->setPixmap(QPixmap(ruta).scaled(2000, 600));
            fondo->setGeometry(i * 2000, 0, 2000, 600);
            fondo->show();
        }

        // Crear jugador
        jugador = personaje;
        jugador->setPosicion(QVector2D(100, 450));
        jugador->getSprite()->show();
        jugador->getSprite()->raise();

        offsetX = 0;
        setFocus();

        if (!timer->isActive()) {
            timer->start(16); // 60 FPS
        }
    };

    // Botones para seleccionar personaje
    connect(btnRoshi, &QPushButton::clicked, this, [=]() {
        iniciarJuego(new Roshi(this));
    });

    connect(btnLunch, &QPushButton::clicked, this, [=]() {
        iniciarJuego(new Lunch(this));
    });

    // Timer principal: controla físicas y scroll
    connect(timer, &QTimer::timeout, this, [=]() {
        if (!jugador || !jugador->getSprite() || !fondoNivel) return;

        // Movimiento horizontal con A y D
        if (teclasPresionadas.contains(Qt::Key_A)) {
            jugador->moverIzquierda();
        } else if (teclasPresionadas.contains(Qt::Key_D)) {
            jugador->moverDerecha();
        } else {
            jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
        }

        // Aplicar gravedad
        jugador->aplicarFisica();

        // Obtener posición actual
        QVector2D pos = jugador->getPosicion();

        // Desplazamiento lateral (scroll automático)
        if (pos.x() > 400 && offsetX < 7200) { // 8000 - 800 = 7200
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(400, pos.y());
        }
        else if (pos.x() < 100 && offsetX > 0) {
            offsetX += jugador->getVelocidad().x(); // negativo
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(100, pos.y());
        }
        else {
            jugador->actualizar(); // sin scroll, se actualiza normal
        }
    });
}

// Tecla presionada
void Nivel1::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());
    if (!jugador) return;

    if (event->key() == Qt::Key_W) jugador->saltar();
    if (event->key() == Qt::Key_Space) jugador->atacar();

    // Ataques especiales
    if (event->key() == Qt::Key_R) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->disparar();
        else jugador->lanzarEnergia();
    }

    // Transformación de Lunch
    if (event->key() == Qt::Key_F) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->transformar();
    }
}

// Tecla soltada
void Nivel1::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}
