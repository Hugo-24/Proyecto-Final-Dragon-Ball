#pragma once
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "juego.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    // Métodos para iniciar niveles, conectados directamente con los botones
    void iniciarNivel1();
    void iniciarNivel2();

private:
    Juego* juego;

    // Atributos para manejar música de fondo del menú
    QMediaPlayer* reproductorMenu;
    QAudioOutput* salidaAudioMenu;
};
