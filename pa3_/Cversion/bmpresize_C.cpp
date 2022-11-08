#include <stdio.h>
#include<stdlib.h>

unsigned char getAvg(unsigned int *curimgptr, int w_in, int k, int RGB);
unsigned char getValue(unsigned int *imgptr, int y);
int mul(int i, int j);


void bmpresize(unsigned int *imgptr, int h, int w, int k, unsigned int *outptr) {
    // 실제로는 imgptr을 한 번에 4개 단위로 가져오고, 4개 단위로 outptr에 써야한다.
    //int step = 1 << k;
    int i, j, h_out, w_out;
    h_out = h >> k;
    int w_in = (w << 1) + w;
    // w_in = w*3;
    w_out = (w >> k); // RGB 3배해준 너비
    w_out = (w_out << 1) + w_out;
    // w_out = (w>>k) * 3
    unsigned int *curimgptr;
    unsigned int *curoutptr;
    int padsize = w_out & 3;
    padsize = 4 - padsize;
    padsize = padsize & 3;
    //padsize = (4 - w_out%4) % 4;

    //printf("%d %d %d\n", h_out, w_out, padsize);
    //for(int i = 0; i<h_out; i++)
    i = 0;
    if (i >= h_out) {
        goto iLoopDone;
    }
    iLoop:

        //for(int j = 0; j<w_out; j+=3)
        j = 0;
        if (j >= w_out) {
            goto jFirstLoopDone;
        }
        jFirstLoop:
            /*
            curimgptr = imgptr + (i << k) * (w_in + (4 - w_in % 4) % 4) + ((j / 3) << k) * 3;
            curoutptr = outptr + i * (w_out + padsize) + j;
            curoutptr[0] = getAvg(curimgptr, w_in, k, 0);
            curoutptr[1] = getAvg(curimgptr, w_in, k, 1);
            curoutptr[2] = getAvg(curimgptr, w_in, k, 2);
            */



            j=j+3;
            if (j != w_out) goto jFirstLoop;

        jFirstLoopDone:

        //for (; j < w_out + padsize; j++)
        if(j > w_out+padsize) {
            goto jSecondLoopDone;
        }
        jSecondLoop:
            outptr[i * (w_out + padsize) + j] = 2;
            j++;
            if (j != w_out+padsize) goto jSecondLoop;
        jSecondLoopDone:

        /*for (; j < w_out + padsize; j++) {
        outptr[i * (w_out + padsize) + j] = 2;
        //printf("2 ");
        }*/



        /*for (j = 0; j < w_out; j += 3) {
        unsigned char *curimgptr = imgptr + (i << k) * (w_in + (4 - w_in % 4) % 4) + ((j / 3) << k) * 3;
        unsigned char *curoutptr = outptr + i * (w_out + padsize) + j;

        curoutptr[0] = getAvg(curimgptr, w_in, k, 0);
        curoutptr[1] = getAvg(curimgptr, w_in, k, 1);
        curoutptr[2] = getAvg(curimgptr, w_in, k, 2);
        //  printf("1 ");
        }
        */

        //for (int p = 0; p < padsize; p++)
        /*for (; j < w_out + padsize; j++) {
        outptr[i * (w_out + padsize) + j] = 2;
        //printf("2 ");
        }*/
    i++;
    if (i != h_out) goto iLoop;

    iLoopDone:
    return;
}
unsigned char getAvg(unsigned char *curimgptr, int w_in, int k, int RGB) {
    int step = 1 << k;
    int sum = 0;
    int padsize= (4 - (w_in)%4) % 4;

    for (int i = 0; i < step; i++) {
        for (int j = 0; j < step; j++) {
            //sum += curimgptr[(a+i)*(w_in*3+padsize) + (b+j)*3 + RGB]; // 여긴 최적화 가능할듯,, imgptr을 밖에서 더해도 아무 문제 없다.
            sum += curimgptr[i*(w_in+padsize) + j*3 + RGB]; // 여긴 최적화 가능할듯,, imgptr을 밖에서 더해도 아무 문제 없다.

        }
    }
    //printf("sum: %d\n", sum);
    sum = sum >> k;
    return sum>>k;
}
unsigned char getValue(unsigned int *imgptr, int y) {

}
int mul(int i, int j) {
    int a = 1 << 30;
    int k = 30;
    int sum = 0;
    while( (a & j) == 0) {
        a = a>>1;
        k--;
    }

    while(k>=0) {
        if((a&j) !=0) {
            sum = sum + (i << k);
        }
        a = a>>1;
        k--;
    }

    return sum;
}


int main(void) {
    unsigned int imgptr[20] = {0x35000000, 0xf235fbf2, 0x000000fb, 0x00000000, 0x00000000, 0x35fbf235, 0xf235fbf2,
                               0xfbf235fb, 0x00000000, 0x00000000, 0x00000000, 0xf2350000, 0xfa6a0afb, 0x00fa6a0a,
                               0x00000000, 0x00000000, 0xf2350000, 0xfbf235fb, 0x00000000, 0x00000000};
    unsigned int outptr[24];

    bmpresize(imgptr, 4, 6, 1, outptr);
    printf("%d\n", mul(20,6));
    for(int i = 0; i<80; i++) {
        printf("%x ",*((unsigned char *)imgptr+i) );
        if((i+1)%20 == 0) printf("\n");
    }

    printf("\n");
    for(int i = 0; i<24; i++) {
        printf("%x ",outptr[i]);
        if(i==11) printf("\n");
    }
}