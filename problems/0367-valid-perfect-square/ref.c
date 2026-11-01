/* Reference solution: Newton's method on integers. Starting from r = num,
 * which is at least the root, each step r = (r + num / r) / 2 moves r down
 * towards the root without passing below its floor, so the loop stops at
 * the floor of the square root, after O(log num) steps. Then num is a
 * square exactly when that floor squared gives num back. Everything is in
 * 64 bits, where neither r * r nor r + num / r can overflow. */
bool isPerfectSquare(int num)
{
    long long r = num;

    while (r * r > num)
        r = (r + num / r) / 2;
    return r * r == num;
}
