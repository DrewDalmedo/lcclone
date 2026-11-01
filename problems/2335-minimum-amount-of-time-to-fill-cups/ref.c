/* Reference solution: a second fills at most two cups, and at most one from
 * each nozzle, so at least max(largest count, ceil(total / 2)) seconds are
 * needed; running the two nozzles with the most cups left, second after
 * second, reaches that bound. */
int fillCups(int *amount, int amountSize)
{
    int total = 0, most = 0, half, i;

    for (i = 0; i < amountSize; i++) {
        total += amount[i];
        if (amount[i] > most)
            most = amount[i];
    }
    half = (total + 1) / 2;
    return most > half ? most : half;
}
