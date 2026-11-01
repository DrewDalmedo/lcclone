/* Reference solution: a sliding window with a count per letter. When the
 * letter just added reaches three copies, the left end moves past the
 * first copy of that letter in the window, however far that is. */
int maximumLengthSubstring(char *s)
{
    int count[26] = {0};
    int left = 0, best = 0, right, c;

    for (right = 0; s[right] != '\0'; right++) {
        c = s[right] - 'a';
        count[c]++;
        while (count[c] > 2)
            count[s[left++] - 'a']--;
        if (right - left + 1 > best)
            best = right - left + 1;
    }
    return best;
}
