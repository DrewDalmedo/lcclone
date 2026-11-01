/* Reference solution: mark the broken letters in a 26-entry table, then
 * walk text once, remembering whether the current word has met a broken
 * letter. A word is settled at the space after it, or at the end of text. */
int canBeTypedWords(char *text, char *brokenLetters)
{
    bool broken[26] = {false};
    bool spoiled = false;
    int whole = 0, i;

    for (i = 0; brokenLetters[i] != '\0'; i++)
        broken[brokenLetters[i] - 'a'] = true;
    for (i = 0;; i++) {
        if (text[i] == ' ' || text[i] == '\0') {
            if (!spoiled)
                whole++;
            spoiled = false;
            if (text[i] == '\0')
                break;
        } else if (broken[text[i] - 'a']) {
            spoiled = true;
        }
    }
    return whole;
}
