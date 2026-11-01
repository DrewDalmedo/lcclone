/* Small cases: 1 to 8 characters, mostly from a set that holds the letters
 * at both ends of each alphabet, the characters right next to them in
 * ASCII (@ [ \ ] ^ _ ` {), a digit, a space and a quote, so that every
 * off-by-one range test and bit trick meets a character it gets wrong.
 *
 * Full-size cases: 100 characters, in a shape the seed picks: any
 * printable ASCII; capitals only; no capitals at all; or capitals only at
 * both ends, with spaces and punctuation between. */
static const char tricky[] = "AZazMq@[\\]^_`{|}~ 0\"";

static char printable(void)
{
    return (char)rnd(32, 126);
}

void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 8) : 100, shape = (int)rnd(0, 3), i;
    char c;

    for (i = 0; i < n; i++) {
        if (small) {
            c = rnd_chance(80) ? rnd_char(tricky) : printable();
        } else if (shape == 0) {
            c = printable();
        } else if (shape == 1) {
            c = (char)rnd('A', 'Z');
        } else if (shape == 2) {
            do
                c = printable();
            while (c >= 'A' && c <= 'Z');
        } else {
            c = i == 0 || i == n - 1 ? (char)rnd('A', 'Z') : rnd_char(" .,!?-@[_{}~");
        }
        s[i] = c;
    }
    s[n] = '\0';
    put_string(s);
}
