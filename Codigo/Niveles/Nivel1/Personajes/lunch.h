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
    void disparar();            // Disparo de bazuca (tecla R)
    void dispararSubfusil();    // NUEVO: Disparo individual con subfusil (tecla E)
    void transformar();         // Alternar entre modos
    bool estaEnModoAgresiva() const;
    // No redefine atacar(): Lunch no puede atacar cuerpo a cuerpo

protected:
    void actualizarSprite() override;

private:
    bool transformada;         // true si está en modo agresiva (Launch rubia)
    bool rafagaActiva;         // Ya no se usa, pero lo dejamos por compatibilidad si se desea reactivar
    QTimer* timerRafaga;       // Ya no se usa activamente
    QWidget* contenedor;       // Referencia al contenedor (Nivel1) para lanzar proyectiles en la escena
};

#endif // LUNCH_H
