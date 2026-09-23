# Mazmorra Slime

Juego 2D de acción/plataformas en C++17 + SFML.

Incluye:
- protagonista humano/slime;
- Espíritu, Fuego, Agua y Planta;
- 10 niveles con enemigos diferentes;
- jefe Golem Ancestral con segunda fase;
- movimiento, salto, ataques, habilidades, partículas, HUD y cámara;
- gráficos creados con formas de SFML, sin sprites externos.

## Controles
A/D mover · W o Espacio saltar · J ataque · K especial · 1-4 poderes · ESC salir · ENTER reiniciar.

## Compilación
Requiere CMake 3.16+ y compilador C++17. SFML 2.6.1 se descarga automáticamente mediante CMake.

cmake -S . -B build
cmake --build build --config Release

En Linux:
./build/mazmorra_slime

En Windows con Visual Studio, usar la configuración Release.

## Niveles
1 Entrada: slimes y murciélagos.
2 Cámaras de fuego: gólems de lava y salamandras.
3 Caverna inundada: pirañas y medusas.
4 Jardín maldito: plantas carnívoras e insectos.
5 Cripta espiritual: fantasmas, espectros y caballeros muertos.
6 Fortaleza goblin: guerreros, arqueros y escudos.
7 Minas abandonadas: murciélagos gigantes, gusanos de roca y mineros.
8 Biblioteca prohibida: libros vivientes, magos oscuros y espíritus de tinta.
9 Sala de guardianes: caballeros oscuros, gólems y magos guardianes.
10 Cámara del Golem: Golem Ancestral y segunda fase.
