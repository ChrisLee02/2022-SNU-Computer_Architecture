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

typedef unsigned short SFP16;
typedef short Exponent;
typedef unsigned short Mantissa;
typedef unsigned short Sign;


/* Add two SFP16-type numbers and return the result */




short isLargerThan(SFP16 x, SFP16 y);


void decodeFP(SFP16 x, Exponent *E, Mantissa *M, Sign *S);


void ShiftM(Mantissa *M, Exponent *E, unsigned short *G, unsigned short *R, unsigned short *S, unsigned short d);


SFP16 fpadd(SFP16 x, SFP16 y) {
    /* TODO */
    SFP16 PLUSINF = 0x7F00;
    SFP16 MINUSINF = 0xFF00;
    SFP16 NAN = 0xFF01;
    SFP16 ZERO = 0x0000;
    SFP16 MinusZERO = 0x8000;


    if (((x & 0x7F00) == 0x7F00 && (x & 0x0011) != 0x0000) ||
        ((y & 0x7F00) == 0x7F00 && (y & 0x0011) != 0x0000)) {
        return NAN;
    }


    if (x == PLUSINF) {
        if (y == MINUSINF) {
            return NAN;
        } else {
            return PLUSINF;
        }
    }

    if (x == MINUSINF) {
        if (y == PLUSINF) {
            return NAN;
        } else {
            return MINUSINF;
        }
    }

    if (x == ZERO || x == MinusZERO) {
        return y;
    }


    if (isLargerThan(y, x) == 1 ) {
        return fpadd(y, x);
    }

    Exponent Ex, Ey;
    Mantissa Mx, My;
    Sign SignX, SignY;
    unsigned short G = 0, R = 0, S = 0;
    decodeFP(x, &Ex, &Mx, &SignX);
    decodeFP(y, &Ey, &My, &SignY);

    if (Ex == Ey && Mx == My && SignX != SignY) {
        return ZERO;
    }

    unsigned short d = Ex - Ey;
    ShiftM(&My, &Ey, &G, &R, &S, d);

    if (SignX == SignY) {
        Mx = Mx + My;
    } else {
        if (G == 0 && R == 0 && S == 0) {
            Mx = Mx - My;
        } else {
            Mx = Mx - My - 1;
            unsigned short tmp = 8 - 4 * G - 2 * R - S;
            S = tmp % 2;
            tmp = tmp / 2;
            R = tmp % 2;
            tmp = tmp / 2;
            G = tmp % 2;
        }
    }



    while (Mx < 0x0100) {
        Mx = (Mx << 1) | G;
        G = R;
        R = S;
        S = 0;
        Ex--;
    }

    while (Mx > 0x01FF) {
        S = R | S;
        R = G;
        G = (Mx) & 0x0001;
        Mx = (Mx >> 1);
        Ex++;
    }




    S = R | S;
    R = G;

    if (R == 1 && (S == 1 || (Mx & 1) == 1)) {
        Mx = Mx + 1;
    }


    while (Mx > 0x01FF) {
        Mx = (Mx >> 1);
        Ex++;
    }
    SFP16 result = 0;

    if (Ex == 64) {
        if (SignX == 0) {
            return PLUSINF;
        } else {
            return MINUSINF;
        }
    }
    if (Ex <= -63) {
        short tmp = -62 - Ex;
        Mx = Mx >> tmp;
        Ex = -63;

    }

    unsigned short exp = Ex + 63;

    result = result | (Mx & 0x00FF);
    result = result | (exp << 8);
    result = result | (SignX << 15);

    return result;
}



void ShiftM(Mantissa *M, Exponent *E, unsigned short *G, unsigned short *R, unsigned short *S, unsigned short d) {
    for (short i = 0; i < d; i++) {
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
    if (exp == 0x0000) {
        *E = 1 - bias;
        *M = (0x00FF & x);
    } else {
        *E = exp - bias;
        *M = (0x00FF & x);
        *M = *M | 0x0100;
    }

}

short isLargerThan(SFP16 x, SFP16 y) {
    x = x & 0x7FFF;
    y = y & 0x7FFF;
    return (x > y) ? 1 : 0;
}
