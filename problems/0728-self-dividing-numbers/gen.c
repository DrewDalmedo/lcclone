/* One range [left, right]. Small cases are short ranges: among the first
 * couple of hundred numbers (single digits, 10, 20, 100 to 110), anywhere
 * up to 10^4, a single number, or up against 10^4 itself, which has zeros.
 * Full-size ones are the whole range [1, 10^4] (every self-dividing number
 * there is) or most of it. */
void gen(int small)
{
    int shape = (int)rnd(0, 9), left, right;

    if (small) {
        if (shape < 3) {
            left = (int)rnd(1, 200);
            right = left + (int)rnd(0, 20);
        } else if (shape < 6) {
            left = (int)rnd(1, 10000);
            right = left + (int)rnd(0, 40);
        } else if (shape < 8) {
            left = (int)rnd(1, 10000);
            right = left;
        } else {
            left = (int)rnd(9950, 10000);
            right = 10000 - (int)rnd(0, 3);
            if (right < left)
                right = left;
        }
        if (right > 10000)
            right = 10000;
    } else if (shape < 4) {
        left = 1;
        right = 10000;
    } else {
        left = (int)rnd(1, 1000);
        right = (int)rnd(9000, 10000);
    }
    put_int(left);
    put_int(right);
}
