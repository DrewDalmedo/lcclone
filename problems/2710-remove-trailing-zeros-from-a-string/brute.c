/* Slow and obviously right: copy num, then, as long as its last character
 * is '0', cut that character off, measuring the string again each time. */
char *removeTrailingZeros(char *num)
{
    char *out = malloc(strlen(num) + 1);

    strcpy(out, num);
    while (strlen(out) > 0 && out[strlen(out) - 1] == '0')
        out[strlen(out) - 1] = '\0';
    return out;
}
