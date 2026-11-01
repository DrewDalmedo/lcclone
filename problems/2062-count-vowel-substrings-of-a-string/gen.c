/* A word of mostly vowels. The seed picks which vowels may appear (all
 * five, or four with one left out) and how often a consonant (y, b or z)
 * splits the vowels into runs, from never to about one letter in three.
 * Small cases are up to 20 letters long; full-size ones are 100, and
 * often vowels only, where the answer is largest and a check of every
 * highlight takes longest. */
void gen(int small)
{
    static const char *sets[] = {"aeiou", "eiou", "aiou", "aeou", "aeiu", "aeio"};
    static const int percent[] = {0, 0, 4, 12, 30};
    char word[101];
    int n = small ? (int)rnd(1, 20) : 100, i;
    int set = rnd_chance(75) ? 0 : (int)rnd(1, 5);
    int split = percent[rnd(0, 4)];

    for (i = 0; i < n; i++)
        word[i] = rnd_chance(split) ? rnd_char("ybz") : rnd_char(sets[set]);
    word[n] = '\0';
    put_string(word);
}
