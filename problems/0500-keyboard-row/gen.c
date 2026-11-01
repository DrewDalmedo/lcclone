/* Words that mostly keep to one keyboard row, some of them spoiled by one
 * letter from another row, in lowercase, uppercase, capitalised or mixed
 * case. A word is sometimes a repeat of an earlier one, in the same case or
 * another. Small cases have up to 6 words of up to 5 letters; full-size
 * ones have 20 words, half of them 100 letters long, and a spoiled word
 * often has its odd letter last. */
static const char *rows[] = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};

static char recase(char c, int mode, int pos)
{
    int up;

    if (mode == 0)
        up = 0;
    else if (mode == 1)
        up = 1;
    else if (mode == 2)
        up = pos == 0;
    else
        up = rnd_chance(50);
    return up ? (char)(c - 'a' + 'A') : c;
}

void gen(int small)
{
    static char words[20][101];
    char *ptrs[20];
    int n = small ? (int)rnd(1, 6) : 20, i, k, len, r, other, pos, mode;
    char c;

    for (k = 0; k < n; k++) {
        if (k > 0 && rnd_chance(20)) {
            i = (int)rnd(0, k - 1);
            mode = (int)rnd(0, 4);
            len = (int)strlen(words[i]);
            for (pos = 0; pos < len; pos++) {
                c = (char)tolower((unsigned char)words[i][pos]);
                words[k][pos] = mode == 4 ? words[i][pos] : recase(c, mode, pos);
            }
            words[k][len] = '\0';
        } else {
            if (small)
                len = (int)rnd(1, 5);
            else
                len = rnd_chance(50) ? 100 : (int)rnd(1, 100);
            r = (int)rnd(0, 2);
            mode = (int)rnd(0, 3);
            for (pos = 0; pos < len; pos++) {
                c = rows[r][rnd(0, (long long)strlen(rows[r]) - 1)];
                words[k][pos] = recase(c, mode, pos);
            }
            words[k][len] = '\0';
            if (rnd_chance(small ? 40 : 50)) {
                other = (r + (int)rnd(1, 2)) % 3;
                pos = rnd_chance(50) ? len - 1 : (int)rnd(0, len - 1);
                c = rows[other][rnd(0, (long long)strlen(rows[other]) - 1)];
                words[k][pos] = recase(c, mode, pos);
            }
        }
        ptrs[k] = words[k];
    }
    put_strings(ptrs, n);
}
