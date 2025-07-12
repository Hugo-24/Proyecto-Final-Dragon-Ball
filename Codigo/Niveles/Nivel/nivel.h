#pragma once  // Evita múltiples inclusiones del mismo archivo (alternativa a #ifndef)

// Inclusiones necesarias para elementos gráficos de Qt
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "mapa.h"  // Clase del entorno visual del nivel (scroll, fondo, etc.)

// Clase abstracta que sirve como base para Nivel1, Nivel2, Nivel3
class Nivel : public QWidget
{
    Q_OBJECT  // Macro necesaria para usar señales y slots en clases que heredan de QObject (como QWidget)

public:
    explicit Nivel(QWidget *parent = nullptr); // Constructor con padre opcional
    virtual ~Nivel();                          // Destructor virtual para liberar memoria

    // Método virtual puro que cada nivel debe implementar
    virtual void cargarNivel() = 0;

    // Método virtual para reiniciar el estado del nivel (opcionalmente redefinible)
    virtual void reiniciarNivel();

    // Método virtual para detener música (si el nivel tiene música)
    virtual void detenerMusica() {}

protected:
    // Mapa del nivel (fondo visual, puede tener scroll o ser estático)
    Mapa *mapa;

    // Menú de pausa y sus botones
    QWidget* menuPausa = nullptr;
    QPushButton* btnReiniciar = nullptr;
    QPushButton* btnVolverAlMenu = nullptr;

    // Métodos para mostrar u ocultar el menú de pausa
    void mostrarMenuPausa();
    void ocultarMenuPausa();

signals:
    // Señales que los niveles pueden emitir (se conectan desde el exterior)
    void regresarAlMenu();       // Para volver al menú principal
    void reiniciarEsteNivel();   // Para reiniciar el nivel actual
};
