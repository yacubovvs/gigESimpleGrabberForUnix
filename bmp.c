//#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "bmp.h"

static unsigned char read_u8(FILE* fp)
{
    unsigned char b0;

    b0 = getc(fp);
    return b0;
}


static unsigned short read_u16(FILE* fp)
{
    unsigned char b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
}

static unsigned int read_u32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static int read_s32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

static BITMAPFILEHEADER readHeader(FILE* pFile) {
    BITMAPFILEHEADER header;
    header.bfType = read_u16(pFile);
    header.bfSize = read_u32(pFile);
    header.bfReserved1 = read_u16(pFile);
    header.bfReserved2 = read_u16(pFile);
    header.bfOffBits = read_u32(pFile);

    return header;
}

static BITMAPINFOHEADER readBmiHeader(FILE* pFile) {
    BITMAPINFOHEADER bmiHeader;

    bmiHeader.biSize = read_u32(pFile);
    bmiHeader.biWidth = read_s32(pFile);
    bmiHeader.biHeight = read_s32(pFile);
    bmiHeader.biPlanes = read_u16(pFile);
    bmiHeader.biBitCount = read_u16(pFile);
    bmiHeader.biCompression = read_u32(pFile);
    bmiHeader.biSizeImage = read_u32(pFile);
    bmiHeader.biXPelsPerMeter = read_s32(pFile);
    bmiHeader.biYPelsPerMeter = read_s32(pFile);
    bmiHeader.biClrUsed = read_u32(pFile);
    bmiHeader.biClrImportant = read_u32(pFile);

    return bmiHeader;
}


static void writeBmiHeader(BITMAPFILEHEADER header, FILE* pFile) {
    write_u16(pFile, header.bfType);
    write_u32(pFile, header.bfSize);
    write_u16(pFile, header.bfReserved1);
    write_u16(pFile, header.bfReserved2);
    write_u32(pFile, header.bfOffBits);

   // putc(BITMAPFILEHEADER, pFile);
}

static void writeHeader(BITMAPINFOHEADER header, FILE* pFile) {
    write_u32(pFile, header.biSize);
    write_s32(pFile, header.biWidth);
    write_s32(pFile, header.biHeight);
    write_u16(pFile, header.biPlanes);
    write_u16(pFile, header.biBitCount);
    write_u32(pFile, header.biCompression);
    write_u32(pFile, header.biSizeImage);
    write_s32(pFile, header.biXPelsPerMeter);
    write_s32(pFile, header.biYPelsPerMeter);
    write_u32(pFile, header.biClrUsed);
    write_u32(pFile, header.biClrImportant);
}

static void write_u8(FILE* fp, unsigned char val){
    putc( (char)(val), fp);
}


static void write_u16(FILE* fp, unsigned short val)
{
    putc( (char)val, fp);
    putc( (char)(val >> 8), fp);
}

static void write_u32(FILE* fp, unsigned int val)
{
    putc((char)(val), fp);
    putc((char)(val >> 8), fp);
    putc((char)(val >> 16), fp);
    putc((char)(val >> 24), fp);
}


static void write_s32(FILE* fp, int val)
{
    putc((char)(val), fp);
    putc((char)(val >> 8), fp);
    putc((char)(val >> 16), fp);
    putc((char)(val >> 24), fp);
}

