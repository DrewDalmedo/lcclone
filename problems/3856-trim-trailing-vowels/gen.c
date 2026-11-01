/* Small cases have up to 8 letters, about two in three of them vowels; the
 * others are y, z and the consonants next to a vowel in the alphabet, so
 * that vowel tails of every length, all-vowel strings and strings ending
 * in a consonant all come up. Full-size cases have 100 letters: a random
 * head of length 0, 1, 100 or in between whose last letter is a consonant,
 * followed by a tail of vowels. */
void gen(int small)
{
    static const char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char s[101];
    int n = small ? (int)rnd(1, 8) : 100, head, shape, i;

    if (small) {
        for (i = 0; i < n; i++)
            s[i] = rnd_chance(65) ? rnd_char("aeiou") : rnd_char("bdfhjnptvyz");
    } else {
        shape = (int)rnd(0, 3);
        head = shape == 0 ? 0 : shape == 1 ? 1 : shape == 2 ? 100 : (int)rnd(2, 99);
        for (i = 0; i < head - 1; i++)
            s[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        if (head > 0)
            s[head - 1] = rnd_char(consonants);
        for (i = head; i < n; i++)
            s[i] = rnd_char("aeiou");
    }
    s[n] = '\0';
    put_string(s);
}
