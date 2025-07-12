#include "nivel.h"
#include <QTimer>  // Aunque aquí no se usa directamente, puede ser útil para los hijos

// Constructor. Inicializa el mapa como nulo
Nivel::Nivel(QWidget* parent) : QWidget(parent), mapa(nullptr) {}

// Destructor. Libera memoria asignada al mapa si existe
Nivel::~Nivel() {
    if (mapa) delete mapa;
}

// Reinicia el nivel (emite señal para que quien escuche la reinicie)
void Nivel::reiniciarNivel() {
    emit reiniciarEsteNivel();
}

// Método para crear y mostrar el menú de pausa
void Nivel::mostrarMenuPausa() {
    // Si aún no se ha creado el menú, lo construimos
    if (!menuPausa) {
        menuPausa = new QWidget(this);
        menuPausa->setGeometry(0, 0, 800, 600);  // Tamaño del menú (pantalla completa)
        menuPausa->setStyleSheet("background-color: rgba(0, 0, 0, 150);");  // Fondo semitransparente

        // Layout vertical centrado
        QVBoxLayout* layout = new QVBoxLayout(menuPausa);
        layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        layout->setContentsMargins(0, 150, 0, 0);  // Margen superior para bajar el contenido
        layout->setSpacing(20);

        // Título del menú
        QLabel* titulo = new QLabel("Juego en Pausa", menuPausa);
        titulo->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
        titulo->setAlignment(Qt::AlignCenter);
        layout->addWidget(titulo);

        // Botones del menú
        btnReiniciar = new QPushButton("Reiniciar Nivel", menuPausa);
        btnVolverAlMenu = new QPushButton("Volver al Menú Principal", menuPausa);

        // Estilo y tamaño de los botones
        for (QPushButton* btn : {btnReiniciar, btnVolverAlMenu}) {
            btn->setFixedSize(300, 60);
            btn->setStyleSheet("color: black; background-color: white; border: 2px solid black;");
            QFont font = btn->font();
            font.setPointSize(16);
            btn->setFont(font);
            layout->addWidget(btn);
        }

        // Conectamos el botón de reinicio con lógica:
        // Oculta el menú, detiene música (si aplica) y reinicia el nivel
        connect(btnReiniciar, &QPushButton::clicked, this, [=]() {
            ocultarMenuPausa();
            detenerMusica();
            reiniciarNivel();
        });

        // Conectamos el botón de volver al menú
        connect(btnVolverAlMenu, &QPushButton::clicked, this, [=]() {
            ocultarMenuPausa();
            detenerMusica();         // Detiene la música antes de salir
            emit regresarAlMenu();   // Emite la señal para regresar al menú
        });

        menuPausa->setLayout(layout);  // Se asigna el layout al widget
    }

    menuPausa->show();  // Se muestra el menú si ya estaba creado
}

// Oculta el menú de pausa (sin destruirlo)
void Nivel::ocultarMenuPausa() {
    if (menuPausa) menuPausa->hide();
}
