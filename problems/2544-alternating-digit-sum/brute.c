/* Slow and obviously right: print n as text and walk its digit characters
 * from the left, adding the 1st, 3rd, ... and subtracting the others. */
int alternateDigitSum(int n)
{
    char text[16];
    int score = 0, i;

    snprintf(text, sizeof text, "%d", n);
    for (i = 0; text[i] != '\0'; i++) {
        if (i % 2 == 0)
            score += text[i] - '0';
        else
            score -= text[i] - '0';
    }
    return score;
}
