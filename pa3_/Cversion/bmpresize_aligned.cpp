#include <stdio.h>
#include <stdlib.h>

unsigned char getAvg(unsigned int *imgptr, int y, int x, int k, int w_in, int RGB);

unsigned char getValue(unsigned int *imgptr, int inputByte);

int mul(int i, int j);

void write(unsigned int *outptr, int outputByte, int avgVal);

void bmpresize(unsigned int *imgptr, int h, int w, int k, unsigned int *outptr) {
    int i, j, h_out, w_out;
    h = h >> k;
    // h_out = h>>k
    w_out = (w >> k); // RGB 3배까지 감안한 너비
    w_out = (w_out << 1) + w_out;
    // w_out = (w>>k) * 3
    w = (w << 1) + w;
    // w_in = w*3;
    int padsize = w_out & 3;
    padsize = 4 - padsize;
    padsize = padsize & 3;
    //padsize = (4 - w_out%4) % 4;

    //printf("%d %d %d\n", h_out, w_out, padsize);
    for (i = 0; i < h; i++) {
        for (j = 0; j < w_out; j += 3) {
            for (int l = 0; l < 3; l++) {
                // int inputByte = (i << k) * (w_in + (4 - w_in % 4) % 4) + ((j / 3) << k) * 3;
                int avgVal = getAvg(imgptr, i << k, (j / 3) << k, k, w, l);
                int outputByte = i*(w_out + padsize) + j + l;
               /* int padsize = w_out & 3;
                padsize = 4 - padsize;
                padsize = padsize & 3;*/
                write(outptr, outputByte, avgVal);

                /*if(i==0&&j==3&&l==0) {
                    printf("%d\n", avgVal);
                    printf("%d\n", outputByte);
                    printf("%x\n", outptr[0]);
                }*/
            }
        }
        for (; j < w_out + padsize; j++) {
            int outputByte = i*(w_out + padsize) + j;
            write(outptr, outputByte, 0);
        }

        // getAvgVal 로직: input의 [i][j] pixel과 RGB값에 대해, 평균값을 구한다!
        // i, j, BGR(0,1,2)에 대해 루프 돌려 -> getVal호출해서 더하고 나눈값 반환
        // getVal 로직:

        // write 로직: 1byte 값을 outputByte에 쓴다!
        // 1. mask: 0x000000FF와 value을 각각 <<outputByteOrder 해준다.
        // 2. mask에 not연산 때리고, (outputIndex)outptr 값과 and를 때려서 그 한 바이트만 0으로 초기화
        // 3. 초기화한 그 값과 shift해둔 value를 or연산 때리기
        // 4. 결과값을 outptr에 쓴다.
    }

}

void write(unsigned int *outptr, int outputByte, int avgVal) {
    int outputByteOrder = outputByte%4;
    int outputIndex = outputByte - outputByte%4;
    int val = *(outptr + outputIndex/4);
    int mask = 0x000000FF << (outputByteOrder*8);
    avgVal = avgVal << (outputByteOrder*8);
    mask = ~mask;
    val = val & mask;
    val = val | avgVal;
    *(outptr + outputIndex/4) = val;
}

/*int divideby3(int num)
{
    int sum = 0;
    while (num > 3) {
        sum = (num >> 2) + sum;
        num = (num >> 2) + (num & 3);
    }
    if (num == 3)
        sum = add(sum, 1);
    return sum;
}*/

int mul(int i, int j) {
    int a = 1 << 30;
    int k = 30;
    int sum = 0;
    while ((a & j) == 0) {
        a = a >> 1;
        k--;
    }

    while (k >= 0) {
        if ((a & j) != 0) {
            sum = sum + (i << k);
        }
        a = a >> 1;
        k--;
    }

    return sum;
}

unsigned char getAvg(unsigned int *imgptr, int y, int x, int k, int w_in, int RGB) {
    int step = 1 << k;
    int sum = 0;
    int padsize = (4 - (w_in) % 4) % 4;
    int w_in_with_padding = w_in + padsize;
    //x = x*3;
    for (int i = y; i < step + y; i++) {
        //for (int j = x*3; j < step*3 + x*3; j+=3)
        for(int j = x; j < step + x; j++){
            int inputByte = i * w_in_with_padding + j*3 + RGB;
            /*if(y==0&&x==2&&RGB==0) {
                printf("%d %d %d\n", i,j, inputByte);
            }*/
            sum = sum + getValue(imgptr, inputByte);
        }
    }
    //printf("sum: %d\n", sum);
    sum = sum >> k;
    return sum >> k;
}

unsigned char getValue(unsigned int *imgptr, int inputByte) {
    //int inputByteOrder = inputByte % 4;
    int inputByteOrder = inputByte & 3;
    int inputIndex = inputByte - inputByte % 4;
    unsigned int val = *(imgptr + inputIndex / 4);
    int mask = 0x000000FF << (inputByteOrder<<3);
    val = val & mask;
    val = val >> (inputByteOrder*8);
    return (unsigned char)val;
}

/*
unsigned char getAvg(unsigned int *curimgptr, int w_in, int k, int RGB) {
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
*/


int main(void) {
    /*unsigned int imgptr[20] = {0x35000000, 0xf235fbf2, 0x000000fb, 0x00000000, 0x00000000, 0x35fbf235, 0xf235fbf2,
                               0xfbf235fb, 0x00000000, 0x00000000, 0x00000000, 0xf2350000, 0xfa6a0afb, 0x00fa6a0a,
                               0x00000000, 0x00000000, 0xf2350000, 0xfbf235fb, 0x00000000, 0x00000000};
    unsigned int outptr[6];

    bmpresize(imgptr, 4, 6, 1, outptr);

    for (int i = 0; i < 80; i++) {
        printf("%x ", *((unsigned char *) imgptr + i));
        if ((i + 1) % 20 == 0) printf("\n");
    }

    printf("\n");
    for (int i = 0; i < 24; i++) {
        printf("%x ", *((unsigned char *) outptr + i));
        if (i == 11) printf("\n");
    }*/
    unsigned int imgptr[12] = {0x01020304, 0x01020304, 0x01020304, 0x01020304, 0x01020304,
                               0x01020304, 0x01020304, 0x01020304, 0x01020304, 0x01020304, 0x01020304, 0x01020304};
    unsigned int outptr[4];

    bmpresize(imgptr, 4, 4, 1, outptr);

    for (int i = 0; i < 12; i++) {
        printf("%08x\n", imgptr[i]);
    }

    printf("\n");
    for (int i = 0; i < 4; i++) {
        printf("%08x\n", outptr[i]);
    }
    /*ans1:
    .word 0x02020302
    .word 0x00000201
    .word 0x02020302
    .word 0x00000201*/


}