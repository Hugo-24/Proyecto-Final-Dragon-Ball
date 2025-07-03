#include "MainWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *fondo = new QLabel(this);
    fondo->setPixmap(QPixmap(":/Sprites/Fondos/fondo_menu.jpg").scaled(800, 600));
    fondo->setScaledContents(true);
    fondo->setFixedSize(800, 600);
    fondo->lower();

    QPushButton *btnNivel1 = new QPushButton("Iniciar Nivel 1", this);
    QPushButton *btnNivel2 = new QPushButton("Iniciar Nivel 2", this);

    btnNivel1->setFixedSize(300, 60);
    btnNivel2->setFixedSize(300, 60);

    QFont font = btnNivel1->font();
    font.setPointSize(16);
    btnNivel1->setFont(font);
    btnNivel2->setFont(font);

    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30); // separación entre botones
    layout->addWidget(btnNivel1, 0, Qt::AlignHCenter);
    layout->addWidget(btnNivel2, 0, Qt::AlignHCenter);


    central->setLayout(layout);
    setFixedSize(800, 600);
    setCentralWidget(central);

    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
}

void MainWindow::iniciarNivel1() {
    juego = new Juego(this);
    juego->cambiarNivel("nivel1");
    setCentralWidget(juego);
}

void MainWindow::iniciarNivel2() {
    juego = new Juego(this);
    juego->cambiarNivel("nivel2");

    // Conecta la señal del nivel al slot del MainWindow
    Nivel2* nivel2 = dynamic_cast<Nivel2*>(juego->getNivelActual());
    if (nivel2) {
        connect(nivel2, &Nivel2::regresarAlMenu, this, [this]() {
            this->close();  // Cierra esta ventana MainWindow

            // Reabrimos el menú principal desde cero
            MainWindow* nuevoMenu = new MainWindow();
            nuevoMenu->show();
        });
    }

    setCentralWidget(juego);
}

