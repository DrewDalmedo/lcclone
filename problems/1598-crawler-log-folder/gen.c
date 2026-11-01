/* A log of folder changes. Small cases have up to 8 entries, often "../"
 * (so the walk keeps hitting the top folder) and "./", with names of one
 * or two characters from a, b and 7, so that names repeat and have the
 * lengths of "./" and "../". Full-size ones have 1000 entries with names of
 * 1 to 9 letters and digits, in one of four shapes picked by the seed: a
 * mix, a deep walk of mostly downward moves, a run of "../" at the top
 * folder before the mix, or a walk that keeps bouncing off the top. */
static void entry(char *s, int up, int stay, int small)
{
    int r = (int)rnd(0, 99), len, k;

    if (r < up) {
        strcpy(s, "../");
    } else if (r < up + stay) {
        strcpy(s, "./");
    } else {
        len = (int)(small ? rnd(1, 2) : rnd(1, 9));
        for (k = 0; k < len; k++)
            s[k] = rnd_char(small ? "ab7" : "abcdefghijklmnopqrstuvwxyz0123456789");
        s[len] = '\0';
        strcat(s, "/");
    }
}

void gen(int small)
{
    static char text[1000][11];
    char *logs[1000];
    int n = small ? (int)rnd(1, 8) : 1000, shape = (int)rnd(0, 3), lead, i;

    lead = !small && shape == 2 ? (int)rnd(1, 100) : 0;
    for (i = 0; i < n; i++) {
        logs[i] = text[i];
        if (small)
            entry(text[i], 35, 20, 1);
        else if (i < lead)
            strcpy(text[i], "../");
        else if (shape == 1)
            entry(text[i], 8, 4, 0);
        else if (shape == 3)
            entry(text[i], 50, 5, 0);
        else
            entry(text[i], 35, 15, 0);
    }
    put_strings(logs, n);
}
