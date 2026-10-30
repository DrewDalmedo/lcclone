/* Reference solution: binary search for the first letter after target. */
char nextGreatestLetter(char *letters, int lettersSize, char target)
{
    int lo = 0, hi = lettersSize;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (letters[mid] <= target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo < lettersSize ? letters[lo] : letters[0];
}
