/* Small cases have up to 12 letters, full-size ones 1000. The seed picks
 * the shape:
 *   0. letters drawn from a few letters of the alphabet (1 to 4 when
 *      small, any when full size), each in either form;
 *   1. every letter keeps one form, so none is doubled, and sometimes one
 *      letter, often not the greatest present, also gets a copy in its
 *      other form;
 *   2. small: only a, b, y and z, in either form; full size: like 1, with
 *      every letter of the alphabet present;
 *   3. small: any of the 52 letters; full size: one letter has 256 or 512
 *      copies of one form and one of the other, which breaks a counter
 *      kept in a char, among letters that each keep one form, and the
 *      letter just below it is doubled too. */
static void put_letters(int *a, int n)
{
    char s[1001];
    int i;

    rnd_shuffle(a, n);
    for (i = 0; i < n; i++)
        s[i] = (char)a[i];
    s[n] = '\0';
    put_string(s);
}

static int form(int letter, int capital)
{
    return (capital ? 'A' : 'a') + letter;
}

void gen(int small)
{
    int a[1000], letters[26], fixed[26];
    int n = small ? (int)rnd(1, 12) : 1000;
    int shape = (int)rnd(0, 3), k, i, j, t, up, letter, count;

    for (i = 0; i < 26; i++) {
        letters[i] = i;
        fixed[i] = rnd_chance(50);
    }
    rnd_shuffle(letters, 26);
    if (shape == 0) {
        k = small ? (int)rnd(1, 4) : (int)rnd(1, 26);
        for (i = 0; i < n; i++) {
            j = (int)rnd(0, k - 1);
            up = rnd_chance(50);
            a[i] = form(letters[j], up);
        }
    } else if (shape == 1 || (shape == 2 && !small)) {
        k = small ? (int)rnd(1, 6) : shape == 2 ? 26 : (int)rnd(1, 26);
        for (i = 0; i < n; i++) {
            j = i < k ? i : (int)rnd(0, k - 1);
            a[i] = form(letters[j], fixed[letters[j]]);
        }
        if (rnd_chance(60)) {
            j = (int)rnd(0, k - 1);
            i = (int)rnd(0, n - 1);
            a[i] = form(letters[j], !fixed[letters[j]]);
        }
    } else if (shape == 2) {
        for (i = 0; i < n; i++) {
            t = (int)rnd(0, 3);
            letter = t < 2 ? t : 22 + t;
            up = rnd_chance(50);
            a[i] = form(letter, up);
        }
    } else if (small) {
        for (i = 0; i < n; i++) {
            letter = (int)rnd(0, 25);
            up = rnd_chance(50);
            a[i] = form(letter, up);
        }
    } else {
        letter = (int)rnd(1, 25);
        count = rnd_chance(50) ? 256 : 512;
        up = rnd_chance(50);
        for (i = 0; i < count; i++)
            a[i] = form(letter, up);
        a[i++] = form(letter, !up);
        a[i++] = form(letter - 1, 0);
        a[i++] = form(letter - 1, 1);
        for (; i < n; i++) {
            do
                t = (int)rnd(0, 25);
            while (t == letter || t == letter - 1);
            a[i] = form(t, fixed[t]);
        }
    }
    put_letters(a, n);
}
