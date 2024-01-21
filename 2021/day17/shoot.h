#include <stdio.h>
#include <stdbool.h>

struct ShotResult {
    int maxHeight;
    int hitCount;
};

struct ShotResult shoot(int targetMinX, int targetMaxX, int targetMinY, int targetMaxY) {
    int velocityMinX = 0;
    int velocityMaxX = targetMaxX;
    int velocityMinY = targetMinY;
    int velocityMaxY = -targetMinY;

    struct ShotResult result = { 0, 0 };

    for (int velocityX0 = velocityMinX; velocityX0 <= velocityMaxX; velocityX0++) {
        for (int velocityY0 = velocityMinY; velocityY0 <= velocityMaxY; velocityY0++) {
            int x = 0;
            int y = 0;
            int velocityX = velocityX0;
            int velocityY = velocityY0;
            int maxHeight = 0;

            while (x <= targetMaxX && y >= targetMinY) {
                x += velocityX;
                y += velocityY;

                if (velocityX > 0) {
                    --velocityX;
                }

                if (velocityX < 0) {
                    ++velocityX;
                }

                --velocityY;

                if (y > maxHeight) {
                    maxHeight = y;
                }

                if (x >= targetMinX && x <= targetMaxX && y >= targetMinY && y <= targetMaxY) {
                    ++result.hitCount;

                    if (maxHeight > result.maxHeight) {
                        result.maxHeight = maxHeight;
                    }

                    break;
                }
            }
        }
    }

    return result;
}