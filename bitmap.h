#ifndef BITMAP_H
#define BITMAP_H
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

typedef struct Pixel24  /* 24-bit color pixels structure */
{
    uint8_t r;  /*Red Value*/
    uint8_t g;  /*Green Value*/
    uint8_t b;  /*Blue Value*/
} Pixel24;

typedef struct Pixel32  /* 32-bit color pixels structure - NOT USED YET*/
{
    uint8_t a;  /*Red Value*/
    uint8_t b;  /*Green Value*/
    uint8_t g;  /*Blue Value*/
    uint8_t r;  /*Alpha Value*/
} Pixel32;

typedef struct Pixel8   /* Monochrome pixel structure - NOT USED YET */
{
    uint8_t brightness;
} Pixel8;

typedef struct __attribute__((__packed__))
{
    uint16_t mantissa  :16;
    uint8_t  exp       :7;
    uint8_t  sign      :1;
} PixelFloatStruct;

typedef union __attribute__((__packed__))     /* Pixel union - Treating pixels as integer makes comparisons easier */
{
    Pixel24 pix;
    PixelFloatStruct f; /* Reads a pixel24 structure as a float*/
    uint32_t integer;
} Pixel;

Pixel *  pixelArray;         /* Array containing all pixels in image */
uint32_t pixelCount = 0;    /* Ammount of pixels in array */

typedef union       /* IntegerBytes union - Makes it easier to write 4 byte integers in files */
{
    uint32_t integer;
    uint8_t  bytes[4];
} IntegerBytes;

typedef struct BMPHEADER    /* .BMP header */
{
    uint8_t  signature1;    /*B*/
    uint8_t  signature2;    /*M*/
    uint32_t fileSize;      /*Unreliable*/
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;    /*Offset to data array*/
    uint32_t infoHeaderSize;/*Must be 40*/
    uint32_t width;         /*Width in pixels*/
    uint32_t height;        /*Height in pixels*/
    uint16_t nPlanes;       /*Must be 1*/
    uint32_t compression;   /*Vide enum BMP_COMPRESSION*/
    uint32_t imageDataSize; /*Includes padding*/
    uint32_t hResolution;   /*Unreliable*/
    uint32_t vResolution;   /*Unreliable*/
    uint32_t nColors;       /*Or zero*/
    uint32_t nImportantColors;  /*Or zero*/
} BMPHEADER;

void swapEndian16(uint16_t * valuePtr)  /* Swap endianness on 2-byte values */
{
    uint16_t value = *valuePtr;
    value = (value<<8) | (value>>8);
    *valuePtr = value;
}

void swapEndian32(uint32_t * valuePtr) /* Swap endianness on 4-byte values */
{
    uint32_t value = *valuePtr;
    value = (value<<24) | ((value<<8) & 0x00ff0000) | ((value>>8) & 0x0000ff00) | (value>>24);
    *valuePtr = value;
}

void loadBitmapHeader(FILE * bmp, BMPHEADER * bmpHeader)  /* Loads and returns header of a bitmap file */
{


    fseek(bmp,10,SEEK_SET);
    fread(&bmpHeader->dataOffset,4,1,bmp);
    fseek(bmp,18,SEEK_SET);
    fread(&bmpHeader->width,4,1,bmp);
    fread(&bmpHeader->height,4,1,bmp);
}

void goToPixelArray(FILE * bmp)  /* Moves to the data array of a bitmap file */
{
    fseek(bmp,54,SEEK_SET);
}

Pixel pixel24ToPixel(Pixel24 pix24)
{
    Pixel p;
    p.pix = pix24;

    return p;
}

Pixel24 getPixel24(FILE * bmp)  /*Reads a single 3 byte pixel in a bitmap file - DOESNT SKIP PADDING */
{
    Pixel24 pixel;
    fread(&pixel.b,1,1,bmp);
    fread(&pixel.g,1,1,bmp);
    fread(&pixel.r,1,1,bmp);

    return pixel;
}

Pixel32 getPixel32(FILE * bmp)  /*Reads a single 4 byte pixel in a bitmap file */
{
    Pixel32 pixel;
    fread(&pixel.a,1,1,bmp);
    fread(&pixel.b,1,1,bmp);
    fread(&pixel.g,1,1,bmp);
    fread(&pixel.r,1,1,bmp);
    return pixel;
}

Pixel8 getPixel8(FILE * bmp)    /*Reads a single 1 byte pixel in a bitmap file */
{
    Pixel8 pixel;
    fread(&pixel,sizeof(Pixel8),1,bmp);
    return pixel;
}

void printPixel24(Pixel24 p)    /* Prints the color values of a 3-byte pixel */
{
    printf("R:%dG:%dB:%d\n", p.r, p.g, p.b);
}

void fwriteInteger32(uint32_t integer, FILE * file) /* Writes 4 byte integer on file */
{
    IntegerBytes buffer;
    buffer.integer = integer;
    fwrite(buffer.bytes,4,1,file);
}

void createBMPHEADER(FILE * file, uint32_t width, uint32_t height)
{
    uint32_t fileSize = width*height*3 + height * width%4;
    fseek(file,0,SEEK_SET);

    fputc('B',file);
    fputc('M',file);
    fwriteInteger32(54 + fileSize , file);
    fwriteInteger32(0,file);
    fwriteInteger32(54,file);
    fwriteInteger32(40, file);
    fwriteInteger32(width, file);
    fwriteInteger32(height, file);
    fputc(1,file);
    fputc(0,file);
    fputc(24,file);
    fputc(0,file);
    fwriteInteger32(0,file);
    fwriteInteger32(fileSize, file);
    fwriteInteger32(0,file);
    fwriteInteger32(0,file);
    fwriteInteger32(0,file);
    fwriteInteger32(0,file);

}


void loadPixels(FILE * file, uint32_t width, uint32_t height, Pixel * arr) /* Loads all pixle sin a bmp file on a Pixel array */
{
    uint8_t padding = width%4;

    goToPixelArray(file);

    for(uint8_t i = 0; i < height; i++)
    {
        for(uint8_t j = 0; j < width; j++) arr[(height - 1 -i) * width + j] = pixel24ToPixel(getPixel24(file));

        for(uint8_t p = 0; p < padding; p++) fgetc(file);
    }

}

void createPixelArray(FILE * bmp, uint32_t width, uint32_t height)
{
    uint32_t pixelAmmount = width * height;

    pixelArray = (Pixel *) malloc(pixelAmmount*sizeof(Pixel));

    loadPixels(bmp, width, height, pixelArray);

}

void printPixelArray(uint32_t numPixels)
{
    for(uint32_t i = 0; i < numPixels; i++) printPixel24(pixelArray[i].pix);
}



#pragma pack(pop)

#endif // BITMAP_H
