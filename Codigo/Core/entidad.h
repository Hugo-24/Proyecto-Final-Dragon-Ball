#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QLabel>
#include <QVector2D>
#include <QDebug>

class Entidad {
public:
    Entidad(QWidget* parent);
    virtual ~Entidad();

    virtual void mover();                      // Actualiza posición lógica con velocidad
    virtual void actualizar();                 // Aplica mover(); el sprite se mueve desde Nivel
    virtual void aplicarFisica();              // Gravedad y límites
    virtual void interactuar(Entidad* otra);   // Por defecto, no hace nada

    void setPosicion(const QVector2D& pos);    // Asigna posición lógica Y mueve sprite (NO TOCAR ESTE)
    void setPosicionLogica(const QVector2D& pos); // Asigna solo la posición lógica (NO mueve sprite)
    QVector2D getPosicion() const;

    void setVelocidad(const QVector2D& vel);
    QVector2D getVelocidad() const;

    QLabel* getSprite();
    void setSprite(const QString& ruta);

    void moverSpriteConOffset(float offsetX); // Corrige posición visual según scroll

protected:
    QVector2D posicion;
    QVector2D velocidad;
    QLabel* sprite = nullptr;
    bool enElSuelo = false;
};

#endif // ENTIDAD_H
