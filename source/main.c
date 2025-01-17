#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>

#define MEM_VRAM        0x06000000
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

typedef u16             M3LINE[SCREEN_WIDTH];
#define m3_mem          ((M3LINE*)MEM_VRAM)

void drawPixel(int x, int y, int color) {
    m3_mem[y][x] = color;
}

int main(void) {

    irqInit();
    irqEnable(IRQ_VBLANK);

    /* Set GBA to Mode 3 (Bitmap Mode) */
    SetMode( MODE_3 | BG2_ON );

    /* Draw Center Line */
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        drawPixel(SCREEN_WIDTH / 2, i, 0x7FFF);  // White pixels
    }

    while (1) {
        VBlankIntrWait();
    }
}
