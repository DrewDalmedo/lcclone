/* Reference solution: walk along s keeping a table of the letters seen,
 * so the number of different letters so far is updated in O(1) per step,
 * and compare it with the length so far modulo 3. */
int residuePrefixes(char *s)
{
    int seen[26] = {0}, distinct = 0, count = 0, len;

    for (len = 1; s[len - 1] != '\0'; len++) {
        if (!seen[s[len - 1] - 'a']) {
            seen[s[len - 1] - 'a'] = 1;
            distinct++;
        }
        if (distinct == len % 3)
            count++;
    }
    return count;
}
