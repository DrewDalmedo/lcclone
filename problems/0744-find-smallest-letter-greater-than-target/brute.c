/* Slow and obviously right: look at every letter and keep the smallest
 * one after target; when there is none, the smallest of all. */
char nextGreatestLetter(char *letters, int lettersSize, char target)
{
    char best = 0, least = letters[0];
    int i;

    for (i = 0; i < lettersSize; i++) {
        if (letters[i] > target && (best == 0 || letters[i] < best))
            best = letters[i];
        if (letters[i] < least)
            least = letters[i];
    }
    return best ? best : least;
}
