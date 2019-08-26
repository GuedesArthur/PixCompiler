#ifndef PIXCOMPILER_H
#define PIXCOMPILER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bitmap.h"

#define PALLETWID 6
#define PALLETHEI 6
#define NPALLET PALLETWID * PALLETHEI
#define PALLETPAD PALLETWID%4


enum Tokens
{
    IF,     THEN,   ELSE,   WHILE,
    ENDLINE, END,
    INT,    LONG,   FLOAT,  STRING,
    TYPEINT,TYPELONG,TYPEFLOAT,TYPESTRING,
    IN,     OUT,    ASSIGN,
    ADD,    SUB,    MULT,   DIV,
    QUOT,   PAROPEN,PARCLOSE,
    AND,    OR,     NOT,
    EQUALS, DIFFERS,
    GREATER,GREATEREQUAL,
    LESS,   LESSEQUAL,
    INIT,   RETURN,
    ENDOFCODE, ID
};

const char * tokenName[] =
{
    "IF",     "THEN",   "ELSE",   "WHILE",
    "ENDLINE", "END",
    "INT",    "LONG",   "FLOAT",  "STRING",
    "TYPEINT","TYPELONG","TYPEFLOAT","TYPESTRING",
    "IN",     "OUT",    "ASSIGN",
    "ADD",    "SUB",    "MULT",   "DIV",
    "QUOT",   "PAROPEN","PARCLOSE",
    "AND",    "OR",     "NOT",
    "EQUALS", "DIFFERS",
    "GREATER","GREATEREQUAL",
    "LESS",   "LESSEQUAL",
    "INIT",   "RETURN",
    "ENDOFCODE"
};

const char * tokenText[] =                  /*Token id to corresponding C language text */
{
    "if(", ")\n{", "\nelse{", "\nwhile(",
    ";\n", "}\n",
    "","","","",
    "int", "long", "float", "const char *",
    "scanf(", "printf(", "=",
    " + ", " - ", " * ", "  / ",
    "\"", " (", ") ",
    "&&", "||", "!",
    "==", "!=",
    ">", ">=",
    "<", "<=",
    "#include<stdio.h> \nint main()\n{", "return ",
    "}/* end of code */", ""
};

uint8_t isNextTokenValue(uint8_t token)
{
    return token == INT || token == LONG || token == FLOAT || token == STRING;
}

typedef struct PalletStruct
{
    Pixel   ifColor,        thenColor,      elseColor,      whileColor,
            endLineColor,   endColor,
            intColor,       longColor,      floatColor,     stringColor,
            typeIntColor,   typeLongColor,  typeFloatColor, typeStringColor,
            inColor,        outColor,       assignColor,
            addColor,       subColor,       multColor,      divColor,
            quotColor,      parOpenColor,   parCloseColor,
            andColor,       orColor,        notColor,
            equalsColor,    differsColor,
            greaterColor,   greaterEqualColor,
            lessColor,      lessEqualColor,
            initColor,      returnColor,
            endOfCodeColor;
} PalletStruct;

typedef struct __attribute__((__packed__)) FloatBitsStruct
{
    uint8_t sign       :1;
    uint8_t exp          ;
    uint32_t mantissa :23;
} FloatBitsStruct;

typedef union
{
    FloatBitsStruct bits;
    float f;
} FloatBits;


typedef union
{
    PalletStruct pixels;
    Pixel pixArray[NPALLET];
    uint32_t array[NPALLET];
} Pallet;



float pixelToFloat(Pixel p) /* Reads pixel data as a floating point value */
{
    FloatBits f;


    f.bits.exp = p.f.exp;
    f.bits.mantissa = p.f.mantissa;
    f.bits.sign = p.f.sign;

    printf("%f",f.f);

    return f.f;
}
Pallet pallet;   /* Token color pallet - Store reserved colors for each token */

Pixel createPixel(uint8_t r, uint8_t g, uint8_t b) /* Creates a pixel value out of RGB values */
{
    Pixel pixel;
    pixel.pix.r = r;
    pixel.pix.g = g;
    pixel.pix.b = b;

    return pixel;
}

void fwritePixel(FILE * file, Pixel p)
{
    fwrite(&p.pix.b,1,1,file);
    fwrite(&p.pix.g,1,1,file);
    fwrite(&p.pix.r,1,1,file);
}

char hexDigitToChar(uint8_t hex) /* Converts a hex digit to a hex character */
{
    if(hex < 0xA)  return (char) (hex + '0');
    else return (char) (hex - 10 + 'A');
}

char * pixelToString(Pixel p)
{
    char * str = (char *) malloc(6);

    str[0] = hexDigitToChar(p.pix.b / 16);
    str[1] = hexDigitToChar(p.pix.b & 0xF);
    str[2] = hexDigitToChar(p.pix.g / 16);
    str[3] = hexDigitToChar(p.pix.g & 0xF);
    str[4] = hexDigitToChar(p.pix.r / 16);
    str[5] = hexDigitToChar(p.pix.r & 0xF);
    str[6] = 0;

    return str;

}

void createPAINTNETPalletFile()   /*Create a pallet file that can be used with Paint.NET*/
{
    FILE * pFile = fopen("pixPallet.txt","w");
    char * str;

    printf("Creating pallet file for Paint.NET\n");

    for(uint8_t i = 0; i < NPALLET; i++)
    {
        fputc('F',pFile);
        fputc('F',pFile);
        str = pixelToString(pallet.pixArray[i]);
        fwrite(str,1,6,pFile);
        fputc('\n',pFile);
        free(str);
    }

    fclose(pFile);

}

void createDefaultPallet()
{
    FILE * file = fopen("pallet.bmp","wb");
    Pixel defaultPixelArray[PALLETHEI][PALLETWID] = {
                                        {
                                        createPixel(128,  0,128), createPixel( 32,128, 32), createPixel(192, 32, 64),
                                        createPixel(255,192,  0), createPixel(255,255,255), createPixel( 32,128,128)
                                        },
                                        {
                                        createPixel( 96,128,255), createPixel(192,192, 32), createPixel( 64,224, 64),
                                        createPixel( 32, 64, 32), createPixel(128,128,255), createPixel(192, 64,192)
                                        },
                                        {
                                        createPixel(128,128, 32), createPixel(192,128,192), createPixel( 32,128,224),
                                        createPixel(  0, 64, 32), createPixel(192, 64, 32), createPixel(128,224,128)
                                        },
                                        {
                                        createPixel( 64, 64,224), createPixel( 96, 96, 64), createPixel(128, 96, 32),
                                        createPixel( 96,128, 32), createPixel( 64, 64, 96), createPixel(224, 96,224)
                                        },
                                        {
                                        createPixel( 64,128, 32), createPixel(224,128, 32), createPixel(  0,128,  0),
                                        createPixel( 32,224,224), createPixel( 32, 64, 64), createPixel( 32,255, 32)
                                        },
                                        {
                                        createPixel(128, 32, 32), createPixel(255,255, 96), createPixel(  0, 96, 96),
                                        createPixel(128,224, 32), createPixel(255,128,255), createPixel(  0,  0,  0)
                                        }
                                        };

    createBMPHEADER(file, PALLETWID,PALLETHEI);

    for(uint8_t i = 0; i < PALLETHEI; i++)
    {
        for(uint8_t j = 0; j < PALLETWID; j++) fwritePixel(file, defaultPixelArray[PALLETHEI - 1 - i][j]);

        for(uint8_t p = 0; p < PALLETPAD; p++) fputc(255,file);

    }

    fclose(file);
}


void loadPalletFile(FILE * palletFile)
{
    loadPixels(palletFile, PALLETWID, PALLETHEI, pallet.pixArray);
}


void loadPallet()
{
    FILE * palletFile = fopen("pallet.bmp","rb");

    if(!palletFile)
    {
        printf("Creating default pallet.\n");
        createDefaultPallet();
        palletFile = fopen("pallet.bmp","rb");
    }

    printf("Loading pallet.bmp .\n");

    loadPalletFile(palletFile);
    createPAINTNETPalletFile();


    fclose(palletFile);

}

typedef uint8_t Token;

typedef struct id
{
    Pixel color;
    uint64_t value;
    Token token;
    uint8_t type;
} id;

enum IDType
{
    NOT_ASSIGNED, TYPE_INT, TYPE_LONG, TYPE_FLOAT, TYPE_STRING, TYPE_MACRO
};

#endif // PIXCOMPILER_H
