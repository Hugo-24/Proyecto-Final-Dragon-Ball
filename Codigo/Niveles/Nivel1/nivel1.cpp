#include "nivel1.h"
#include "mapa.h"
#include "roshi.h"
#include "lunch.h"
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

Nivel1::Nivel1(QWidget* parent) : Nivel(parent) {
    jugador = nullptr;
    timer = new QTimer(this);
    setFocusPolicy(Qt::StrongFocus);
}

void Nivel1::cargarNivel() {
    // Fondo del nivel (Kame House y playa)
    mapa = new Mapa(":/Sprites/Fondos/fondo_nivel1.jpg", this);
    mapa->setGeometry(0, 0, 800, 600);
    mapa->show();

    // Layout para mostrar botones sobre el fondo
    QWidget* selector = new QWidget(this);
    selector->setGeometry(0, 0, 800, 600);

    QVBoxLayout* layout = new QVBoxLayout(selector);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);

    // Botón para Roshi
    QPushButton* btnRoshi = new QPushButton("Jugar con Roshi", this);
    btnRoshi->setFixedSize(300, 60);

    // Botón para Launch
    QPushButton* btnLunch = new QPushButton("Jugar con Launch", this);
    btnLunch->setFixedSize(300, 60);

    // Estilo de fuente grande
    QFont font = btnRoshi->font();
    font.setPointSize(16);
    btnRoshi->setFont(font);
    btnLunch->setFont(font);

    layout->addWidget(btnRoshi, 0, Qt::AlignHCenter);
    layout->addWidget(btnLunch, 0, Qt::AlignHCenter);
    selector->setLayout(layout);
    selector->show();

    // Conexión para seleccionar personaje Roshi
    connect(btnRoshi, &QPushButton::clicked, this, [=]() {
        jugador = new Roshi(this);
        jugador->setPosicion(QVector2D(100, 450)); // posición inicial segura
        jugador->actualizar();
        jugador->getSprite()->show();
        jugador->getSprite()->raise();
        selector->hide();
        setFocus();
    });

    // Conexión para seleccionar personaje Launch
    connect(btnLunch, &QPushButton::clicked, this, [=]() {
        jugador = new Lunch(this);
        jugador->setPosicion(QVector2D(100, 450));
        jugador->actualizar();
        jugador->getSprite()->show();
        jugador->getSprite()->raise();
        selector->hide();
        setFocus();
    });

    // Timer del juego (~60 FPS)
    connect(timer, &QTimer::timeout, this, [=]() {
        if (!jugador) return;

        // Movimiento horizontal
        if (teclasPresionadas.contains(Qt::Key_Left)) {
            jugador->moverIzquierda();
        } else if (teclasPresionadas.contains(Qt::Key_Right)) {
            jugador->moverDerecha();
        } else {
            jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
        }

        jugador->aplicarFisica();
        jugador->actualizar();
    });

    timer->start(16); // 60 FPS
}

// Captura de teclas
void Nivel1::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());
    if (!jugador) return;

    if (event->key() == Qt::Key_Up) jugador->saltar();
    if (event->key() == Qt::Key_Space) jugador->atacar();

    if (event->key() == Qt::Key_R) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->disparar();
        else jugador->lanzarEnergia();
    }

    if (event->key() == Qt::Key_F) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->transformar();
    }
}

void Nivel1::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}
