# Slime: La Torre Antigua

Proyecto 2D de acción y plataformas en C++17 + SFML 2.6.1.

- Torre de 10 niveles.
- Slime humanoide.
- 4 elementos: Espíritu, Fuego, Agua y Plata.
- Desbloqueos: Espíritu inicial, Fuego nivel 3, Agua nivel 5, Plata nivel 7.
- Enemigos diferenciados y jefe Golem en el nivel 10.
- Cámara lateral, plataformas, proyectiles, habilidades y partículas.
- Funciona sin sprites ni sonidos externos; usa formas de SFML.

## Windows
Requisitos: Visual Studio 2022 con Desktop development with C++, Git y CMake.

    cmake -S . -B build
    cmake --build build --config Release

Ejecutable: .\build\Release\mazmorra_slime.exe

## Controles
A/D mover · W o ESPACIO saltar · J atacar · K habilidad · 1 Espíritu · 2 Fuego · 3 Agua · 4 Plata · ESC salir.
