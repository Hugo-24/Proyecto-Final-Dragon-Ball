#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "mapa.h"

class Nivel : public QWidget
{
    Q_OBJECT

public:
    explicit Nivel(QWidget *parent = nullptr);
    virtual ~Nivel();

    virtual void cargarNivel() = 0; // Cada nivel lo implementa
    virtual void reiniciarNivel();  // Opcional, por si quieres reiniciar estado
    virtual void detenerMusica() {}
protected:
    Mapa *mapa;

    QWidget* menuPausa = nullptr;
    QPushButton* btnReiniciar = nullptr;
    QPushButton* btnVolverAlMenu = nullptr;

    void mostrarMenuPausa();
    void ocultarMenuPausa();

signals:
    void regresarAlMenu();
    void reiniciarEsteNivel();
};
