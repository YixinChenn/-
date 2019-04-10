#include "BMPstructfns.h"

int padding(int ncols){
	int alignByte = (4 - ncols % 4) % 4;
	return alignByte;
}

void write4LE(unsigned int val, FILE *fp){
	for (int i=0; i<4; i++){
		int r=val%256;
		fputc(r,fp);
		val/=256;
	}
}



void write2LE(unsigned int val, FILE *fp){
	for (int i=0; i<2; i++){
		int r=val%256;
		fputc(r,fp);
		val/=256;
	}
}

void writePIXEL(int red, int grn, int blu, FILE *fp){
	fputc(red, fp);
	fputc(grn, fp);
	fputc(blu, fp);
}

void writePAD(int pad, FILE *fp){
	fputc(pad, fp);
}


BITMAPFILEHEADER readFileHeader(FILE *fp){
    BITMAPFILEHEADER header;
    fread(&header.bfType, sizeof(header.bfType), 1, fp);
    fread(&header.bfSize, sizeof(header.bfSize), 1, fp);
    fread(&header.bfReserved1, sizeof(header.bfReserved1), 1, fp);
    fread(&header.bfReserved2, sizeof(header.bfReserved2), 1, fp);
    fread(&header.bfOffBits, sizeof(header.bfOffBits), 1, fp);
    
    return header;
}

BITMAPINFOHEADER readInfoHeader(FILE *fp){
    BITMAPINFOHEADER header;
    fread(&header.biSize, sizeof(header.biSize), 1, fp);
    fread(&header.biWidth, sizeof(header.biWidth), 1, fp);
    fread(&header.biHeight, sizeof(header.biHeight), 1, fp);
    fread(&header.biPlanes, sizeof(header.biPlanes), 1, fp);
    fread(&header.biBitCount, sizeof(header.biBitCount), 1, fp);
    fread(&header.biCompression, sizeof(header.biCompression), 1, fp);
    fread(&header.biSizeImage, sizeof(header.biSizeImage), 1, fp);
    fread(&header.biXPelsPerMeter, sizeof(header.biXPelsPerMeter), 1, fp);
    fread(&header.biYPelsPerMeter, sizeof(header.biYPelsPerMeter), 1, fp);
    fread(&header.biClrUsed, sizeof(header.biClrUsed), 1, fp);
    fread(&header.biClrImportant, sizeof(header.biClrImportant), 1, fp);

    return header;
}

void readImgData(IMGDATA *data, FILE *fp, int h, int w, int pad){
    int i = 0, cnt = 0;
    for(i = 0; i < h; i++){
        int j = 0;
        for(int j = 0; j < w; j++){
            fread((data + cnt), sizeof(IMGDATA), 1, fp);
            if((data+cnt)->r<110 && (data+cnt)->b<110 && (data+cnt)->g<110){
                (data+cnt)->r = 0;
                (data+cnt)->b = 0;
                (data+cnt)->g = 0;
            }
            if((data + cnt)->r < 70){
                (data + cnt)->r = 0;
            }
            else if((data + cnt)->r > 180){
                (data + cnt)->r = 255;
            }
            if((data + cnt)->b < 70){
                (data + cnt)->b = 0;
            }
            else if((data + cnt)->b > 180){
                (data + cnt)->b = 255;
            }
            if((data + cnt)->g < 70){
                (data + cnt)->g = 0;
            }
            else if((data + cnt)->g > 180){
                (data + cnt)->g = 255;
            }
            cnt++;
        }

        if(pad != 0){
            int npad = 0;
            IMGDATA paddata;
            for(npad = 0; npad < pad; npad++){
                fread(&paddata, sizeof(IMGDATA), 1, fp);
            }
        }
    }   
}

void saveToNewFile(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, IMGDATA *data, FILE *fp, int pad){
    writeFileHeader(fileHeader,fp);
    writeInfoHeader(infoHeader,fp);
    int cnt = 0, i=0;
    for(int i = 0; i < infoHeader.biHeight; i++){
        for(int j = 0; j < infoHeader.biWidth; j++){
            fwrite((data+cnt), sizeof(IMGDATA), 1, fp);
            cnt++;
        }
        int npad;
        for(npad = 0; npad < pad; npad++) writePAD(0,fp);
    }
}

void writeFileHeader(BITMAPFILEHEADER header, FILE *fp){
    
    fwrite(&header.bfType, sizeof(header.bfType), 1, fp);
    fwrite(&header.bfSize, sizeof(header.bfSize), 1, fp);
    fwrite(&header.bfReserved1, sizeof(header.bfReserved1), 1, fp);
    fwrite(&header.bfReserved2, sizeof(header.bfReserved2), 1, fp);
    fwrite(&header.bfOffBits, sizeof(header.bfOffBits), 1, fp);
    
    return;
}

void writeInfoHeader(BITMAPINFOHEADER header, FILE *fp){
    
    fwrite(&header.biSize, sizeof(header.biSize), 1, fp);
    fwrite(&header.biWidth, sizeof(header.biWidth), 1, fp);
    fwrite(&header.biHeight, sizeof(header.biHeight), 1, fp);
    fwrite(&header.biPlanes, sizeof(header.biPlanes), 1, fp);
    fwrite(&header.biBitCount, sizeof(header.biBitCount), 1, fp);
    fwrite(&header.biCompression, sizeof(header.biCompression), 1, fp);
    fwrite(&header.biSizeImage, sizeof(header.biSizeImage), 1, fp);
    fwrite(&header.biXPelsPerMeter, sizeof(header.biXPelsPerMeter), 1, fp);
    fwrite(&header.biYPelsPerMeter, sizeof(header.biYPelsPerMeter), 1, fp);
    fwrite(&header.biClrUsed, sizeof(header.biClrUsed), 1, fp);
    fwrite(&header.biClrImportant, sizeof(header.biClrImportant), 1, fp);

    return;
}

uint32_t bswap_32(uint32_t x)  
{  
    return (((uint32_t)(x) & 0xff000000) >> 24) | \
           (((uint32_t)(x) & 0x00ff0000) >> 8) | \
           (((uint32_t)(x) & 0x0000ff00) << 8) | \
           (((uint32_t)(x) & 0x000000ff) << 24) ;  
}

IMGDATA getPix(int x, int y, IMGDATA *data, int w){
    return *(data + y*w + x);
}

int nextToClr(IMGDATA *data, IMGDATA clr, int cnt, int w, int h){
    IMGDATA black;
    black.r = 0;
    black.b = 0;
    black.g = 0;
    int x = cnt%w;
    int y = cnt/w;
    if(x > 0){
        if(sameClr(getPix(x-1,y,data,w), clr, 0)) return 1;
        if(y > 0 && sameClr(getPix(x-1,y-1,data,w), clr, 0)) return 1;
        if(y < h-1 && sameClr(getPix(x-1,y+1,data,w), clr, 0)) return 1; 
    }
    if(x < w-1){
        if(sameClr(getPix(x+1,y,data,w), clr, 0)) return 1;
        if(y > 0 && sameClr(getPix(x+1,y-1,data,w), clr, 0)) return 1;
        if(y < h-1 && sameClr(getPix(x+1,y+1,data,w), clr, 0)) return 1; 
        
    }
    if(y > 0 && sameClr(getPix(x,y-1,data,w), clr, 0)) return 1;
    if(y < h-1 && sameClr(getPix(x,y+1,data,w), clr, 0)) return 1;

    return 0;
}

int sameClr(IMGDATA pix, IMGDATA clr, int mode){
    if(mode){
        if(pix.r == clr.r && pix.b == clr.b && pix.g == clr.g)
            return 1;
        else
            return 0;
    }
    if(clr.r == 0 && pix.r > 0) return 0;
    if(clr.b == 0 && pix.b > 0) return 0;
    if(clr.g == 0 && pix.g > 0) return 0;
    if(clr.r < 255-ACC && pix.r > clr.r+ACC) return 0;
    if(clr.r >= ACC && pix.r < clr.r-ACC) return 0;
    if(clr.b < 255-ACC && pix.b > clr.b+ACC) return 0;
    if(clr.b >= ACC && pix.b < clr.b-ACC) return 0;
    if(clr.g < 255-ACC && pix.g > clr.g+ACC) return 0;
    if(clr.g >= ACC && pix.g < clr.g-ACC) return 0;
    return 1;
}
