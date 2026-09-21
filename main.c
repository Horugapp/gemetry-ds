#include <nds.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// --- Configuración del Juego ---
#define PLAYER_X 40
#define GROUND_Y 140
#define JUMP_FORCE -8
#define GRAVITY 0.5

typedef struct {
    int x;
    int type; // 0 = Bloque, 1 = Pincho
} Obstacle;

Obstacle level1[] = {
    {200, 0}, {400, 1}, {600, 0}, {800, 1}, {1000, 0}, {1200, 1}
};
int level_len = 6;

// --- Estado Global ---
float player_y = GROUND_Y;
float velocity_y = 0;
int scroll_x = 0;
bool is_jumping = false;
bool is_paused = false;
int attempts = 1;
int progress = 0;

int main(void) {
    // Inicializar video y consolas
    ndsInit();
    videoSetMode(MODE_0_2D);

    // Inicializar consola de depuración para UI básica
    consoleDemoInit();

    while(1) {
        scan3D();
        touchRead();
        u16 key = keysDown();

        // --- Lógica de Pausa Táctil ---
        // Detectar toque en el área central de la pantalla inferior
        if (touchX > 80 && touchX < 176 && touchY > 50 && touchY < 100) {
            if (keysHeld() & KEY_TOUCH) {
                is_paused = !is_paused;
                // Esperar un poco para evitar que la pausa cambie mil veces por segundo
                for(int i=0; i<100000; i++);
            }
        }

        if (!is_paused) {
            // --- Lógica de Salto ---
            if ((key & KEY_A) || (key & KEY_B) || (keysHeld() & KEY_TOUCH)) {
                if (!is_jumping) {
                    velocity_y = JUMP_FORCE;
                    is_jumping = true;
                }
            }

            // Gravedad y Movimiento
            velocity_y += GRAVITY;
            player_y += velocity_y;

            if (player_y >= GROUND_Y) {
                player_y = GROUND_Y;
                velocity_y = 0;
                is_jumping = false;
            }

            scroll_x += 2;
            progress = (scroll_x / 100); // Simplificado para ejemplo
            if(progress > 100) progress = 100;
        }

        // --- Renderizado ---
        // En libnds, el orden de dibujo es crucial.
        // 1. El fondo se maneja por hardware (BG), se dibuja automáticamente.
        // 2. Los objetos se dibujan encima.

        // UI Pantalla Inferior
        consoleDemoClear();
        consoleDemoSetCursorPos(5, 2);
        printf("CUBE DASH DS");
        consoleDemoSetCursorPos(5, 4);
        printf("Intento: %d", attempts);
        consoleDemoSetCursorPos(5, 5);
        printf("Progreso: %d%%", progress);

        if (is_paused) {
            consoleDemoSetCursorPos(10, 8);
            printf("[ PAUSADO ]");
        } else {
            consoleDemoSetCursorPos(10, 8);
            printf("[ TOCA PARA PAUSAR ]");
        }

        swiWaitForVBlank();
    }

    return 0;
}
