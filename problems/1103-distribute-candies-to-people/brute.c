/* Slow and obviously right: hand out the helpings one by one, going round
 * the table, until the bag is empty. */
int *distributeCandies(int candies, int num_people, int *returnSize)
{
    int *out = calloc((size_t)num_people, sizeof *out);
    int left = candies, g = 1, seat = 0, give;

    while (left > 0) {
        give = g < left ? g : left;
        out[seat] += give;
        left -= give;
        g++;
        seat = (seat + 1) % num_people;
    }
    *returnSize = num_people;
    return out;
}
