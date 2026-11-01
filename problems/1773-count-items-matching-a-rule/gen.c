/* Rows of three words and a query. Small cases draw every word from a
 * few short ones that are prefixes of each other, plus the three column
 * titles, and often put ruleValue in the queried column or in another
 * one. Full-size ones have 10^4 rows; the seed picks a ten-letter
 * ruleValue that every row passes, a ten-letter one that rows match or
 * miss only by its last letter, or a short word of a and b among random
 * words of a and b, many of them its prefixes or extensions. */
static const char *tiny[] = {"a", "ab", "abc", "b", "ba", "type", "color",
                             "name"};
static const char *titles[] = {"type", "color", "name"};

static void random_word(char *w, int len, const char *letters)
{
    int i;

    for (i = 0; i < len; i++)
        w[i] = rnd_char(letters);
    w[len] = '\0';
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000;
    int key = (int)rnd(0, 2);
    int shape = small ? 0 : (int)rnd(0, 2);
    int i, c, len;
    char value[11], word[11];

    if (small)
        strcpy(value, tiny[rnd(0, 7)]);
    else if (shape == 2)
        random_word(value, (int)rnd(1, 4), "ab");
    else
        random_word(value, 10, "abcdefghijklmnopqrstuvwxyz");
    putchar('[');
    for (i = 0; i < n; i++) {
        printf("%s[", i ? "," : "");
        for (c = 0; c < 3; c++) {
            if (small) {
                strcpy(word, tiny[rnd(0, 7)]);
                if (rnd_chance(c == key ? 40 : 20))
                    strcpy(word, value);
            } else if (c != key) {
                if (rnd_chance(10))
                    strcpy(word, value);
                else
                    random_word(word, (int)rnd(1, 10), "abcdefghij");
            } else if (shape == 0) {
                strcpy(word, value);
            } else if (shape == 1) {
                strcpy(word, value);
                if (rnd_chance(50))
                    word[9] = word[9] == 'z' ? 'a' : (char)(word[9] + 1);
            } else {
                len = (int)rnd(1, 5);
                random_word(word, len, "ab");
            }
            printf("%s\"%s\"", c ? "," : "", word);
        }
        putchar(']');
    }
    puts("]");
    put_string(titles[key]);
    put_string(value);
}
