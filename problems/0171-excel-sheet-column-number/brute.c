/* Slow and obviously right, straight from the order of the names: count
 * the names shorter than columnTitle (26 of one letter, 26^2 of two, ...),
 * then add its place among the names of its own length in alphabetical
 * order, which is the name read as an ordinary base-26 number with A as 0,
 * plus 1. */
int titleToNumber(char *columnTitle)
{
    int len = (int)strlen(columnTitle), i;
    long long shorter = 0, count = 1, place = 0;

    for (i = 1; i < len; i++) {
        count *= 26;
        shorter += count;
    }
    for (i = 0; i < len; i++)
        place = place * 26 + (columnTitle[i] - 'A');
    return (int)(shorter + place + 1);
}
