#ifndef LUNCH_H
#define LUNCH_H

#include "personaje.h"
#include <QTimer>

/**
 * Clase Lunch: personaje jugable con transformación entre modos.
 * En modo agresiva puede disparar bazuca o subfusil.
 */
class Lunch : public Personaje {
public:
    Lunch(QWidget* parent);
    ~Lunch();

    void disparar();            // Disparo con bazuca (tecla R)
    void dispararSubfusil();    // Disparo con subfusil (tecla E)
    void transformar();         // Alterna entre modo tranquila y agresiva

    bool estaEnModoAgresiva() const; // Retorna el estado de transformación

protected:
    void actualizarSprite() override; // Actualiza el sprite según el estado y movimiento

private:
    bool transformada;          // true si está en modo agresiva (Launch rubia)
    QWidget* contenedor;        // Referencia al contenedor (Nivel1) para lanzar proyectiles
};

#endif // LUNCH_H
