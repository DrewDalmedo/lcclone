/* Slow and obviously right: for each letter from a to z, search the whole
 * sentence for it, and fail as soon as one is not found. */
bool checkIfPangram(char *sentence)
{
    const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    size_t n = strlen(sentence), i;
    int c, found;

    for (c = 0; c < 26; c++) {
        found = 0;
        for (i = 0; i < n; i++)
            if (sentence[i] == alphabet[c])
                found = 1;
        if (!found)
            return false;
    }
    return true;
}
