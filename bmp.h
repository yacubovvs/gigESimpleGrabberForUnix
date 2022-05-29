#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

typedef struct
{
    unsigned int    bfType;
    unsigned long   bfSize;
    unsigned int    bfReserved1;
    unsigned int    bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    int   rgbBlue;
    int   rgbGreen;
    int   rgbRed;
    int   rgbReserved;
} RGBQUAD;


static unsigned short read_u16(FILE* fp);
static unsigned int   read_u32(FILE* fp);
static int            read_s32(FILE* fp);
static unsigned char  read_u8(FILE* fp);
static BITMAPFILEHEADER readHeader(FILE* pFile);
static BITMAPINFOHEADER readBmiHeader(FILE* pFile);
static void writeBmiHeader(BITMAPFILEHEADER bmiHeader, FILE* pFile);
static void writeHeader(BITMAPINFOHEADER header, FILE* pFile);

static void write_u8(FILE* fp, unsigned char val);
static void write_u16(FILE* fp, unsigned short val);
static void write_u32(FILE* fp, unsigned int val);
static void write_s32(FILE* fp, int val);

#endif // MAIN_H_INCLUDEDs