#ifndef LUNCH_H
#define LUNCH_H

#include "personaje.h"
#include <QTimer>

/**
 * Clase Lunch: personaje con transformación entre modos.
 * En modo agresiva puede atacar con bazuca o subfusil.
 */
class Lunch : public Personaje {
public:
    Lunch(QWidget* parent);

    void atacar() override;
    void disparar();
    void comenzarRafaga();  // Disparo con subfusil (sostener tecla)
    void detenerRafaga();   // Se suelta la tecla E
    void transformar();     // Alternar entre modos
    bool estaEnModoAgresiva() const;

protected:
    void actualizarSprite() override;

private:
    bool transformada;
    bool rafagaActiva;
    QTimer* timerRafaga;
    QWidget* contenedor; // Para lanzar proyectiles en la escenaz
};

#endif // LUNCH_H
