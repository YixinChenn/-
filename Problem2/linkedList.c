#include "linkedList.h"

int inButterfly(IMGDATA data){
    if(data.r == 0 && data.b == 0 && data.g == 0)
        return -1;
    else if(data.r == 255 && data.b == 255 && data.g == 255)
        return -1;
    else
        return 0;
}

NODE* insert(int x, int y, NODE* head){
    // printf("insert ");
    NODE *p = (NODE*)malloc(sizeof(NODE));
    p->x = x;
    p->y = y;
    p->next = head;
    return p;
}

void generateNewImg(NODE* head, BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader){
    FILE *fp = fopen("Output.bmp", "wb");
    int i, j, cnt = 0;
    int npad = 0;
    int w = infoHeader.biWidth;
    int h = infoHeader.biHeight;
    int pad = padding(w);
    IMGDATA *data = (IMGDATA*)malloc(sizeof(IMGDATA) * infoHeader.biWidth * infoHeader.biHeight);
    writeFileHeader(fileHeader,fp);
    writeInfoHeader(infoHeader,fp);

    for(i=0; i < w*h; i++){
        (data+i)->r = 255;
        (data+i)->b = 255;
        (data+i)->g = 255;
    }

    NODE *p = head;
    while(p){
        (data + p->y*w + p->x)->r = 0;
        (data + p->y*w + p->x)->b = 0;
        (data + p->y*w + p->x)->g = 0;
        p=p->next;
    }

    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            fwrite((data + cnt), sizeof(IMGDATA), 1, fp);
            cnt++;
        }
        for(npad = 0; npad < pad; npad++)
            writePAD(0,fp);
    }
    fclose(fp);
}

