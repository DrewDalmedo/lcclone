/* Reference solution: walk s once, keeping the lengths of the current run
 * of equal digits and of the run before it. Every half-and-half substring
 * is centred on the border between two neighbouring runs, and a border
 * between runs of lengths p and q is the centre of min(p, q) of them. The
 * last border is counted after the loop. */
int countBinarySubstrings(char *s)
{
    int n = (int)strlen(s), prev = 0, cur = 1, count = 0, i;

    for (i = 1; i < n; i++) {
        if (s[i] == s[i - 1]) {
            cur++;
        } else {
            count += prev < cur ? prev : cur;
            prev = cur;
            cur = 1;
        }
    }
    return count + (prev < cur ? prev : cur);
}
