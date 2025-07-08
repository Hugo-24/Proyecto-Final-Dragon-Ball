#include "entidad.h"

Entidad::Entidad(QWidget* parent) {
    sprite = new QLabel(parent);
    sprite->setScaledContents(true);
    sprite->setFixedSize(64, 64); // Tamaño inicial (puede ajustarse luego)
}

Entidad::~Entidad() {
    if (sprite) {
        sprite->deleteLater();  // Más seguro que delete directo
        sprite = nullptr;
    }
}

// Mueve la posición lógica sumando la velocidad (sin afectar sprite visual)
void Entidad::mover() {
    posicion += velocidad;

    // Límite izquierdo
    if (posicion.x() < 0) {
        posicion.setX(0);
    }

    // Límite derecho (basado en ancho de la escena)
    QWidget* escena = qobject_cast<QWidget*>(sprite->parent());
    if (escena) {
        float limite = escena->width() - sprite->width();
        if (posicion.x() > limite) {
            posicion.setX(limite);
        }
    }
}

// Lógica de movimiento general (llama a mover(); el sprite se mueve en Nivel)
void Entidad::actualizar() {
    mover();  // Actualiza posición lógica solamente
    // El sprite se mueve con moverSpriteConOffset(offsetX) desde el Nivel
}

// Aplica gravedad y controla si está en el suelo (para saltos)
void Entidad::aplicarFisica() {
    QVector2D vel = getVelocidad();
    QVector2D pos = getPosicion();

    vel.setY(vel.y() + 0.8); // gravedad
    pos += vel;

    if (pos.y() > 444) {
        pos.setY(444);
        vel.setY(0);
        enElSuelo = true;
    } else {
        enElSuelo = false;
    }

    setVelocidad(vel);
    setPosicionLogica(pos); // Solo cambia la posición lógica, sin mover sprite visual
}

// Asigna la posición lógica y MUEVE el sprite visual (NO MODIFICAR)
void Entidad::setPosicion(const QVector2D& pos) {
    posicion = pos;
    sprite->move(static_cast<int>(pos.x()), static_cast<int>(pos.y()));
}

// Asigna la posición lógica sin mover el sprite (USAR para enemigos o cuando uses scroll)
void Entidad::setPosicionLogica(const QVector2D& pos) {
    posicion = pos;
    // No se mueve el sprite aquí. Se hará desde Nivel1 usando moverSpriteConOffset()
}

// Devuelve la posición lógica actual
QVector2D Entidad::getPosicion() const {
    return posicion;
}

// Velocidad
void Entidad::setVelocidad(const QVector2D& vel) {
    velocidad = vel;
}

QVector2D Entidad::getVelocidad() const {
    return velocidad;
}

// Devuelve el sprite visual
QLabel* Entidad::getSprite() {
    if (!sprite) {
        qDebug() << "¡Advertencia: getSprite() devuelve nullptr!";
    }
    return sprite;
}

// Cambia la imagen visual del sprite
void Entidad::setSprite(const QString& ruta) {
    QPixmap pix(ruta);
    sprite->setPixmap(pix);       // Asigna imagen
    sprite->resize(pix.size());   // Ajusta tamaño del QLabel
}

// Mueve el sprite en pantalla, corrigiendo con offset de scroll (NO toca posición lógica)
void Entidad::moverSpriteConOffset(float offsetX) {
    if (sprite) {
        sprite->move(posicion.x() - offsetX, posicion.y());
    }
}

// Por defecto no interactúa con nada
void Entidad::interactuar(Entidad* /*otra*/) {
    // Vacío por defecto
}
