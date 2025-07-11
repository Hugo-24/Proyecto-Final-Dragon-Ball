#include "menu_principal.h"
#include <QVBoxLayout>

/**
 * Constructor del menú principal. Configura botones y vista de escena.
 */
MenuPrincipal::MenuPrincipal(QWidget* parent) : QWidget(parent) {
    // Tamaño inicial del menú
    resize(400, 400);

    // Layout vertical principal
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Botones para cada nivel
    QPushButton* btn1 = new QPushButton("Jugar Nivel 1");
    QPushButton* btn2 = new QPushButton("Jugar Nivel 2");
    QPushButton* btn3 = new QPushButton("Jugar Nivel 3");  // Nuevo botón para Nivel 3

    // Agregar botones al layout
    layout->addWidget(btn1);
    layout->addWidget(btn2);
    layout->addWidget(btn3); //  Agregado al layout

    // Crear escena y vista gráfica (si estás usando gráficos en otra parte)
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    layout->addWidget(view);

    // Crear instancia del juego
    juego = new Juego(this);

    // Conectar señales a sus respectivos slots
    connect(btn1, &QPushButton::clicked, this, &MenuPrincipal::jugarNivel1);
    connect(btn2, &QPushButton::clicked, this, &MenuPrincipal::jugarNivel2);
    connect(btn3, &QPushButton::clicked, this, &MenuPrincipal::jugarNivel3); //  Conexión para el nuevo botón
}

/**
 * Cambia al nivel 1 dentro del juego.
 */
void MenuPrincipal::jugarNivel1() {
    juego->cambiarNivel("nivel1");
}

/**
 * Cambia al nivel 2 dentro del juego.
 */
void MenuPrincipal::jugarNivel2() {
    juego->cambiarNivel("nivel2");
}

/**
 * Cambia al nivel 3 dentro del juego.
 */
void MenuPrincipal::jugarNivel3() {
    juego->cambiarNivel("nivel3");
}
