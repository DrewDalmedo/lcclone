/* Two decimal numbers. Small cases have up to 6 digits each: "0", all 9s,
 * random digits, digits that are mostly 9s (partial carry chains), or a
 * pair whose sum is a power of ten (a carry out of every column from the
 * lowest nonzero digit up). Full-size ones have 10^4 digits, in a shape the
 * seed picks: two random numbers; a pair whose sum is 10^(10^4); 10^4 nines
 * plus a single digit; "0" and a random number; or two random numbers
 * whose top digits are small enough that nothing carries out of the top. */

/* s gets a number of len digits: a top digit from 1 to top_hi, then digits
 * drawn from `rest`. */
static void random_number(char *s, int len, int top_hi, const char *rest)
{
    int i;

    s[0] = (char)('0' + rnd(1, top_hi));
    for (i = 1; i < len; i++)
        s[i] = rnd_char(rest);
    s[len] = '\0';
}

/* t = 10^len - s, where s has len digits and a top digit of at most 8, so
 * that t has len digits too. */
static void complement(const char *s, char *t, int len)
{
    int p = len - 1, i;

    while (s[p] == '0')
        p--;
    for (i = 0; i < p; i++)
        t[i] = (char)('9' - s[i] + '0');
    t[p] = (char)('0' + 10 - (s[p] - '0'));
    for (i = p + 1; i < len; i++)
        t[i] = '0';
    t[len] = '\0';
}

static void small_number(char *s, int len)
{
    switch (rnd(0, 4)) {
    case 0:
        strcpy(s, "0");
        break;
    case 1:
        memset(s, '9', (size_t)len);
        s[len] = '\0';
        break;
    case 2:
        random_number(s, len, 9, "0999");
        break;
    default:
        random_number(s, len, 9, "0123456789");
        break;
    }
}

void gen(int small)
{
    static char a[10001], b[10001];
    static const char digits[] = "0123456789";
    int max = small ? 6 : 10000, la, lb, shape;

    if (small) {
        la = (int)rnd(1, max);
        lb = (int)rnd(1, max);
        if (rnd_chance(20)) {
            random_number(a, la, 8, digits);
            complement(a, b, la);
        } else {
            small_number(a, la);
            small_number(b, lb);
        }
    } else {
        shape = (int)(gen_seed() % 5);
        if (shape == 0) {
            random_number(a, max, 9, digits);
            random_number(b, max, 9, digits);
        } else if (shape == 1) {
            random_number(a, max, 8, digits);
            complement(a, b, max);
        } else if (shape == 2) {
            memset(a, '9', (size_t)max);
            a[max] = '\0';
            random_number(b, 1, 9, digits);
        } else if (shape == 3) {
            strcpy(a, "0");
            random_number(b, max, 9, digits);
        } else {
            random_number(a, max, 4, digits);
            random_number(b, max, 4, digits);
        }
    }
    if (rnd_chance(50)) {
        put_string(a);
        put_string(b);
    } else {
        put_string(b);
        put_string(a);
    }
}
