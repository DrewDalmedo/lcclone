/* Reference solution: decode each word from left to right, multiplying
 * by 10 and adding the letter's digit, then compare the sum. A word holds
 * at most 8 digits, so the sum stays below 2 * 10^8 and fits in an int. */
static int word_value(const char *w)
{
    int v = 0;

    for (; *w != '\0'; w++)
        v = v * 10 + (*w - 'a');
    return v;
}

bool isSumEqual(char *firstWord, char *secondWord, char *targetWord)
{
    return word_value(firstWord) + word_value(secondWord) ==
           word_value(targetWord);
}
