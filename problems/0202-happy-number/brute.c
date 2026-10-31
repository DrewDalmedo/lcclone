/* Slow and obviously right: just take the steps. One step turns any n in
 * range (at most ten digits) into at most 10 * 81 = 810, and from then on
 * every number has at most three digits, so it stays at most
 * 3 * 81 = 243. A sequence held within 243 values is in its final loop
 * after 243 more steps, so after 1000 steps it is at 1 exactly when n is
 * happy. */
bool isHappy(int n)
{
    int k, sum;

    for (k = 0; k < 1000; k++) {
        sum = 0;
        while (n > 0) {
            sum += (n % 10) * (n % 10);
            n /= 10;
        }
        n = sum;
    }
    return n == 1;
}
