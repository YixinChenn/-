#include "linkedList.h"
#include <stdbool.h>

int main(){
    FILE *fp = fopen("butterfly.bmp", "rb");
    NODE *head = NULL;
 
    BITMAPFILEHEADER fileHeader = readFileHeader(fp);
    BITMAPINFOHEADER infoHeader = readInfoHeader(fp);

    int w = infoHeader.biWidth;
    int h = infoHeader.biHeight;

    IMGDATA *data = (IMGDATA*)malloc(sizeof(IMGDATA) * infoHeader.biWidth * infoHeader.biHeight);
    readImgData(data, fp,infoHeader.biHeight, infoHeader.biWidth, padding(infoHeader.biWidth));

choose:
    printf("choose a coordinate x and y (for example, insert \"40 200\"). \nx < %d, y < %d.\n", infoHeader.biWidth, infoHeader.biHeight);
    int x,y,px,py;
    scanf("%d %d", &x, &y);
    IMGDATA clr = *(data+y*w + x);
    printf("pixel color: r = %d, b = %d, g = %d\n", clr.r, clr.b, clr.g);
    if(inButterfly(getPix(x,y,data,w)) < 0){
        printf("please make sure the coordinate is in a color region\n");
        goto choose;
    }
    int cnt = 0;
    IMGDATA black;
    black.r = 0;
    black.b = 0;
    black.g = 0;
    while(cnt < w*h){
        if(sameClr(*(data+cnt), clr, 0) != 1){
            (data+cnt)->r = 255;
            (data+cnt)->b = 255;
            (data+cnt)->g = 255;
        }
        cnt++;
    }


    int flag = 1;
    int t,b,l,r;
    for(px = x; px < w; px++){
        for(py = 0; py < h; py++){
            if(sameClr(getPix(px,py,data,w),clr,0)==1){
                flag = 0;
                continue;
            }
        }
        if(flag) break;
        else flag = 1;
    }
    r = px;
    for(px = x; px > 0; px--){
        for(py = 0; py < h; py++){
            if(sameClr(getPix(px,py,data,w),clr,0)==1){
                flag = 0;
                continue;
            }
        }
        if(flag) break;
        else flag = 1;
    }
    l = px;
    for(py = y; py < h; py++){
        for(px = 0; px < w; px++){
            if(sameClr(getPix(px,py,data,w),clr,0)==1){
                flag = 0;
                continue;
            }
        }
        if(flag) break;
        else flag = 1;
    }
    t = py;
    for(py = y; py > 0; py--){
        for(px = 0; px < w; px++){
            if(sameClr(getPix(px,py,data,w),clr,0)==1){
                flag = 0;
                continue;
            }
        }
        if(flag) break;
        else flag = 1;
    }
    b = py;
    cnt = 0;
    int i,j;
    printf("l%d r%d t%d b%d\n",l,r,t,b);
    for(i=0;i<h;i++){
        for(j=0;j<w;j++){
            if(i<b || i>t || j<l || j>r){
                (data+cnt)->r =255;
                (data+cnt)->b =255;
                (data+cnt)->g =255;
            }
            cnt++;
        }
    }
    cnt=0;
    IMGDATA white;
    white.r = 255;
    white.b = 255;
    white.g = 255;
    while(cnt<w*h){
        if(sameClr(*(data+cnt),white,1) == 1){
            if(nextToClr(data,clr,cnt,w,h) == 1){
                (data+cnt)->r = 0;
                (data+cnt)->b = 0;
                (data+cnt)->g = 0;
            }
        }
        cnt++;
    }

    cnt = 0;
    while(cnt<w*h){
        if(sameClr(*(data+cnt),black,1) == 1){
            head = insert(cnt%w, cnt/w, head);
        }
        cnt++;
    }


    // FILE *out = fopen("2.bmp", "wb");
    // saveToNewFile(fileHeader, infoHeader, data, out, padding(w));

    if(!head) printf("list is empty\n");
    generateNewImg(head, fileHeader, infoHeader);

}