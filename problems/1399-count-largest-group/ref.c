/* Reference solution: count how many numbers have each digit sum (at most
 * 36, from 9999), then find the largest count and how many digit sums
 * reach it. */
int countLargestGroup(int n)
{
    int size[37] = {0};
    int best = 0, ties = 0, x, y, s;

    for (x = 1; x <= n; x++) {
        s = 0;
        for (y = x; y > 0; y /= 10)
            s += y % 10;
        size[s]++;
    }
    for (s = 1; s <= 36; s++) {
        if (size[s] > best) {
            best = size[s];
            ties = 1;
        } else if (size[s] == best) {
            ties++;
        }
    }
    return ties;
}
