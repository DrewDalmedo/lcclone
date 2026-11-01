/* Reference solution: after each print the marker is on the letter just
 * printed, so every move to the next letter is independent and takes the
 * shorter way round the dial: d or 26 - d notches, where d is the
 * distance in the alphabet. Each letter adds one second for the print. */
int minTimeToType(char *word)
{
    int total = 0, at = 0, i, d;

    for (i = 0; word[i] != '\0'; i++) {
        d = word[i] - 'a' - at;
        if (d < 0)
            d = -d;
        total += (d < 26 - d ? d : 26 - d) + 1;
        at = word[i] - 'a';
    }
    return total;
}
