/* Reference solution: a difference array over the posts. Each stretch
 * [s, e] adds 1 at s and takes 1 away at e + 1; the running sum is then
 * how many painters painted each post, and every post from left to right
 * needs a positive count. */
bool isCovered(int **ranges, int rangesSize, int *rangesColSize, int left,
               int right)
{
    int diff[52] = {0}, count = 0, x, i;

    (void)rangesColSize;
    for (i = 0; i < rangesSize; i++) {
        diff[ranges[i][0]]++;
        diff[ranges[i][1] + 1]--;
    }
    for (x = 1; x <= right; x++) {
        count += diff[x];
        if (x >= left && count <= 0)
            return false;
    }
    return true;
}
