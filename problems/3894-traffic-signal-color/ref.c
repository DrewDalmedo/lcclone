/* Reference solution: test the two single readings, then the Red range
 * 31..90, and copy the matching word into malloc'ed memory. */
char *trafficSignal(int timer)
{
    const char *word;
    char *out;

    if (timer == 0)
        word = "Green";
    else if (timer == 30)
        word = "Orange";
    else if (timer > 30 && timer <= 90)
        word = "Red";
    else
        word = "Invalid";
    out = malloc(strlen(word) + 1);
    strcpy(out, word);
    return out;
}
