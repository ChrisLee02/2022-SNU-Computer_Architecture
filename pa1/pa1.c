//---------------------------------------------------------------
//
//  4190.308 Computer Architecture (Fall 2022)
//
//  Project #1:
//
//  September 6, 2022
//
//  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
//  Jaehoon Shim (mattjs@snu.ac.kr)
//  IlKueon Kang (kangilkueon@snu.ac.kr)
//  Wookje Han (gksdnrwp@snu.ac.kr)
//  Jinsol Park (jinsolpark@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//---------------------------------------------------------------



// 기존 000000... 에다가 쓰고자 하는 bit 101010.... 로 or 를 취하면 쓰고자 하는 bit 값이 들어간다.
// 기존 101010...에다가 0000000....로 or를 취하면 기존 값이 유지가 된다.
// 11111... 로 or를 취하면 11111로 초기화, 00000....로 and를 취하면 0으로 초기화
// 내 코드에서는 0으로 초기화한 뒤 쓰는 쪽으로 설계
// -> 이것과 >>, << 연산자를 이용해서 bit writing function을 만들어야 한다.
// bit 단위의 bitPos 변수를 이용하자. 8을 기준으로 몫(p + n으로 접근)과 나머지(비트 단위의 칸을 의미)를 이용하면 된다.
// 인자로 넘겨줄 값은? u8 변수의 값과 몇 비트짜리인지에 대한 정보.
// 비트를 끊어서 써주는 경우를 신경써줘야 한다.



typedef unsigned char u8;

u8 getFilteredValue(const u8 *src, int i, int j, int width);
u8 getValue(const u8 *src, int i, int j, int width);
void bitWriter(int *bitPos, u8 bitValue, int bitSize, u8 *result);
int calN(u8 maxDelta);


/* TODO: Implement this function */
int encode(const u8 *src, int width, int height, u8 *result) {
    if (width == 0 || height == 0) return 0;

    int bitPos = 0;


    u8 filter, min, max; // filter 값을 임시 저장할 변수, filter 값의 min, max
    int n; // delta에 필요한 비트 개수

    for (int i = 0; i < height; i++) {

        min = getFilteredValue(src, i, 0, width);
        max = min;

        for (int j = 1; j < width; j++) {
            filter = getFilteredValue(src, i, j, width);

            if (filter > max) max = filter;

            if (filter < min) min = filter;

        }

        n = calN(max - min);
        bitWriter(&bitPos, min, 8, result);
        bitWriter(&bitPos, n, 4, result);

        //printf("%d %d %d\n", min, max, n);
        //print_ans1(result, 9);
        if (n == 0) continue;

        for (int j = 0; j < width; j++) {
           // printf("delta: %d\n", getFilteredValue(src, i, j, width) - min );
            bitWriter(&bitPos,
                      getFilteredValue(src, i, j, width) - min,
                      n, result);
            //print_ans1(result, 9);

        }

    }
    if(bitPos%8 != 0) bitWriter(&bitPos, 0, 8-bitPos%8, result);
    return (bitPos + 1) / 8;
}


u8 getValue(const u8 *src, int i, int j, int width) {
    return *(src+i*width+j);
}

int calN(u8 maxDelta) {
    if (maxDelta == 0) return 0;
    else if (maxDelta == 1) return 1;
    else if (maxDelta < 4) return 2;
    else if (maxDelta < 8) return 3;
    else if (maxDelta < 16) return 4;
    else if (maxDelta < 32) return 5;
    else if (maxDelta < 64) return 6;
    else if (maxDelta < 128) return 7;
    else return 8;
}

void bitWriter(int *bitPos, u8 bitValue, int bitSize, u8 *result) {


    int tmpBitPos = *bitPos % 8; // 한 바이트 내에서의 0~7 사이 position
    int bytePos = *bitPos / 8;

    if((*bitPos + bitSize)/8 != bytePos) {
        int frontShift = 8 - tmpBitPos; //8
        int backShift = bitSize - frontShift; //0

        u8 frontMask = 255 << frontShift;
        u8 backMask = 255 >> backShift;

        *(result + bytePos) = *(result + bytePos) & frontMask;
        *(result + bytePos + 1) = *(result + bytePos + 1) & backMask;

        u8 frontValue = bitValue >> (bitSize - frontShift);
        u8 backValue = bitValue << (8 - backShift);

        *(result + bytePos) = *(result + bytePos) | frontValue;
        *(result + bytePos + 1) = *(result + bytePos + 1) | backValue;
    }
    else {
        int leftShift = 8 - bitSize; //7
        int rightShift = tmpBitPos;
        u8 mask = 255 << leftShift;
        mask = mask >> rightShift;
        mask = ~mask;
        *(result + bytePos) = *(result + bytePos) & mask;
        u8 value = ((bitValue << leftShift) >> rightShift);
        *(result + bytePos) = *(result + bytePos) | value;
    }

    *bitPos = *bitPos + bitSize;

}

u8 getFilteredValue(const u8 *src, int i, int j, int width) {
    int avg;
    int sum = 0;
    int cnt = 0;
    u8 value = getValue(src, i, j, width);
    if(i-1 >= 0) {
        cnt++;
        sum += getValue(src, i-1, j, width);
    }
    if(j-1 >= 0) {
        cnt++;
        sum += getValue(src, i, j-1, width);
    }
    if(i-1 >=0 && j-1 >=0) {
        cnt++;
        sum += getValue(src, i-1, j-1, width);
    }
    avg = cnt==0 ? 0 : sum/cnt;
    return value>=avg ? value - avg : value + (256 - avg);
}
