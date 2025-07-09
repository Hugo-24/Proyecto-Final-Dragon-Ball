#include "nivel.h"
#include <QTimer>

Nivel::Nivel(QWidget* parent) : QWidget(parent), mapa(nullptr) {}

Nivel::~Nivel() {
    if (mapa) delete mapa;
}

void Nivel::reiniciarNivel() {
    emit reiniciarEsteNivel();
}
void Nivel::mostrarMenuPausa() {
    if (!menuPausa) {
        menuPausa = new QWidget(this);
        menuPausa->setGeometry(0, 0, 800, 600);
        menuPausa->setStyleSheet("background-color: rgba(0, 0, 0, 150);");

        QVBoxLayout* layout = new QVBoxLayout(menuPausa);
        layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        layout->setContentsMargins(0, 150, 0, 0);
        layout->setSpacing(20);

        QLabel* titulo = new QLabel("Juego en Pausa", menuPausa);
        titulo->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
        titulo->setAlignment(Qt::AlignCenter);
        layout->addWidget(titulo);

        btnReiniciar = new QPushButton("Reiniciar Nivel", menuPausa);
        btnVolverAlMenu = new QPushButton("Volver al Menú Principal", menuPausa);

        for (QPushButton* btn : {btnReiniciar, btnVolverAlMenu}) {
            btn->setFixedSize(300, 60);
            btn->setStyleSheet("color: black; background-color: white; border: 2px solid black;");
            QFont font = btn->font();
            font.setPointSize(16);
            btn->setFont(font);
            layout->addWidget(btn);
        }

        connect(btnReiniciar, &QPushButton::clicked, this, [=]() {
            ocultarMenuPausa();
            detenerMusica();
            reiniciarNivel();
        });

        connect(btnVolverAlMenu, &QPushButton::clicked, this, [=]() {
            ocultarMenuPausa();
            detenerMusica();         // <<< Detenemos la música antes de salir
            emit regresarAlMenu();   // <<< Señal para regresar
        });

        menuPausa->setLayout(layout);
    }

    menuPausa->show();
}

void Nivel::ocultarMenuPausa() {
    if (menuPausa) menuPausa->hide();
}
