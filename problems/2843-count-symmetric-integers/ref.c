/* Reference solution: only two- and four-digit numbers can be symmetric.
 * Walk the range and compare the digit sums of the two halves, taking the
 * digits apart with / and %. */
int countSymmetricIntegers(int low, int high)
{
    int count = 0, x;

    for (x = low; x <= high; x++) {
        if (x >= 10 && x <= 99)
            count += x / 10 == x % 10;
        else if (x >= 1000 && x <= 9999)
            count += x / 1000 + x / 100 % 10 == x / 10 % 10 + x % 10;
    }
    return count;
}
