/* Slow and obviously right: work out each post's altitude anew, adding up
 * the changes before it from post 0, and take the largest. */
int largestAltitude(int *gain, int gainSize)
{
    int best = 0, post, i, alt;

    for (post = 0; post <= gainSize; post++) {
        alt = 0;
        for (i = 0; i < post; i++)
            alt += gain[i];
        if (post == 0 || alt > best)
            best = alt;
    }
    return best;
}
