#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "personaje.h"
#include "proyectil.h"

#include <QVector>
#include <QTimer>
#include <QSet>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
/**
 * Nivel1 representa el primer nivel del juego, que incluye scroll lateral,
 * selección de personaje (Roshi o Launch) y físicas básicas.
 */
class Nivel1 : public Nivel {
    Q_OBJECT
signals:
    void volverAlMenu(); // Señal para regresar al menú principal
public:
    explicit Nivel1(QWidget* parent = nullptr);
    void cargarNivel() override;

    // Permite que Lunch agregue proyectiles durante disparos
    void agregarProyectil(Proyectil* p);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    Personaje* jugador;           // Personaje activo (Roshi o Lunch)
    QTimer* timer;                // Timer principal (actualiza físicas y scroll)
    QSet<int> teclasPresionadas; // Teclas actualmente presionadas
    QVector<Proyectil*> proyectiles; // Lista de proyectiles activos
    QLabel* fondoSeleccion;      // Fondo visible durante la selección de personaje
    QWidget* selector;           // Contenedor de botones para elegir personaje
    // Soporte para dos fondos continuos en scroll
    QWidget* fondoNivel;         // Contenedor de los fondos (scroll completo)
    QLabel* mapa1;               // Primer fondo (inicio del nivel)
    QLabel* mapa2;               // Segundo fondo (continuación del nivel)
    int offsetX;                  // Desplazamiento horizontal para scroll lateral
    void reiniciarNivel() override;
};

#endif // NIVEL1_H
