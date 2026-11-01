/* Reference solution: an echo lengthens one block of equal neighbouring
 * letters, so s is either word itself or word with one block shortened,
 * to any length from 1 up to one less than it had. A block of L letters
 * gives L - 1 such strings, and all of these strings differ, so the count
 * is 1 plus the number of letters equal to the letter before them. */
int possibleStringCount(char *word)
{
    int count = 1, i;

    for (i = 1; word[i]; i++)
        if (word[i] == word[i - 1])
            count++;
    return count;
}
