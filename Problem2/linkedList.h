#include <stdio.h>
#include <stdlib.h>
#include "BMPstructfns.h"

typedef struct NODE{
    int x;
    int y;
    struct NODE *next;
}NODE;

int inButterfly(IMGDATA data);

NODE* insert(int x, int y, NODE* head);

void generateNewImg(NODE* head, BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader);
