/* Each ring is drawn as a number colour * 10 + rod, then written out as
 * two characters. Small cases drop 1 to 12 rings onto one to three rods
 * (rods 0 and 9 often among them), so that complete rods, rods with
 * repeats of one or two colours and rods completed by the last ring come
 * up often. Full-size ones are 100 rings, in a shape the seed picks:
 * random rings on all ten rods, every rod complete, every rod missing one
 * colour, or every rod missing one colour except one rod that the last
 * ring completes. */
static const char colour[] = "RGB";

void gen(int small)
{
    int ring[100], rods[3], n, k, i, c, r;
    char s[201];

    if (small) {
        n = (int)rnd(1, 12);
        k = (int)rnd(1, 3);
        for (i = 0; i < k; i++)
            rods[i] = rnd_chance(40) ? 9 * (int)rnd(0, 1) : (int)rnd(0, 9);
        for (i = 0; i < n; i++) {
            c = (int)rnd(0, 2);
            r = (int)rnd(0, k - 1);
            ring[i] = c * 10 + rods[r];
        }
    } else {
        n = 100;
        switch (gen_seed() % 4) {
        case 0: /* random rings on all ten rods */
            for (i = 0; i < n; i++) {
                c = (int)rnd(0, 2);
                r = (int)rnd(0, 9);
                ring[i] = c * 10 + r;
            }
            break;
        case 1: /* every rod complete, and 70 more rings anywhere */
            for (i = 0; i < 30; i++)
                ring[i] = i / 10 * 10 + i % 10;
            for (i = 30; i < n; i++) {
                c = (int)rnd(0, 2);
                r = (int)rnd(0, 9);
                ring[i] = c * 10 + r;
            }
            rnd_shuffle(ring, n);
            break;
        case 2: /* each rod lacks one colour, so none is complete */
            for (i = 0; i < n; i++) {
                r = (int)rnd(0, 9);
                c = (r + (int)rnd(1, 2)) % 3; /* never colour r % 3 */
                ring[i] = c * 10 + r;
            }
            break;
        default: /* one rod finished by the last ring, the rest partial */
            r = (int)rnd(0, 9);
            c = (int)rnd(0, 2);
            for (i = 0; i < n - 1; i++) {
                k = (int)rnd(0, 9);
                if (k == r)
                    ring[i] = ((c + (int)rnd(1, 2)) % 3) * 10 + k;
                else
                    ring[i] = ((k + (int)rnd(1, 2)) % 3) * 10 + k;
            }
            ring[0] = ((c + 1) % 3) * 10 + r;
            ring[1] = ((c + 2) % 3) * 10 + r;
            ring[n - 1] = c * 10 + r;
            break;
        }
    }
    for (i = 0; i < n; i++) {
        s[2 * i] = colour[ring[i] / 10];
        s[2 * i + 1] = (char)('0' + ring[i] % 10);
    }
    s[2 * n] = '\0';
    put_string(s);
}
