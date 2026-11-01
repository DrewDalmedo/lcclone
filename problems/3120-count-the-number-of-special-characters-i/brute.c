/* Slow and obviously right: for each of the 26 letters, search the whole
 * word for its lowercase form and again for its uppercase form. */
int numberOfSpecialChars(char *word)
{
    int count = 0, letter, i, has_lower, has_upper;

    for (letter = 0; letter < 26; letter++) {
        has_lower = has_upper = 0;
        for (i = 0; word[i]; i++) {
            if (word[i] == 'a' + letter)
                has_lower = 1;
            if (word[i] == 'A' + letter)
                has_upper = 1;
        }
        if (has_lower && has_upper)
            count++;
    }
    return count;
}
