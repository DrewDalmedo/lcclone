/* Small cases have 2 to 5 numbers and up to 4 short words, the numbers
 * rising (often across 9 and 10), rising with one pair broken (equal,
 * smaller or swapped), or random values up to 12 so that ties come up.
 * Full-size cases fill s to about 200 characters, in a shape the seed
 * picks: rising numbers (true), rising with only the last number out of
 * place, rising with one break somewhere, or 100 one-character pieces
 * with up to nine one-digit numbers among them. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz";

/* Makes val[i] no larger than val[i - 1]: equal, smaller, or swapped. */
static void spoil(int *val, int i)
{
    int how = (int)rnd(0, 2), t;

    if (how == 0) {
        val[i] = val[i - 1];
    } else if (how == 1) {
        val[i] = (int)rnd(1, val[i - 1]);
    } else {
        t = val[i];
        val[i] = val[i - 1];
        val[i - 1] = t;
    }
}

void gen(int small)
{
    static const int word_pct[] = {0, 20, 50, 80};
    int kind[100], val[100], wlen[100], digits[9];
    char s[256];
    int ntok = 0, m = 0, len = 0, prev = 0, i, k, t, v, mode, shape, pct;
    int step_max, is_num, tlen;

    if (small) {
        m = (int)rnd(2, 5);
        ntok = m + (int)rnd(0, 4);
        for (t = 0; t < ntok; t++) {
            kind[t] = t < m;
            wlen[t] = (int)rnd(1, 3);
        }
        rnd_shuffle(kind, ntok);
        mode = (int)rnd(0, 2);
        for (i = 0; i < m; i++) {
            if (mode == 2) {
                val[i] = (int)rnd(1, 12);
            } else {
                v = (int)rnd(1, 9);
                val[i] = prev + v;
                prev = val[i];
            }
        }
        if (mode == 1) {
            i = (int)rnd(1, m - 1);
            spoil(val, i);
        }
    } else {
        shape = (int)(gen_seed() % 4);
        if (shape == 3) {
            /* 100 one-character pieces. */
            ntok = 100;
            m = (int)rnd(2, 9);
            for (i = 0; i < 9; i++)
                digits[i] = i + 1;
            rnd_shuffle(digits, 9);
            for (i = 1; i < m; i++) /* sort the first m */
                for (k = i; k > 0 && digits[k - 1] > digits[k]; k--) {
                    t = digits[k];
                    digits[k] = digits[k - 1];
                    digits[k - 1] = t;
                }
            for (i = 0; i < m; i++)
                val[i] = digits[i];
            if (rnd_chance(50)) {
                i = (int)rnd(1, m - 1);
                spoil(val, i);
            }
            for (t = 0; t < ntok; t++) {
                kind[t] = t < m;
                wlen[t] = 1;
            }
            rnd_shuffle(kind, ntok);
        } else {
            pct = word_pct[rnd(0, 3)];
            step_max = (int)rnd(1, 4);
            for (;;) {
                /* The first numbers come by the fourth piece at the latest. */
                is_num = (m < 2 && ntok >= 2) || !rnd_chance(pct);
                if (is_num) {
                    v = (int)rnd(1, step_max);
                    v += prev;
                    if (v > 99)
                        break;
                    tlen = v >= 10 ? 2 : 1;
                } else {
                    tlen = (int)rnd(1, 5);
                }
                if (ntok == 100 || len + (ntok > 0) + tlen > 200)
                    break;
                len += (ntok > 0) + tlen;
                kind[ntok] = is_num;
                wlen[ntok] = tlen;
                ntok++;
                if (is_num) {
                    val[m++] = v;
                    prev = v;
                }
            }
            if (shape == 1) {
                spoil(val, m - 1);
            } else if (shape == 2) {
                i = (int)rnd(1, m - 1);
                spoil(val, i);
            }
        }
    }

    len = 0;
    for (t = 0, i = 0; t < ntok; t++) {
        if (t > 0)
            s[len++] = ' ';
        if (kind[t]) {
            len += sprintf(s + len, "%d", val[i++]);
        } else {
            for (k = 0; k < wlen[t]; k++)
                s[len++] = rnd_char(letters);
        }
    }
    s[len] = '\0';
    put_string(s);
}
