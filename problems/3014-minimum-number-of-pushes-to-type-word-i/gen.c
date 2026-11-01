/* A word of different letters: the alphabet in random order, cut to m
 * letters. The answer depends only on m. Small cases take m from all of
 * 1..26, half the time from the lengths where a round of keys fills up
 * (1, 8, 9, 16, 17, 24, 25, 26); brute.c is quick for any m. Full-size
 * cases take 24 to 26 letters. */
void gen(int small)
{
    static const int edges[] = {1, 8, 9, 16, 17, 24, 25, 26};
    int letters[26], m, i;
    char word[27];

    if (!small)
        m = 26 - (int)(gen_seed() % 3);
    else if (rnd_chance(50))
        m = edges[rnd(0, 7)];
    else
        m = (int)rnd(1, 26);
    for (i = 0; i < 26; i++)
        letters[i] = i;
    rnd_shuffle(letters, 26);
    for (i = 0; i < m; i++)
        word[i] = (char)('a' + letters[i]);
    word[m] = '\0';
    put_string(word);
}
