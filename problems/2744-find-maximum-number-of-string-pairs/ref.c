/* Reference solution: scan the codes, marking each one in a 26 x 26 table
 * of codes seen so far; a code whose letters, swapped, were seen already
 * completes a match. As the codes are all different, each entry has at
 * most one mirror, so every match found can be kept. A code is marked only
 * after its lookup, so a doubled-letter code never matches itself. */
int maximumNumberOfStringPairs(char **words, int wordsSize)
{
    bool seen[26][26] = {{false}};
    int matches = 0, i, a, b;

    for (i = 0; i < wordsSize; i++) {
        a = words[i][0] - 'a';
        b = words[i][1] - 'a';
        if (seen[b][a])
            matches++;
        seen[a][b] = true;
    }
    return matches;
}
