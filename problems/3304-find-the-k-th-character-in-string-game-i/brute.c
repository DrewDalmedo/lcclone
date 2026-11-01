/* Slow and obviously right: build the word round by round until it has at
 * least k letters, and read off position k. */
char kthCharacter(int k)
{
    char word[1024];
    int len = 1, i;

    word[0] = 'a';
    while (len < k) {
        for (i = 0; i < len; i++)
            word[len + i] = word[i] == 'z' ? 'a' : (char)(word[i] + 1);
        len *= 2;
    }
    return word[k - 1];
}
