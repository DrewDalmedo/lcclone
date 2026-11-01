/* Reference solution: count the letters of both strings. Each letter of
 * target allows have / need spellings, where have is its count in s and
 * need its count in target; the answer is the smallest of these. */
int rearrangeCharacters(char *s, char *target)
{
    int have[26] = {0}, need[26] = {0}, best = INT_MAX, i;

    for (i = 0; s[i] != '\0'; i++)
        have[s[i] - 'a']++;
    for (i = 0; target[i] != '\0'; i++)
        need[target[i] - 'a']++;
    for (i = 0; i < 26; i++)
        if (need[i] > 0 && have[i] / need[i] < best)
            best = have[i] / need[i];
    return best;
}
