/* Reference solution: erasing a copy of digit that is followed by a larger
 * digit makes the number grow at that place, and the earlier the place,
 * the more it counts, so erase the first such copy. If there is none,
 * every erasure makes the number shrink at the place it happens (or not
 * at all), and the later that place, the better: erase the last copy. */
char *removeDigit(char *number, char digit)
{
    size_t n = strlen(number), pick = 0, i;
    char *out = malloc(n);

    for (i = 0; i < n; i++)
        if (number[i] == digit) {
            pick = i;
            if (i + 1 < n && number[i + 1] > digit)
                break;
        }
    memcpy(out, number, pick);
    memcpy(out + pick, number + pick + 1, n - pick - 1);
    out[n - 1] = '\0';
    return out;
}
