#pragma once

#include <QWidget>
#include <map>
#include <string>

#include "nivel.h"


/**
 * Clase principal que gestiona qué nivel se está mostrando.
 */
class Juego : public QWidget {
    Q_OBJECT

signals:
    void regresarAlMenu();  // Señal para volver al menú principal

public:
    explicit Juego(QWidget *parent = nullptr);

    void cambiarNivel(const std::string& id);
    Nivel* getNivelActual() const { return nivelActual; }

private:
    std::map<std::string, Nivel*> niveles; // Mapa de niveles por ID
    Nivel* nivelActual = nullptr;          // Nivel actualmente visible
};
