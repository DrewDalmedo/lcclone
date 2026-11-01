/* Slow and obviously right: for each word of words1, count how often it
 * occurs in each list by comparing it with every word. A word counted once
 * in words1 is met only once, so no word is counted twice. */
int countWords(char **words1, int words1Size, char **words2, int words2Size)
{
    int paired = 0, i, k, in1, in2;

    for (i = 0; i < words1Size; i++) {
        in1 = in2 = 0;
        for (k = 0; k < words1Size; k++)
            if (strcmp(words1[i], words1[k]) == 0)
                in1++;
        for (k = 0; k < words2Size; k++)
            if (strcmp(words1[i], words2[k]) == 0)
                in2++;
        if (in1 == 1 && in2 == 1)
            paired++;
    }
    return paired;
}
