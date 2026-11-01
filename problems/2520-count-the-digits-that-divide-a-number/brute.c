/* Slow and obviously right: print num as text, and for each character
 * check whether num is a multiple of the digit it shows. */
int countDigits(int num)
{
    char text[16];
    int count = 0, i;

    sprintf(text, "%d", num);
    for (i = 0; text[i] != '\0'; i++)
        if (num % (text[i] - '0') == 0)
            count++;
    return count;
}
