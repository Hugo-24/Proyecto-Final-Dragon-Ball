#ifndef CORAZON_H
#define CORAZON_H

#include "objeto.h"
#include <QVector2D>

class Nivel1;
class QTimer;

/**
 * Corazon representa un ítem curativo que aparece al matar enemigos.
 * El jugador puede recogerlo al tocarlo. Desaparece tras 10 segundos.
 * Nivel1 se encarga de aplicar curación y eliminarlo.
 */
class Corazon : public Objeto {
public:
    Corazon(QWidget* parent, const QVector2D& posicion, Nivel1* nivel);
    ~Corazon();

    // Marca que fue recogido si colisiona con el jugador
    void interactuar(Entidad* otra) override;

    // Desplaza su sprite con el scroll (no afecta lógica)
    void moverConScroll(float offsetX);

    // Indica si ya fue recogido por el jugador
    bool estaRecogido() const;

    // Indica si debe desaparecer (por tiempo)
    bool debeDesaparecer() const;

private:
    Nivel1* nivel = nullptr;              // Referencia al nivel
    QTimer* timerDesaparicion = nullptr;  // Timer de 10 segundos
    bool recogido = false;                // True si ya fue tomado
    bool expirado = false;                // True si pasaron los 10 segundos
};

#endif // CORAZON_H
