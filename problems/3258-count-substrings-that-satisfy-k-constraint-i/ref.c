/* Reference solution: a sliding window. If a substring meets the
 * k-constraint, so does every substring inside it, so for each right end
 * the starts that work form a run ending at that right end. Move the left
 * end on while both counts exceed k, then every start from there on
 * gives a substring that meets it. */
int countKConstraintSubstrings(char *s, int k)
{
    int count[2] = {0, 0}, left = 0, right, total = 0;

    for (right = 0; s[right]; right++) {
        count[s[right] - '0']++;
        while (count[0] > k && count[1] > k)
            count[s[left++] - '0']--;
        total += right - left + 1;
    }
    return total;
}
