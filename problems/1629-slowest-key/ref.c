/* Reference solution: one pass over the presses, keeping the longest time
 * so far and its key. A longer press replaces both; an equally long one
 * replaces the key only when its letter comes later in the alphabet. */
char slowestKey(int *releaseTimes, int releaseTimesSize, char *keysPressed)
{
    int best = releaseTimes[0], len, i;
    char key = keysPressed[0];

    for (i = 1; i < releaseTimesSize; i++) {
        len = releaseTimes[i] - releaseTimes[i - 1];
        if (len > best || (len == best && keysPressed[i] > key)) {
            best = len;
            key = keysPressed[i];
        }
    }
    return key;
}
