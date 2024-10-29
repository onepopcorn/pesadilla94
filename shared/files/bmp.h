#ifndef _BMP_FORMAT_H
#define _BMP_FORMAT_H

/**
 * BMP Format info
 * https://en.wikipedia.org/wiki/BMP_file_format
 * 
 */
#define BMP_SIGNATURE 0x4D42 // In little endian

// BMP file header (14 bytes)
typedef struct BMPFileHeader {
    uint16_t bfType;      // File type ("BM" for Bitmap)
    uint32_t bfSize;      // Size of the file
    uint16_t bfReserved1; // Reserved, must be 0
    uint16_t bfReserved2; // Reserved, must be 0
    uint32_t bfOffBits;   // Offset to start of pixel data
} BMPFileHeader;

// BMP DIB header (this is the Windows BITMAPINFOHEADER - 40 bytes)
typedef struct {
    uint32_t biSize;          // Size of this header (40 bytes)
    int32_t  biWidth;         // Width of the image
    int32_t  biHeight;        // Height of the image
    uint16_t biPlanes;        // Number of color planes (must be 1)
    uint16_t biBitCount;      // Number of bits per pixel (1, 4, 8, 16, 24, 32)
    uint32_t biCompression;   // Compression type (0 = none)
    uint32_t biSizeImage;     // Image size (can be 0 for uncompressed images)
    int32_t  biXPelsPerMeter; // Horizontal resolution
    int32_t  biYPelsPerMeter; // Vertical resolution
    uint32_t biClrUsed;       // Number of colors in the palette
    uint32_t biClrImportant;  // Important colors (generally ignored)
} BMPDIBHeader;

#endif