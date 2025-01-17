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
    int prevX;
    int prevY;
    int width;
    int height;
    int velocityX;
    int velocityY;
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
void clearPrevious(struct rect* cRect) {
    for (int x = cRect->prevX; x < cRect->prevX + cRect->width; x++) {
        for (int y = cRect->prevY; y < cRect->prevY + cRect-> height; y++) {
            drawPixel(x, y, 0x0000);
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
    humanPlayer.y = SCREEN_HEIGHT / 2 - 24 / 2;
    humanPlayer.prevX = humanPlayer.x;
    humanPlayer.prevY = humanPlayer.y;
    humanPlayer.width = 8;
    humanPlayer.height = 24;
    humanPlayer.velocityX = 0;
    humanPlayer.velocityY = 0;

    struct rect computerPlayer;
    computerPlayer.x = SCREEN_WIDTH - 8 - 1;
    computerPlayer.y = SCREEN_HEIGHT / 2 - 24 / 2;
    computerPlayer.prevX = computerPlayer.x;
    computerPlayer.prevY = computerPlayer.y;
    computerPlayer.width = 8;
    computerPlayer.height = 24;
    computerPlayer.velocityX = 0;
    computerPlayer.velocityY = 0;

    struct rect ball;
    ball.x = SCREEN_WIDTH / 2 - 8 / 2;
    ball.y = SCREEN_HEIGHT / 2 - 8 / 2;
    ball.prevX = ball.x;
    ball.prevY = ball.y;
    ball.width = 8;
    ball.height = 8;
    ball.velocityX = 0;
    ball.velocityY = 0;

    /* Draw Center Line */
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        drawPixel(SCREEN_WIDTH / 2, i, 0x7FFF);  // White pixels
    }

    while (1) {
        VBlankIntrWait();

        /* Respond to input */
        scanKeys();
        int key_pressed = keysDown();
        int key_released = keysUp();

        /* If up or down released -> velocity = 0 */
        if ((key_released & KEY_UP) || (key_released & KEY_DOWN)) {
            humanPlayer.velocityY = 0;
        }
        /* If up is pressed -> velocity = -2 */
        if ((key_pressed & KEY_UP) && humanPlayer.y >= 0) {
            // Move up
            humanPlayer.velocityY = -2;
        }
        /* If down is pressed -> velocity = 2 */
        if ((key_pressed & KEY_DOWN) && humanPlayer.y <= SCREEN_HEIGHT -
            humanPlayer.height) {
                // Move down
                humanPlayer.velocityY = 2;
        }
        /* If pressed but at ceiling or floor, don't continue */
        if ((humanPlayer.y <= 0 && humanPlayer.velocityY < 0) ||
            ((humanPlayer.y >= SCREEN_HEIGHT - humanPlayer.height) &&
            humanPlayer.velocityY > 0)) {
                humanPlayer.velocityY = 0;
        }

        humanPlayer.y += humanPlayer.velocityY;

        /* Clear Previous */
        clearPrevious(&ball);
        clearPrevious(&humanPlayer);
        clearPrevious(&computerPlayer);

        /* Draw Players */
        drawRect(&ball);
        drawRect(&humanPlayer);
        drawRect(&computerPlayer);

        humanPlayer.prevX = humanPlayer.x;
        humanPlayer.prevY = humanPlayer.y;
    }
}
