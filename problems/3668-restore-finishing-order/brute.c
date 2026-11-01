/* Slow and obviously right: look up each friend's place by scanning the
 * results sheet, then sort the friends by place with a selection sort. */
int *recoverOrder(int *order, int orderSize, int *friends, int friendsSize,
                  int *returnSize)
{
    int *out = malloc((size_t)friendsSize * sizeof *out);
    int *place = malloc((size_t)friendsSize * sizeof *place);
    int i, k, best, tmp;

    for (i = 0; i < friendsSize; i++) {
        out[i] = friends[i];
        for (k = 0; k < orderSize; k++)
            if (order[k] == friends[i])
                place[i] = k;
    }
    for (i = 0; i < friendsSize; i++) {
        best = i;
        for (k = i + 1; k < friendsSize; k++)
            if (place[k] < place[best])
                best = k;
        tmp = out[i];
        out[i] = out[best];
        out[best] = tmp;
        tmp = place[i];
        place[i] = place[best];
        place[best] = tmp;
    }
    free(place);
    *returnSize = friendsSize;
    return out;
}
