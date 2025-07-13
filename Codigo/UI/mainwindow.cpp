#include "MainWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QPixmap>

/**
 * Constructor de la ventana principal.
 * Aquí se configura el menú inicial con botones para iniciar los niveles.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // === Widget principal que contendrá el menú ===
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central); // Layout vertical

    // === Imagen de fondo del menú ===
    QLabel *fondo = new QLabel(this);
    fondo->setPixmap(QPixmap(":/Sprites/Fondos/fondo_menu.jpg").scaled(800, 600)); // Carga y escala el fondo
    fondo->setScaledContents(true); // Escala el contenido del QLabel
    fondo->setFixedSize(800, 600);  // Tamaño de pantalla
    fondo->lower();                 // Envía el fondo detrás de los botones

    // === Botones para iniciar cada nivel ===
    QPushButton *btnNivel1 = new QPushButton("Iniciar Nivel 1", this);
    QPushButton *btnNivel2 = new QPushButton("Iniciar Nivel 2", this);
    QPushButton *btnNivel3 = new QPushButton("Iniciar Nivel 3", this);

    // Ajustar tamaño de botones
    btnNivel1->setFixedSize(300, 60);
    btnNivel2->setFixedSize(300, 60);
    btnNivel3->setFixedSize(300, 60);

    // Establecer fuente uniforme para todos los botones
    QFont font = btnNivel1->font();
    font.setPointSize(16);
    btnNivel1->setFont(font);
    btnNivel2->setFont(font);
    btnNivel3->setFont(font);

    // Organizar los botones en el layout
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);  // Espaciado entre botones
    layout->addWidget(btnNivel1, 0, Qt::AlignHCenter);
    layout->addWidget(btnNivel2, 0, Qt::AlignHCenter);
    layout->addWidget(btnNivel3, 0, Qt::AlignHCenter);

    central->setLayout(layout);      // Asigna el layout al widget central
    setFixedSize(800, 600);          // Tamaño fijo de la ventana principal
    setCentralWidget(central);       // Establece el widget central de la QMainWindow

    // === Reproductor de música de fondo para el menú ===
    reproductorMenu = new QMediaPlayer(this);       // Crea el reproductor
    salidaAudioMenu = new QAudioOutput(this);       // Crea la salida de audio

    reproductorMenu->setAudioOutput(salidaAudioMenu);       // Asocia la salida
    salidaAudioMenu->setVolume(10);                         // Volumen (0 a 100)
    reproductorMenu->setSource(QUrl("qrc:/Sonidos/Menu/menu-soundtrack.mp3")); // Ruta al audio
    reproductorMenu->setLoops(QMediaPlayer::Infinite);      // Reproduce en bucle
    reproductorMenu->play();                                // Inicia la música

    // === Conectar cada botón a su respectiva función de inicio de nivel ===
    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
    connect(btnNivel3, &QPushButton::clicked, this, &MainWindow::iniciarNivel3);
}

// =================== Funciones de cambio de nivel ======================

/**
 * Inicia el Nivel 1 y reemplaza el menú por el juego.
 */
void MainWindow::iniciarNivel1() {
    if (reproductorMenu) reproductorMenu->stop(); // Detiene la música del menú

    juego = new Juego(this);           // Crea instancia del controlador Juego
    juego->cambiarNivel("nivel1");     // Carga nivel 1

    // Al regresar al menú desde el nivel (cuando se emite regresarAlMenu)
    connect(juego, &Juego::regresarAlMenu, this, [=]() {
        MainWindow* nuevoMenu = new MainWindow(); // Crea un nuevo menú limpio
        nuevoMenu->show();                        // Muestra el menú
        this->close();                            // Cierra el actual
    });

    setCentralWidget(juego); // Reemplaza el menú por el juego
}

/**
 * Inicia el Nivel 2 y reemplaza el menú.
 */
void MainWindow::iniciarNivel2() {
    if (reproductorMenu) reproductorMenu->stop();

    juego = new Juego(this);
    juego->cambiarNivel("nivel2");

    connect(juego, &Juego::regresarAlMenu, this, [=]() {
        MainWindow* nuevoMenu = new MainWindow();
        nuevoMenu->show();
        this->close();
    });

    setCentralWidget(juego);
}

/**
 * Inicia el Nivel 3 y reemplaza el menú.
 */
void MainWindow::iniciarNivel3() {
    if (reproductorMenu) reproductorMenu->stop();

    juego = new Juego(this);
    juego->cambiarNivel("nivel3");

    connect(juego, &Juego::regresarAlMenu, this, [=]() {
        MainWindow* nuevoMenu = new MainWindow();
        nuevoMenu->show();
        this->close();
    });

    setCentralWidget(juego);
}
