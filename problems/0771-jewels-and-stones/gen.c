/* Small cases draw from one to three letters in both cases (such as
 * "gGqQ"), so that a jewel's other case turns up often among the stones:
 * up to 4 jewels and up to 8 stones. Full-size cases have 50 stones and 1
 * to 50 jewels from all 52 letters; the seed picks stones of any letter,
 * only jewels, no jewels at all, or mostly the other case of a jewel. */
static char other_case(char c)
{
    return (char)(c >= 'a' ? c - 'a' + 'A' : c - 'A' + 'a');
}

void gen(int small)
{
    char pool[53], jewels[51], stones[51], outside[53], c;
    int idx[52], np, nj, ns, no = 0, i, k, shape;

    if (small) {
        k = (int)rnd(1, 3);
        for (i = 0; i < 26; i++)
            idx[i] = i;
        rnd_shuffle(idx, 26);
        for (i = 0; i < k; i++) {
            pool[2 * i] = (char)('a' + idx[i]);
            pool[2 * i + 1] = (char)('A' + idx[i]);
        }
        np = 2 * k;
    } else {
        for (i = 0; i < 26; i++) {
            pool[i] = (char)('a' + i);
            pool[26 + i] = (char)('A' + i);
        }
        np = 52;
    }
    pool[np] = '\0';

    for (i = 0; i < np; i++)
        idx[i] = i;
    rnd_shuffle(idx, np);
    nj = small ? (int)rnd(1, np < 4 ? np : 4) : (int)rnd(1, 50);
    for (i = 0; i < nj; i++)
        jewels[i] = pool[idx[i]];
    jewels[nj] = '\0';
    for (i = nj; i < np; i++)
        outside[no++] = pool[idx[i]];
    outside[no] = '\0';

    ns = small ? (int)rnd(1, 8) : 50;
    shape = small ? 0 : (int)rnd(0, 3);
    for (i = 0; i < ns; i++) {
        if (shape == 1 || (shape == 3 && rnd_chance(30))) {
            c = rnd_char(jewels);
        } else if (shape == 2) {
            c = rnd_char(outside);
        } else if (shape == 3) {
            c = other_case(rnd_char(jewels));
        } else {
            c = rnd_char(pool);
        }
        stones[i] = c;
    }
    stones[ns] = '\0';
    put_string(jewels);
    put_string(stones);
}
