/* Reference solution: walk through the sentence once. At the start of each
 * word, compare it with searchWord letter by letter (a space or the end of
 * the sentence never equals a letter, so a shorter word fails), then skip
 * to the next word. */
int isPrefixOfWord(char *sentence, char *searchWord)
{
    const char *p = sentence;
    int word = 1, i;

    for (;;) {
        for (i = 0; searchWord[i] && p[i] == searchWord[i]; i++)
            ;
        if (!searchWord[i])
            return word;
        while (*p && *p != ' ')
            p++;
        if (!*p)
            return -1;
        p++;
        word++;
    }
}
