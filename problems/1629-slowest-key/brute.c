/* Slow and obviously right: work out every press's time, find the longest
 * one, then try the letters from 'z' down to 'a' and return the first that
 * has a press of that length. */
char slowestKey(int *releaseTimes, int releaseTimesSize, char *keysPressed)
{
    int longest = 0, i, len;
    char c;

    for (i = 0; i < releaseTimesSize; i++) {
        len = i == 0 ? releaseTimes[0] : releaseTimes[i] - releaseTimes[i - 1];
        if (len > longest)
            longest = len;
    }
    for (c = 'z'; c >= 'a'; c--)
        for (i = 0; i < releaseTimesSize; i++) {
            len = i == 0 ? releaseTimes[0]
                         : releaseTimes[i] - releaseTimes[i - 1];
            if (keysPressed[i] == c && len == longest)
                return c;
        }
    return '?';
}
