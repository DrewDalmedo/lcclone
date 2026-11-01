/* Slow and obviously right: cut a copy of s into pieces, read every piece
 * that starts with a digit as a number, then check every pair of numbers:
 * each must be smaller than every number after it. */
bool areNumbersAscending(char *s)
{
    char *copy = malloc(strlen(s) + 1), *tok;
    int nums[100], m = 0, i, j;
    bool ok = true;

    strcpy(copy, s);
    for (tok = strtok(copy, " "); tok; tok = strtok(NULL, " "))
        if (isdigit((unsigned char)tok[0]))
            nums[m++] = atoi(tok);
    for (i = 0; i < m; i++)
        for (j = i + 1; j < m; j++)
            if (nums[i] >= nums[j])
                ok = false;
    free(copy);
    return ok;
}
