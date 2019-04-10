//BMPstructfns.h
#ifndef BMPstructfns_h
#define BMPstructfns_h
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ACC 70

typedef struct  tagBITMAPFILEHEADER{
    uint16_t bfType;//文件类型，必须是0x424D，即字符“BM”
    uint32_t bfSize;//文件大小
    uint16_t bfReserved1;//保留字
    uint16_t bfReserved2;//保留字
    uint32_t bfOffBits;//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
    uint32_t biSize;//信息头大小
    uint32_t biWidth;//图像宽度
    uint32_t biHeight;//图像高度
    uint16_t biPlanes;//位平面数，必须为1
    uint16_t biBitCount;//每像素位数:1,4,8,24
    uint32_t  biCompression; //压缩类型
    uint32_t  biSizeImage; //压缩图像大小字节数
    uint32_t  biXPelsPerMeter; //水平分辨率
    uint32_t  biYPelsPerMeter; //垂直分辨率
    uint32_t  biClrUsed; //位图实际用到的色彩数
    uint32_t  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER; //位图信息头定义

typedef struct tagIMAGEDATA
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}IMGDATA;

int padding(int ncols); //number of bytes of padding to add to the end of each pixel row

void write4LE(unsigned int val, FILE *fp); //write four bytes of val little-endian to a file, exit if error

void write2LE(unsigned int val, FILE *fp); //write two bytes of val little-endian to a file, exit if error

void writePIXEL(int red, int grn, int blu, FILE *fp); //write next pixel to bmp image

void writePAD(int pad, FILE *fp); //write bmp image line-padding

BITMAPFILEHEADER readFileHeader(FILE *fp);

BITMAPINFOHEADER readInfoHeader(FILE *fp);

void readImgData(IMGDATA *data, FILE *fp, int h, int w, int pad);

void saveToNewFile(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, IMGDATA *data, FILE *fp, int pad);

uint32_t bswap_32(uint32_t x) ;

IMGDATA getPix(int x, int y, IMGDATA *data, int w);

int sameClr(IMGDATA pix, IMGDATA clr, int mode);

int nextToClr(IMGDATA *data, IMGDATA clr, int cnt, int w, int h);

void writeFileHeader(BITMAPFILEHEADER header, FILE *fp);

void writeInfoHeader(BITMAPINFOHEADER header, FILE *fp);

#endif