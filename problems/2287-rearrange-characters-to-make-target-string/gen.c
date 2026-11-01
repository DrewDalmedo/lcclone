/* Small cases: a target of 1 to 4 letters from "abc" (so letters
 * repeat), and an s made of 0 to 4 spellings of it with a few letters
 * taken out and a few from "abcd" put in, shuffled. Full-size cases have
 * an s of 100 letters, with the seed picking the shape: one letter
 * throughout and a target of that letter (up to 10 times), as many
 * spellings of a random target as fit plus random letters, the same with
 * one letter of target missing from s, or random letters from a small
 * alphabet for both. */
static void shuffle(char *s, int n)
{
    int i, j;
    char t;

    for (i = n - 1; i > 0; i--) {
        j = (int)rnd(0, i);
        t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
}

void gen(int small)
{
    static const char abc[] = "abcdefghijklmnopqrstuvwxyz";
    char s[101], target[11], alpha[27], missing, c;
    int n = 0, m, k, i, j, shape, drop, add;

    if (small) {
        m = (int)rnd(1, 4);
        for (i = 0; i < m; i++)
            target[i] = rnd_char("abc");
        k = (int)rnd(0, 4);
        for (i = 0; i < k; i++)
            for (j = 0; j < m; j++)
                s[n++] = target[j];
        drop = (int)rnd(0, 3);
        for (i = 0; i < drop && n > 0; i++) {
            j = (int)rnd(0, n - 1);
            s[j] = s[--n];
        }
        add = (int)rnd(0, 3);
        if (n + add == 0)
            add = 1;
        for (i = 0; i < add; i++)
            s[n++] = rnd_char("abcd");
    } else {
        n = 100;
        shape = (int)rnd(0, 3);
        k = (int)rnd(3, 26);
        for (i = 0; i < k; i++)
            alpha[i] = abc[i];
        alpha[k] = '\0';
        if (shape == 0) {
            c = rnd_char(abc);
            m = (int)rnd(1, 10);
            for (i = 0; i < m; i++)
                target[i] = c;
            for (i = 0; i < n; i++)
                s[i] = c;
        } else if (shape <= 2) {
            m = (int)rnd(1, 10);
            for (i = 0; i < m; i++)
                target[i] = rnd_char(alpha);
            for (i = 0; i + m <= n; i += m)
                memcpy(s + i, target, (size_t)m);
            for (; i < n; i++)
                s[i] = rnd_char(alpha);
            if (shape == 2) {
                missing = target[rnd(0, m - 1)];
                for (i = 0; i < n; i++)
                    while (s[i] == missing)
                        s[i] = rnd_char(abc);
            }
        } else {
            m = (int)rnd(1, 10);
            for (i = 0; i < m; i++)
                target[i] = rnd_char(alpha);
            for (i = 0; i < n; i++)
                s[i] = rnd_char(alpha);
        }
    }
    target[m] = '\0';
    shuffle(s, n);
    s[n] = '\0';
    put_string(s);
    put_string(target);
}
