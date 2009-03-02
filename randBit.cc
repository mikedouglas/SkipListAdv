#include <ctime>
#include <cstdlib>
#include "randBit.h"

int randBit()
{
        static int bitsUpperBd = 0;
        static int bits;

        if (bitsUpperBd == 0) {
                bitsUpperBd = RAND_MAX;
                bits = rand();
        }
        int b = bits & 1;
        bits >>= 1;
        bitsUpperBd >>= 1;
        return b;
}
