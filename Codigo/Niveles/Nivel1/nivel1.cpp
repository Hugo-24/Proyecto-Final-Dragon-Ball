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

        offsetX = 0;
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
        if (!jugador || !jugador->getSprite() || !fondoNivel) return;

        // Movimiento horizontal
        if (teclasPresionadas.contains(Qt::Key_A)) {
            jugador->moverIzquierda();
        } else if (teclasPresionadas.contains(Qt::Key_D)) {
            jugador->moverDerecha();
        } else {
            jugador->setVelocidad(QVector2D(0, jugador->getVelocidad().y()));
        }

        // Aplicar gravedad y físicas
        jugador->aplicarFisica();
        QVector2D pos = jugador->getPosicion();

        // Scroll lateral
        if (pos.x() > 400 && offsetX < 7200) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(400, pos.y());
        } else if (pos.x() < 100 && offsetX > 0) {
            offsetX += jugador->getVelocidad().x();
            fondoNivel->move(-offsetX, 0);
            jugador->getSprite()->move(100, pos.y());
        } else {
            jugador->actualizar();
            QVector2D posJugador = jugador->getPosicion();
            jugador->getSprite()->move(posJugador.x() - offsetX, posJugador.y());
        }

        // Actualizar proyectiles
        for (int i = 0; i < proyectiles.size(); ++i) {
            Proyectil* p = proyectiles[i];
            p->actualizar();
            QVector2D pos = p->getPosicion();
            p->getSprite()->move(pos.x() - offsetX, pos.y());

            if (pos.x() < -100 || pos.x() > 8200) {
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

    // Subfusil: tecla E
    if (event->key() == Qt::Key_E) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l && l->estaEnModoAgresiva()) {
            l->dispararSubfusil();
        }
    }

    // Transformación de Launch
    if (event->key() == Qt::Key_F) {
        auto* l = dynamic_cast<Lunch*>(jugador);
        if (l) l->transformar();
    }

    // NUEVO: Menú de pausa con Esc
    if (event->key() == Qt::Key_Escape) {
        if (!menuPausa || !menuPausa->isVisible()) {
            mostrarMenuPausa();
        } else {
            ocultarMenuPausa();
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

// Reiniciar el nivel (resetea completamente el estado)
void Nivel1::reiniciarNivel() {
    timer->stop();
    this->close(); // Cierra la instancia actual

    // Crear nueva instancia
    Nivel1* nuevoNivel = new Nivel1(parentWidget());

    // Conecta la señal base correctamente
    connect(nuevoNivel, &Nivel::regresarAlMenu, this, [=]() {
        emit regresarAlMenu(); // Propaga correctamente al Juego
    });

    // Mostrar nueva instancia y cargar el nivel
    nuevoNivel->setFixedSize(800, 600);
    nuevoNivel->show();
    nuevoNivel->cargarNivel();
}
