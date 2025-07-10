# Dragon Ball - Defensa de la Kame House

**Videojuego 2D** desarrollado en **C++ con Qt** como proyecto académico. Inspirado en la saga de la Patrulla Roja de *Dragon Ball*, permite al jugador elegir entre **Maestro Roshi** o **Lunch** para defender la Kame House de los soldados del Ejército de la Patrulla Roja.

## Descripción general

Este videojuego combina elementos de **scroll lateral, combate beat-em-up, plataformas y narrativa interactiva**, basado en capítulos originales de *Dragon Ball*. A lo largo del juego, se enfrentan hordas de enemigos, se lanzan proyectiles, se recolectan corazones curativos, y se navega por diferentes niveles, incluyendo un escenario submarino.

## Características principales

- Scroll lateral fluido con múltiples niveles conectados.
- Elección de personaje: Maestro Roshi o Lunch (modo pasivo/agresivo).
- Enemigos con IA simple, ataque a distancia y detección de jugador.
- Sistema de proyectiles (jugador/enemigo).
- Objetos curativos que aparecen cada ciertos enemigos derrotados.
- Música de fondo y efectos de sonido contextuales.
- Final con mensaje de victoria/derrota y retorno al menú.
- Narrativa inicial con diálogos y objetivo claro.
- Compatible con Windows (requiere Qt5+ y C++11 o superior).

## Niveles incluidos

### Nivel 1: Defensa de la Kame House
- Estilo: Beat-em-up con scroll horizontal.
- Objetivo: Derrotar a **30 enemigos** para proteger la isla.
- Enemigos aparecen en tandas cada 10 segundos.
- Corazones curativos aparecen cada 6 enemigos eliminados.

### Nivel 2: Aventura Submarina
- Estilo: Desplazamiento lateral y navegación.
- Mecánicas: Movimiento por inercia, combate submarino, y enemigos que abordan.
- Objetivo: Llegar al final del mapa evitando minas, proyectiles y enemigos.

### Nivel 3: (Planeado)
- Combate final contra el General Blue (jefe con ataques especiales).

## Controles

| Acción                | Tecla      |
|----------------------|------------|
| Moverse izquierda    | `A`        |
| Moverse derecha      | `D`        |
| Saltar               | `W`        |
| Atacar               | `Espace`        |
| Disparar (Lunch/Roshi) | `R/E`      |
| Transformar (Lunch)  | `F`        |
| Pausar juego         | `ESC`        |

>  Nota: Los controles pueden ajustarse en el código (`nivel1.cpp`, `nivel2.cpp`).

## Requisitos

- **Sistema operativo:** Windows (probado en Windows 10)
- **Qt:** Qt 5.15+ (con `QWidgets`, `QMediaPlayer`, `QTimer`, `QVector2D`)
- **Compilador:** MinGW o MSVC compatible con C++11
- **IDE sugerido:** Qt Creator
