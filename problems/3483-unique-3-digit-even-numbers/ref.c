/* Reference solution: count how often each digit is in the pool, then go
 * through the 450 even numbers from 100 to 998 and keep those whose three
 * digits the counts can cover. */
int totalNumbers(int *digits, int digitsSize)
{
    int have[10] = {0}, need[10], total = 0, x, i, ok;

    for (i = 0; i < digitsSize; i++)
        have[digits[i]]++;
    for (x = 100; x < 1000; x += 2) {
        for (i = 0; i < 10; i++)
            need[i] = 0;
        need[x / 100]++;
        need[x / 10 % 10]++;
        need[x % 10]++;
        ok = 1;
        for (i = 0; i < 10; i++)
            if (need[i] > have[i])
                ok = 0;
        total += ok;
    }
    return total;
}
