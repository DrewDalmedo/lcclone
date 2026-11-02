/* Slow and obviously right: print n as text and look at its characters. */
bool validDigit(int n, int x)
{
    char text[16];
    int i;

    snprintf(text, sizeof text, "%d", n);
    if (text[0] == '0' + x)
        return false;
    for (i = 1; text[i] != '\0'; i++)
        if (text[i] == '0' + x)
            return true;
    return false;
}
