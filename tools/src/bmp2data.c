#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#pragma pack(push, 1) // Disable padding for struct members

#include "../../shared/files/bmp.h"
#include "../../shared/files/spritesheet.h"

#pragma pack(pop) // Restore padding

int readHeader(FILE *file, FILE *outFile, int frames) {

    // Read BMP File Header
    BMPFileHeader fileHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    // Check if the file is a BMP
    if (fileHeader.bfType != BMP_SIGNATURE) {
        printf("This is not a valid BMP file.\n");
        return EXIT_FAILURE;
    }

    // Read BMP DIB Header
    BMPDIBHeader dibHeader;
    fread(&dibHeader, sizeof(BMPDIBHeader), 1, file);

    // Check if the image is indexed
    if(dibHeader.biBitCount > 8) {
        printf("This is not an indexed BMP (bit count: %d).\n", dibHeader.biBitCount);
        return EXIT_FAILURE;
    }

    // Check image has dimensions divisible by 2
    if((dibHeader.biWidth & 0x01) != 0) {
        printf("Image width is not divisible by two\n");
        return EXIT_FAILURE;
    }

    if((dibHeader.biHeight & 0x01) != 0) {
        printf("Image height is not divisible by two\n");
        return EXIT_FAILURE;
    }

    // TODO Check sprite width/height to not be bigger than uint_16 
    
    // Display image dimensions
    printf("Width: %d pixels\n", dibHeader.biWidth);
    printf("Height: %d pixels\n", dibHeader.biHeight);
    printf("BPP: %dBits [%d colors]\n", dibHeader.biBitCount, (int) pow(2, dibHeader.biBitCount));

    // Move the file pointer to the start of pixel data
    fseek(file, fileHeader.bfOffBits, SEEK_SET);

    // Calculate the number of pixels (width * height)
    int width = dibHeader.biWidth;
    int height = dibHeader.biHeight;
    int numPixels = width * height;

    // Allocate memory for pixel data
    uint8_t *pixelData = (uint8_t *)malloc(numPixels * sizeof(uint8_t));
    if (!pixelData) {
        printf("Not enough memory to open image data.\n");
        return EXIT_FAILURE;
    }
    uint8_t *spriteData = (uint8_t *)malloc(numPixels * sizeof(uint8_t));
    if (!spriteData) {
        printf("Not enough memory to create pixel reordering buffer.\n");
        return EXIT_FAILURE;
    }

    Sprite outputHeader;
    outputHeader.width = dibHeader.biWidth / frames;
    outputHeader.height = dibHeader.biHeight;
    outputHeader.frames = frames;

    fwrite(&outputHeader, sizeof(outputHeader), 1, outFile);

    // Read the pixel data (bottom-up order)
    fread(pixelData, sizeof(uint8_t), numPixels, file);

    const int SPRITE_WIDTH = width / frames;
    // Align each frame pixel data linearly
    for(int frame = 0; frame < frames; frame++) {
        int offset = frame * SPRITE_WIDTH * height;
        
        for(int row = 0; row < height; row++) {
            for(int col = 0; col < SPRITE_WIDTH; col++) {
                // BMP stores data upside-down so we must invert it, hence the (height -1 - row)
                spriteData[col + row * SPRITE_WIDTH + offset] = pixelData[col + (height - 1 - row) * width + frame * SPRITE_WIDTH];
            }
        }
    }


    // for(int row = 0; row < height; row++) {
    //     memcpy(spriteData + sizeof(uint8_t) * row * width, pixelData + sizeof(uint8_t) * row * width, sizeof(uint8_t) * width);
    // }


    // fwrite(pixelData, sizeof(uint8_t), numPixels, outFile);
    fwrite(spriteData, sizeof(uint8_t), numPixels, outFile);
    printf("File saved correctly\n");

    // Free allocated memory
    free(pixelData);
    free(spriteData);
       
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <bmp_file> <output_file> <num_frames>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input = argv[1];
    const char *output = argv[2];
    const int frames = atoi(argv[3]);

    // Prevent overwrite input
    if(input == output) {
        perror("Error output file must be different from input file\n");
        return EXIT_FAILURE;
    }


    FILE *inputFile = fopen(input, "rb");
    FILE *outFile = fopen(output, "wb");

    int error = readHeader(inputFile, outFile,frames);

    fclose(inputFile);
    fclose(outFile);

    if(error) {
        remove(output);
    }

    return EXIT_SUCCESS;
}
