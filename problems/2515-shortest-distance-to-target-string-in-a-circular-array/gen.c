/* Place cards round a table, a target and a seat to start from. Small
 * cases have 1 to 8 cards drawn from a, b, aa, ab, ba and abc (prefixes
 * and anagrams of each other), and target is usually one of the cards
 * away from the starting seat.
 * Full-size ones have 100 cards, each a near miss of target: target with
 * its last letter changed or cut off, or with one letter added at the
 * end. The seed then puts target on no seat, on one seat 49 or 50 moves
 * from the start, or on a few random seats. */
static char cards[100][102];

static void near_miss(char *out, const char *target)
{
    int len = (int)strlen(target), kind = (int)rnd(0, 2);

    strcpy(out, target);
    if (kind == 1 && len > 1) {
        out[len - 1] = '\0';
    } else if (kind == 2 && len < 100) {
        out[len] = rnd_char("abc");
        out[len + 1] = '\0';
    } else {
        out[len - 1] = out[len - 1] == 'a' ? rnd_char("bc") : 'a';
    }
}

void gen(int small)
{
    static const char *tiny[] = {"a", "b", "aa", "ab", "ba", "abc"};
    char target[101];
    char *words[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int start = (int)rnd(0, n - 1), len, shape, dist, side, copies, i, k;

    if (small) {
        for (i = 0; i < n; i++) {
            k = (int)rnd(0, 5);
            strcpy(cards[i], tiny[k]);
        }
        if (rnd_chance(75)) {
            /* a card other than the starting one, when there is one */
            k = n > 1 ? (start + (int)rnd(1, n - 1)) % n : 0;
            strcpy(target, cards[k]);
        } else {
            k = (int)rnd(0, 5);
            strcpy(target, tiny[k]);
        }
    } else {
        len = rnd_chance(50) ? 100 : (int)rnd(1, 100);
        for (i = 0; i < len; i++)
            target[i] = rnd_char("abc");
        target[len] = '\0';
        for (i = 0; i < n; i++)
            near_miss(cards[i], target);
        shape = (int)rnd(0, 2);
        if (shape == 1) {
            dist = (int)rnd(49, 50);
            side = rnd_chance(50) ? 1 : -1;
            k = ((start + side * dist) % n + n) % n;
            strcpy(cards[k], target);
        } else if (shape == 2) {
            copies = (int)rnd(2, 5);
            for (i = 0; i < copies; i++) {
                k = (int)rnd(0, n - 1);
                strcpy(cards[k], target);
            }
        }
    }
    for (i = 0; i < n; i++)
        words[i] = cards[i];
    put_strings(words, n);
    put_string(target);
    put_int(start);
}
