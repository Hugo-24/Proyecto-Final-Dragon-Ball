Dragon Ball - Defensa de la Kame House

Videojuego 2D desarrollado en C++ con Qt como proyecto académico. Inspirado en la saga de la Patrulla Roja de Dragon Ball, permite al jugador elegir entre Maestro Roshi o Lunch para defender la Kame House del Ejército de la Patrulla Roja, con un cierre en una batalla contra el General Blue.
Descripción general

Este videojuego combina elementos de scroll lateral, combate beat-em-up, plataformas, disparos y narrativa interactiva, basado en capítulos originales de Dragon Ball. A lo largo del juego, el jugador se enfrenta a enemigos, esquiva obstáculos, lanza proyectiles, recoge corazones curativos, y progresa a través de tres niveles temáticos, cada uno con desafíos únicos.
Características principales

    Scroll lateral fluido en escenarios con y sin desplazamiento.

    Elección de personaje inicial: Maestro Roshi (modo defensivo) o Lunch (modo ofensivo).

    Sistema de combate con proyectiles distintos según personaje.

    Enemigos con comportamiento autónomo, detección de jugador y ataques.

    Corazones curativos que aparecen tras derrotar enemigos o según eventos.

    Música ambiental por nivel y efectos de sonido para ataques y eventos clave.

    HUD con corazones visuales y barra de vida del jefe en el combate final.

    Narrativa breve al inicio de cada nivel, incluyendo diálogos cinemáticos.

    Condición de victoria o derrota con retorno automático al menú.

Niveles incluidos
Nivel 1: Defensa de la Kame House

    Estilo: Beat-em-up con scroll horizontal y múltiples enemigos.

    Objetivo: Derrotar a 30 soldados de la Patrulla Roja para completar el nivel.

    Enemigos: Patrulleros que disparan proyectiles.

    Sistema de corazones curativos: Aparecen cada 6 enemigos derrotados.

    Música de combate y HUD con corazones de vida del jugador.

Nivel 2: Aventura Submarina

    Estilo: Desplazamiento lateral estilo shoot-em-up bajo el agua.

    Mecánicas: Movimiento inercial, disparos submarinos, minas y enemigos.

    Objetivo: Navegar el fondo submarino y sobrevivir al asalto enemigo.

    Oleadas de enemigos aparecen desde la derecha.

    Diálogo narrativo inicial con pausa automática.

    Controla un submarino de Goku.

Nivel 3: Combate contra el General Blue

    Estilo: Combate de jefe en arena estática (cueva).

    Enemigo principal: General Blue con inteligencia artificial.

    Ataques especiales:

        Disparo de subfusil (ráfagas rápidas).

        Lanzamiento de roca gigante (proyectil pesado).

        Telequinesis (Za Warudo): Paraliza al jugador durante 4 segundos (no interrumpible).

    Mecánicas adicionales:

        General Blue lanza corazones curativos cada vez que pierde 1/8 de su vida.

        Salta si el jugador está en el aire.

        Barra de vida visible sobre el jefe.

    Música propia del nivel, efectos especiales y sonido exclusivo para Za Warudo.

    Diálogo de introducción entre Goku y Blue antes del combate.

Controles
Acción	Tecla
Moverse izquierda	A
Moverse derecha	D
Saltar	W
Atacar (cuerpo a cuerpo)	Espacio
Disparar (Lunch/Roshi)	R/E
Transformar (Lunch)	F
Pausar juego	ESC

    Nota: Algunas acciones pueden variar levemente por personaje. Los controles están definidos en nivel1.cpp, nivel2.cpp, nivel3.cpp.

Requisitos

    Sistema operativo: Windows (probado en Windows 10)

    Qt: Qt 5.15 o superior

    Módulos usados: QtWidgets, QtMultimedia, QtCore, QtGui

    Compilador: MinGW o MSVC compatible con C++11 o superior

    IDE sugerido: Qt Creator 4.11+

Compilación y ejecución

    Abrir el proyecto:

        Abre el archivo .pro principal en Qt Creator.

    Configurar el kit de compilación:

        Asegúrate de tener instalado un kit con Qt 5.15+ y un compilador compatible con C++11.

    Compilar:

        Usa el botón Construir proyecto (Ctrl+B).

    Ejecutar:

        Presiona Ejecutar (Ctrl+R) o el botón de Play.

    Asegúrate de que todos los recursos (.png, .mp3, .qml, etc.) estén correctamente integrados en el archivo .qrc del proyecto.

Créditos

    Desarrollo por estudiantes de Ingeniería Electronica - Universidad de Antioquia.

    Inspirado en los capítulos originales del anime Dragon Ball (Saga de la Red Ribbon).

    Recursos visuales y sonoros tomados con fines académicos.


