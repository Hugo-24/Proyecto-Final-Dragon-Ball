#include "nivel1.h"
#include "roshi.h"
#include "lunch.h"
#include "proyectil.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QLabel>
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
        jugador->setPosicion(QVector2D(100, 450));
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

    // Lógica principal de juego: físicas + scroll lateral
    connect(timer, &QTimer::timeout, this, [=]() {
        if (!jugador || !jugador->getSprite() || !fondoNivel) return;

        // Movimiento horizontal: A y D
        if (teclasPresionadas.contains(Qt::Key_A)) {
            jugador->moverIzquierda();
        } else if (teclasPresionadas.contains(Qt::Key_D)) {
            jugador->moverDerecha();
        } else {
            jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
        }

        // Física simple (gravedad)
        jugador->aplicarFisica();

        // Posición lógica del personaje
        QVector2D pos = jugador->getPosicion();

        // Scroll lateral hacia la derecha
        if (pos.x() > 400 && offsetX < 7200) { // 8000 - 800
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(400, pos.y());
        }
        // Scroll lateral hacia la izquierda
        else if (pos.x() < 100 && offsetX > 0) {
            offsetX += jugador->getVelocidad().x(); // negativo
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(100, pos.y());
        } else {
            jugador->actualizar(); // sin scroll, actualización normal
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
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) {
            l->disparar();
            // Crear proyectil de Lunch
            auto direccion = l->estaMirandoDerecha() ? QVector2D(1, 0) : QVector2D(-1, 0);
            auto posicion = l->getPosicion() + QVector2D(50 * direccion.x(), 0);
            auto* bala = new Proyectil(this, posicion, direccion, "lunch");
            bala->getSprite()->raise();
        } else {
            jugador->lanzarEnergia();
            // Crear proyectil de Roshi
            auto direccion = jugador->estaMirandoDerecha() ? QVector2D(1, 0) : QVector2D(-1, 0);
            auto posicion = jugador->getPosicion() + QVector2D(50 * direccion.x(), 0);
            auto* bola = new Proyectil(this, posicion, direccion, "roshi");
            bola->getSprite()->raise();
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
