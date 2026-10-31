/* Reference solution: Floyd's cycle detection, in O(1) memory. A slow
 * walker takes one step at a time and a fast one two. The sequence ends in
 * a loop, where the fast walker catches up with the slow one, unless it
 * reaches 1 first; and that loop is just 1 exactly when n is happy. */
static int digit_square_sum(int n)
{
    int sum = 0;

    while (n > 0) {
        int d = n % 10;

        sum += d * d;
        n /= 10;
    }
    return sum;
}

bool isHappy(int n)
{
    int slow = n, fast = digit_square_sum(n);

    while (fast != 1 && fast != slow) {
        slow = digit_square_sum(slow);
        fast = digit_square_sum(digit_square_sum(fast));
    }
    return fast == 1;
}
