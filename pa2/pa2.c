// ----------------------------------------------------------------
//
//   4190.308 Computer Architecture (Fall 2022)
//
//   Project #2: SFP16 (16-bit floating point) Adder
//
//   October 4, 2022
//
//   Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
//   Jaehoon Shim (mattjs@snu.ac.kr)
//   IlKueon Kang (kangilkueon@snu.ac.kr)
//   Wookje Han (gksdnrwp@snu.ac.kr)
//   Jinsol Park (jinsolpark@snu.ac.kr)
//   Systems Software & Architecture Laboratory
//   Dept. of Computer Science and Engineering
//   Seoul National University
//
// ----------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>

typedef unsigned short SFP16;
typedef short Exponent;
typedef unsigned short Mantissa;
typedef unsigned short Sign;


/* Add two SFP16-type numbers and return the result */

/*
 What should I do?
 1. 크기 비교 -> S랑 E랑 M이랑 짜르는 로직도 필요할 듯, 재귀호출로 순서 재배열, E랑 M을 그냥 잘라다가 쓰면 안됨,,
    Denormalize value 고려해야함. -> M에서 1.xx / 0.xx에서 1과 0까지 살려서 넣어야 할 듯
    E는 bit 말고 integer 처리 S는 대소비교 후 절댓값 큰 놈 부호로 결정
 2. calculate d, and shift right My with GRS extension, add Ey
 3. Mx와 My 덧셈.
 4. normalize - shift & modify E, GRS의 shitf는 G, R은 이전 비트 shift, S는 기존 R과 S의 OR 연산값
 5. 이제 GRS는 유기한다. R, S비트만 남김. Update - R은 이전 G값, S는 이전 R과 S의 OR 연산값
 6. R S를 이용해 round to even 로직으로 반올림 수행
 7. 반올림 후 다시 normalize, 필요하다면 마지막으로 라운딩
 8. 결과를 인코딩 & 리턴
*/

//지워라
void printBit(SFP16 x) {
    for (int i = 15; i >= 0; --i) {
        int result = x >> i & 1;
        printf("%d", result);
        if(i%4==0) printf(",");
    }
    printf("\n");
}


bool isLargerThan(SFP16 x, SFP16 y);


void decodeFP(SFP16 x, Exponent *E, Mantissa *M, Sign *S);


void ShiftM(Mantissa *M, Exponent *E, unsigned short *G, unsigned short *R, unsigned short *S, unsigned short d);

SFP16 encode(Sign SignX, Exponent Ex, Mantissa Mx);

SFP16 fpadd(SFP16 x, SFP16 y) {
    /* TODO */
    printBit(x);
    printBit(y);
    SFP16 PLUSINF = 0x7F00;
    SFP16 MINUSINF = 0xFF00;
    SFP16 NAN = 0xFF01;
    SFP16 ZERO = 0x0000;
    SFP16 MinusZERO = 0x8000;

    // 여기서 최상단 예외 처리

    if ( ((x&0x7F00) == 0x7F00 && (x&0x0011) != 0x0000) || ((y&0x7F00) == 0x7F00 && (y&0x0011) != 0x0000)  ) { //NAN case
        return NAN;
    }

    // x랑 y는 둘 다 NAN이 아니다.

    if (x == PLUSINF) {
        if(y == MINUSINF) {
            return NAN;
        }
        else {
            return PLUSINF;
        }
    }

    if (x == MINUSINF) {
        if(y == PLUSINF) {
            return NAN;
        }
        else {
            return MINUSINF;
        }
    }

    if (x == ZERO || x == MinusZERO) {
        return y;
    }



    if (isLargerThan(y, x)) {
        return fpadd(y, x);
    }
    // 예외 처리는 나중에 윗단에서 컷하는 식으로, 일단 정상적인 케이스를 가정하고 로직을 짠다.



    Exponent Ex, Ey;
    Mantissa Mx, My;
    Sign SignX, SignY; // 1이면 음수, 0이면 양수
    unsigned short G = 0, R = 0, S = 0;
    decodeFP(x, &Ex, &Mx, &SignX);
    decodeFP(y, &Ey, &My, &SignY);

    if(Ex == Ey && Mx == My && SignX != SignY) { // x - x = 0 예외처리
        return ZERO;
    }
    /*printf("before shift My\n");
    printBit(My);
*/
    unsigned short d = Ex - Ey;
    ShiftM(&My, &Ey, &G, &R, &S, d);

    /*printf("after shift My\n");
    printBit(My);
    printf("%d %d %d\n", G, R ,S);
*/
    if (SignX == SignY) {
        Mx = Mx + My; // GRS는 신경 안씀
    } else { // GRS 처리해야됨 시발
        if(G == 0 && R == 0 && S == 0) {
            Mx = Mx - My;
        }
        else {
            Mx = Mx - My - 1;
            unsigned short tmp = 8 - 4*G - 2*R - S;
            S = tmp % 2;
            tmp = tmp/2;
            R = tmp % 2;
            tmp = tmp/2;
            G = tmp % 2;
        }
    }
    /*printf("연산 후 M\n");
    printBit(Mx);
    printf("%d %d %d\n", G, R ,S);

    printf("before: %d\n", Ex);
*/
    // normalize step -> x - x = 0이 되는 케이스는 위에서 예외처리
    while (Mx < 0x0100) { //Shift left
        Mx = (Mx << 1) | G;
        G = R;
        R = S;
        S = 0;
        Ex--;
    }

    while (Mx > 0x01FF) { // Shift Right
        S = R | S;
        R = G;
        G = (Mx) & 0x0001;
        Mx = (Mx >> 1);
        Ex++;
    }
    /*printf("after: %d\n", Ex);

    printf("normalize Mx\n");
    printBit(Mx);
    printf("%d %d %d\n", G, R ,S);

*/

    // GRS 유기
    S = R | S;
    R = G;
    /*printf("%d %d\n", R ,S);
*/
    if(R == 1 && (S == 1 || (Mx & 1) == 1) ) {
        Mx = Mx + 1;
    }
   /* printf("round Mx\n");
    printBit(Mx);
    printf("Ex: %d\n", Ex);
*/
    while (Mx > 0x01FF) { // Shift Right
        Mx = (Mx >> 1);
        Ex++;
    }
    SFP16 result = 0;
    //여기서 denormalize / normalize / inf 여부 판단해야 함.
    //E값을 기준으로 판단하면 된다. normalize 기준으로 E값은 최소 -62 ~ 63까지 가능
    //E값이 64가 되는 순간 inf로 판단한다. NAN은 예외처리에서 걸러내야함.
    //E값이 -63일 때, M값 한 번 밀어내고 denormalize로 인코딩해줘야함.
    if(Ex == 64) {
        if(SignX == 0) {
            return PLUSINF;
        }
        else {
            return MINUSINF;
        }
    }
    if(Ex <= -63) {
        short tmp = -62 - Ex;
        Mx = Mx >> tmp;
        Ex = -63;

    }

    unsigned short exp = Ex + 63;
 /*   printf("exp: %d\n", exp);
 */   result = result | (Mx & 0x00FF);
    result = result | (exp << 8);
    result = result | (SignX << 15);
    printf("answer:");
    printBit(result);
    return result;
}

SFP16 encode(Sign SignX, Exponent Ex, Mantissa Mx) {
    unsigned short bias = 63;

    return 0;
}

void ShiftM(Mantissa *M, Exponent *E, unsigned short *G, unsigned short *R, unsigned short *S, unsigned short d) {
    for (int i = 0; i < d; i++) {
        *S = *R | *S;
        *R = *G;
        *G = (*M) & 0x0001;
        *M = (*M >> 1);
    }
    *E = *E + d;
}


void decodeFP(SFP16 x, Exponent *E, Mantissa *M, Sign *S) {
    unsigned short bias = 63;
    *S = (x & 0x8000) == 0x8000 ? 1 : 0;
    SFP16 exp = (x & 0x7FFF) >> 8;
    if (exp == 0x0000) { // denormalized case
        *E = 1 - bias;
        *M = (0x00FF & x); // 0.xxx -> 9 bit
    } else { // normalized case
        *E = exp - bias;
        *M = (0x00FF & x); // 1.xxx -> 9 bit
        *M = *M | 0x0100;
    }

}

bool isLargerThan(SFP16 x, SFP16 y) { // return true if magnitude of x is larger than y
    x = x & 0x7FFF;
    y = y & 0x7FFF;
    return x > y;
}

