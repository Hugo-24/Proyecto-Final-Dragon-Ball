#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include <QTimer>

/**
 * Clase Goku: personaje jugable exclusivo del Nivel3.
 * Puede moverse, saltar y lanzar una bola de energía.
 */
class Goku : public Personaje {
public:
    explicit Goku(QWidget* parent);

    void lanzarEnergia() override;     // Sobrescribe el ataque de energía
    void atacar() override;            // También tiene ataque cuerpo a cuerpo (placeholder)
    ~Goku();

    void setPuedeMoverse(bool valor);  // Permite bloquear su movimiento (útil en cinemáticas)
    bool getPuedeMoverse() const;      // Consulta si se puede mover

protected:
    void actualizarSprite() override;  // Actualiza sprite según dirección y movimiento

private:
    QWidget* contenedor = nullptr;     // Referencia al nivel que lo contiene (Nivel3)
    bool puedeMoverse = true;          // Controla si Goku puede moverse
};

#endif // GOKU_H
