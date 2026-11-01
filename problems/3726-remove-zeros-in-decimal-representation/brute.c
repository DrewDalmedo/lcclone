/* Slow and obviously right: print n as text, copy every character but '0',
 * and read the copy back as a number. */
long long removeZeros(long long n)
{
    char text[32], kept[32];
    int i, k = 0;

    snprintf(text, sizeof text, "%lld", n);
    for (i = 0; text[i] != '\0'; i++)
        if (text[i] != '0')
            kept[k++] = text[i];
    kept[k] = '\0';
    return strtoll(kept, NULL, 10);
}
