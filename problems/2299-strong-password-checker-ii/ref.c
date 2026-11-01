/* Reference solution: one pass that sets a bit for the kind of each
 * character (anything that is not a letter or a digit is one of the
 * symbols) and fails at the first character equal to the one before it. */
bool strongPasswordCheckerII(char *password)
{
    int kinds = 0, i;

    for (i = 0; password[i]; i++) {
        char c = password[i];

        if (i > 0 && c == password[i - 1])
            return false;
        if (c >= 'a' && c <= 'z')
            kinds |= 1;
        else if (c >= 'A' && c <= 'Z')
            kinds |= 2;
        else if (c >= '0' && c <= '9')
            kinds |= 4;
        else
            kinds |= 8;
    }
    return i >= 8 && kinds == 15;
}
