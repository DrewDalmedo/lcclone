/* Slow and obviously right: play the moves out. While the strings differ,
 * a longest one must lose its last character in any solution (the final
 * common string can't be longer than the shortest, nor as long as three
 * equal-length strings that differ), so chop it. If that string is down
 * to one character, all three are single, different characters: give up. */
int findMinimumOperations(char *s1, char *s2, char *s3)
{
    char *s[3] = {s1, s2, s3}, buf[3][101];
    int len[3], moves = 0, i, top;

    for (i = 0; i < 3; i++) {
        strcpy(buf[i], s[i]);
        len[i] = (int)strlen(buf[i]);
    }
    while (strcmp(buf[0], buf[1]) != 0 || strcmp(buf[1], buf[2]) != 0) {
        top = 0;
        for (i = 1; i < 3; i++)
            if (len[i] > len[top])
                top = i;
        if (len[top] == 1)
            return -1;
        buf[top][--len[top]] = '\0';
        moves++;
    }
    return moves;
}
