/* Reference solution: two closed ranges share a moment exactly when each
 * starts no later than the other ends. Zero-padded "HH:MM" strings sort in
 * the same order as the times they name, so strcmp compares them. */
bool haveConflict(char **event1, int event1Size, char **event2, int event2Size)
{
    (void)event1Size;
    (void)event2Size;
    return strcmp(event1[0], event2[1]) <= 0 && strcmp(event2[0], event1[1]) <= 0;
}
