/* Slow and obviously right: cut a copy of the sentence into words, and for
 * each word, from the first, build each of its beginnings (its first 1, 2,
 * ... letters) and compare it whole with searchWord. */
int isPrefixOfWord(char *sentence, char *searchWord)
{
    char *copy = malloc(strlen(sentence) + 1), *w, *start;
    int number = 0, answer = -1;
    size_t len, k;

    strcpy(copy, sentence);
    for (w = strtok(copy, " "); w && answer < 0; w = strtok(NULL, " ")) {
        number++;
        len = strlen(w);
        for (k = 1; k <= len && answer < 0; k++) {
            start = malloc(k + 1);
            memcpy(start, w, k);
            start[k] = '\0';
            if (strcmp(start, searchWord) == 0)
                answer = number;
            free(start);
        }
    }
    free(copy);
    return answer;
}
