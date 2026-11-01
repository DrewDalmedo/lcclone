/* A string of letters and digits in which every digit has a letter left to
 * wipe out: a digit is placed only while the letters so far outnumber the
 * digits. Small cases are short, with letters from "abc" so that equal
 * letters repeat. Full-size ones have 100 characters, in a shape the seed
 * picks: random, no digit at all, 50 letters then 50 digits, letters and
 * digits alternating, or runs of letters and digits. */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 10) : 100;
    int shape = small ? 0 : (int)rnd(0, 4);
    int percent = (int)rnd(10, 70);
    int balance = 0, digit, run = 0, i;
    const char *letters = small ? "abc" : "abcdefghijklmnopqrstuvwxyz";

    for (i = 0; i < n; i++) {
        if (shape == 1)
            digit = 0;
        else if (shape == 2)
            digit = i >= n / 2;
        else if (shape == 3)
            digit = i % 2;
        else if (shape == 4) {
            if (run == 0) {
                run = (int)rnd(1, 8);
                digit = balance > 0 && rnd_chance(50);
            } else
                digit = s[i - 1] >= '0' && s[i - 1] <= '9';
            run--;
        } else
            digit = rnd_chance(percent);
        if (digit && balance == 0)
            digit = 0;
        if (digit) {
            s[i] = rnd_char("0123456789");
            balance--;
        } else {
            s[i] = rnd_char(letters);
            balance++;
        }
    }
    s[n] = '\0';
    put_string(s);
}
