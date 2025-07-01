#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "objeto.h"
#include <QVector2D>

/**
 * @brief Clase Proyectil: representa un disparo lanzado por un personaje (Roshi o Lunch).
 */
class Proyectil : public Objeto {
public:
    /**
     * @brief Constructor del proyectil.
     * @param parent Widget contenedor (escena).
     * @param posicion Posición inicial del proyectil.
     * @param direccion Dirección de movimiento (normalizada).
     * @param tipo Tipo de proyectil: "roshi" o "lunch".
     */
    Proyectil(QWidget* parent, const QVector2D& posicion, const QVector2D& direccion, const QString& tipo);

    /**
     * @brief Lógica de movimiento del proyectil.
     */
    void actualizar() override;

    /**
     * @brief Reacción al colisionar con otra entidad.
     * @param entidad La entidad con la que colisiona.
     */
    void interactuar(Entidad* entidad) override;

private:
    QVector2D direccion;  // Dirección de desplazamiento
    float velocidad;      // Velocidad de movimiento
    QString tipo;         // Tipo del proyectil (para determinar sprite y comportamiento)
};

#endif // PROYECTIL_H
