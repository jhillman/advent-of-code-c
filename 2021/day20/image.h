#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearImage(char **image, int width, int height) {
    for (int y = 0; y < height; y++) {
        memset(image[y], '.', width);
    }
}

int enhance(char *input, int steps) {
    FILE *inputFile = fopen(input, "r");

    int litPixels = 0;

    if (inputFile) {
        char enhancementAlgorithm[516];
        char c;
        int imageWidth = 0;
        int imageHeight = 0;
        int expandedImageWidth;
        int expandedImageHeight;
        char **inputImage;
        char **outputImage;

        fgets(enhancementAlgorithm, sizeof(enhancementAlgorithm), inputFile);
        fgetc(inputFile); // \n

        do {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++imageHeight;
            }

            if (!imageHeight) {
                ++imageWidth;
            }
        } while (!feof(inputFile));

        fseek(inputFile, 0, SEEK_SET);

        fgets(enhancementAlgorithm, sizeof(enhancementAlgorithm), inputFile);
        fgetc(inputFile); // \n

        expandedImageWidth = steps + imageWidth + steps;
        expandedImageHeight = steps + imageHeight + steps;

        inputImage = (char **)malloc(expandedImageHeight * sizeof(char *));

        for (int y = 0; y < steps; y++) {
            inputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
            memset(inputImage[y], '.', expandedImageWidth);
        }

        for (int y = steps; y < steps + imageHeight; y++) {
            inputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
            memset(inputImage[y], '.', expandedImageWidth);

            for (int x = steps; x < steps + imageWidth; x++) {
                inputImage[y][x] = fgetc(inputFile);
            }

            fgetc(inputFile);
        }

        for (int y = steps + imageHeight; y < expandedImageHeight; y++) {
            inputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
            memset(inputImage[y], '.', expandedImageWidth);
        }

        fclose(inputFile);

        outputImage = (char **)malloc(expandedImageHeight * sizeof(char *));

        for (int y = 0; y < expandedImageHeight; y++) {
            outputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
        }

        for (int i = 0; i < steps; i++) {
            clearImage(outputImage, expandedImageWidth, expandedImageHeight);

            for (int y = 0; y < expandedImageHeight; y++) {
                for (int x = 0; x < expandedImageWidth; x++) {
                    int index = 0;
                    int bit = 1 << 8;

                    for (int pixelY = y - 1; pixelY <= y + 1; pixelY++) {
                        for (int pixelX = x - 1; pixelX <= x + 1; pixelX++) {
                            if (pixelY >= 0 && pixelY < expandedImageHeight && pixelX >= 0 && pixelX < expandedImageWidth) {
                                c = inputImage[pixelY][pixelX];
                            } else {
                                c = i % 2 == 0 ? '.' : '#';
                            }

                            index += c == '#' ? bit : 0;
                            bit >>= 1;
                        }
                    }
        
                    outputImage[y][x] = enhancementAlgorithm[index];
                }
            }

            if (i < steps - 1) {
                char **temp = inputImage;
                inputImage = outputImage;
                outputImage = temp;
            }
        }

        for (int y = 0; y < expandedImageHeight; y++) {
            for (int x = 0; x < expandedImageWidth; x++) {
                litPixels += outputImage[y][x] == '#' ? 1 : 0;
            }

            free(inputImage[y]);
            free(outputImage[y]);
        }

        free(inputImage);
        free(outputImage);
    }

    return litPixels;
}
