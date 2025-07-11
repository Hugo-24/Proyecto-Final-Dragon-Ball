#include "Juego.h"
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"
#include <QVBoxLayout>
#include <QDebug>

Juego::Juego(QWidget *parent)
    : QWidget(parent), nivelActual(nullptr)
{
    // Inicialmente no se crean niveles, se crean dinámicamente en cambiarNivel
}

/**
 * Cambia al nivel especificado por id (por ejemplo: "nivel1", "nivel2", "nivel3")
 */
void Juego::cambiarNivel(const std::string& id) {
    // Elimina el nivel actual si ya existe
    if (nivelActual) {
        layout()->removeWidget(nivelActual);
        nivelActual->hide();
        nivelActual->deleteLater();
        nivelActual = nullptr;
    }

    // Crear el nuevo nivel según el id
    if (id == "nivel1") {
        nivelActual = new Nivel1(this);
    } else if (id == "nivel2") {
        nivelActual = new Nivel2(this);
    } else if (id == "nivel3") { // ✅ Ahora manejamos nivel 3
        nivelActual = new Nivel3(this);
    } else {
        qDebug() << "Nivel no reconocido:" << QString::fromStdString(id);
        return;
    }

    // Conectamos la señal para volver al menú si se usa
    connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
        emit regresarAlMenu();
    });

    // Lógica de inicialización
    nivelActual->cargarNivel();

    // Si no existe layout, lo creamos
    if (!layout()) {
        QVBoxLayout* l = new QVBoxLayout(this);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(0);
        setLayout(l);
    }

    layout()->addWidget(nivelActual);
    nivelActual->show();
}
