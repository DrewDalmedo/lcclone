/* Slow and obviously right: print the three numbers as text padded with
 * zeros to four characters, take the smallest character of each column,
 * and read the resulting text back as a number. */
int generateKey(int num1, int num2, int num3)
{
    char a[16], b[16], c[16], key[5];
    int i;

    sprintf(a, "%04d", num1);
    sprintf(b, "%04d", num2);
    sprintf(c, "%04d", num3);
    for (i = 0; i < 4; i++) {
        key[i] = a[i];
        if (b[i] < key[i])
            key[i] = b[i];
        if (c[i] < key[i])
            key[i] = c[i];
    }
    key[4] = '\0';
    return atoi(key);
}
