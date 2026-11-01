/* Reference solution, greedy: the leftmost place that can change matters
 * most. The largest result turns every copy of the leftmost digit that
 * isn't 9 into a 9 (when all digits are 9s, num is already the largest);
 * the smallest turns every copy of the leading digit into a 0. Both are
 * built place by place from the digits of num. */
int minMaxDifference(int num)
{
    int digits[10], len = 0, up = 9, down, hi = 0, lo = 0, i, t;

    for (t = num; t > 0; t /= 10)
        digits[len++] = t % 10; /* digits[len - 1] is the leading digit */
    for (i = len - 1; i >= 0; i--)
        if (digits[i] != 9) {
            up = digits[i];
            break;
        }
    down = digits[len - 1];
    for (i = len - 1; i >= 0; i--) {
        hi = hi * 10 + (digits[i] == up ? 9 : digits[i]);
        lo = lo * 10 + (digits[i] == down ? 0 : digits[i]);
    }
    return hi - lo;
}
