#include "MainWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Widget principal de la ventana
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Fondo del menú principal
    QLabel *fondo = new QLabel(this);
    fondo->setPixmap(QPixmap(":/Sprites/Fondos/fondo_menu.jpg").scaled(800, 600));
    fondo->setScaledContents(true);
    fondo->setFixedSize(800, 600);
    fondo->lower(); // Asegura que el fondo esté detrás de los botones

    // Botones de selección de nivel
    QPushButton *btnNivel1 = new QPushButton("Iniciar Nivel 1", this);
    QPushButton *btnNivel2 = new QPushButton("Iniciar Nivel 2", this);

    btnNivel1->setFixedSize(300, 60);
    btnNivel2->setFixedSize(300, 60);

    QFont font = btnNivel1->font();
    font.setPointSize(16);
    btnNivel1->setFont(font);
    btnNivel2->setFont(font);

    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);
    layout->addWidget(btnNivel1, 0, Qt::AlignHCenter);
    layout->addWidget(btnNivel2, 0, Qt::AlignHCenter);

    central->setLayout(layout);
    setFixedSize(800, 600);
    setCentralWidget(central);

    // Inicializar el reproductor de música del menú
    reproductorMenu = new QMediaPlayer(this);
    salidaAudioMenu = new QAudioOutput(this);

    reproductorMenu->setAudioOutput(salidaAudioMenu);
    salidaAudioMenu->setVolume(50); // Volumen entre 0-100
    reproductorMenu->setSource(QUrl("qrc:/Sonidos/Menu/menu-soundtrack.mp3"));
    reproductorMenu->setLoops(QMediaPlayer::Infinite); // Repetir indefinidamente
    reproductorMenu->play(); // ¡Inicia la música!

    // Conexiones a los botones
    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
}

// Iniciar Nivel 1
void MainWindow::iniciarNivel1() {
    if (reproductorMenu) reproductorMenu->stop(); // Detener música del menú

    juego = new Juego(this);
    juego->cambiarNivel("nivel1");

    // Al regresar al menú desde el nivel
    connect(juego, &Juego::regresarAlMenu, this, [=]() {
        MainWindow* nuevoMenu = new MainWindow();
        nuevoMenu->show();
        this->close(); // Cierra la ventana actual
    });

    setCentralWidget(juego); // Mostrar el nivel
}

// Iniciar Nivel 2
void MainWindow::iniciarNivel2() {
    if (reproductorMenu) reproductorMenu->stop(); // Detener música del menú

    juego = new Juego(this);
    juego->cambiarNivel("nivel2");

    // Al regresar al menú desde el nivel
    connect(juego, &Juego::regresarAlMenu, this, [=]() {
        MainWindow* nuevoMenu = new MainWindow();
        nuevoMenu->show();
        this->close(); // Cierra la ventana actual
    });

    setCentralWidget(juego); // Mostrar el nivel
}
