#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>

#define MEM_VRAM        0x06000000
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

typedef u16             M3LINE[SCREEN_WIDTH];
#define m3_mem          ((M3LINE*)MEM_VRAM)

/* Players and Ball Rectangles */
struct rect {
    int x;
    int y;
    int width;
    int height;
};

void drawPixel(int x, int y, int color) {
    m3_mem[y][x] = color;
}

/* Draw Player / Ball */
void drawRect(struct rect* cRect) {
    for (int x = cRect->x; x < cRect->x + cRect->width; x++) {
        for (int y = cRect->y; y < cRect->y + cRect-> height; y++) {
            drawPixel(x, y, 0x7FFF);
        }
    }
}

int main(void) {

    irqInit();
    irqEnable(IRQ_VBLANK);

    /* Set GBA to Mode 3 (Bitmap Mode) */
    SetMode( MODE_3 | BG2_ON );

    struct rect humanPlayer;
    humanPlayer.x = 1;
    humanPlayer.y = 60;
    humanPlayer.width = 8;
    humanPlayer.height = 24;

    /* Draw Center Line */
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        drawPixel(SCREEN_WIDTH / 2, i, 0x7FFF);  // White pixels
    }

    /* Draw Player */
    drawRect(&humanPlayer);

    while (1) {
        VBlankIntrWait();
    }
}
