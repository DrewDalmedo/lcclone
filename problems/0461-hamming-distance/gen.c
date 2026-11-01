/* Two ints from 0 to 2^31 - 1. Small cases are mostly at most 8 bits wide,
 * so that zeros, equal pairs and near misses come up often; full-size ones
 * start from a 31-bit x. The seed picks how y relates to x: unrelated and
 * of any bit length, x with one to three bits flipped, x itself, 0, or x
 * with all its bits flipped. Then x and y may swap places. */
void gen(int small)
{
    int width = small && rnd_chance(70) ? (int)rnd(1, 8)
              : small ? (int)rnd(1, 31) : 31;
    long long x = rnd(0, (1LL << width) - 1), y, t;
    int shape = (int)rnd(0, 5), k, bit, w;

    switch (shape) {
    case 0:
    case 1:
        w = (int)rnd(1, small ? width : 31);
        y = rnd(0, (1LL << w) - 1);
        break;
    case 2:
        y = x;
        for (k = (int)rnd(1, 3); k > 0; k--) {
            bit = (int)rnd(0, width - 1);
            y ^= 1LL << bit;
        }
        break;
    case 3:
        y = x;
        break;
    case 4:
        y = 0;
        break;
    default:
        y = x ^ ((1LL << width) - 1);
    }
    if (rnd_chance(50)) {
        t = x;
        x = y;
        y = t;
    }
    put_int(x);
    put_int(y);
}
