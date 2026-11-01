/* Small cases: both ends from 0 to 30, often equal. Full-size cases: the
 * seed picks a range from near 0 to near 10^9 (as slow as it gets for a
 * loop over the numbers), a single number, or two random ends. */
void gen(int small)
{
    long long low, high, t;

    if (small) {
        low = rnd(0, 30);
        high = rnd_chance(25) ? low : rnd(low, 30);
    } else {
        switch ((int)(gen_seed() % 3)) {
        case 0:
            low = rnd(0, 3);
            high = rnd(1000000000 - 3, 1000000000);
            break;
        case 1:
            low = rnd(0, 1000000000);
            high = low;
            break;
        default:
            low = rnd(0, 1000000000);
            t = rnd(0, 1000000000);
            high = t;
            if (t < low) {
                high = low;
                low = t;
            }
            break;
        }
    }
    put_int(low);
    put_int(high);
}
