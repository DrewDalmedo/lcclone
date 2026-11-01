/* One starting square. Small cases are 1 to 40, so both parities, primes
 * and odd numbers with several divisors come up often. Full-size ones take
 * a shape the seed picks: one of the ten largest squares (991 to 1000),
 * any square from 1 to 1000, or an odd square from 901 to 999: a lost
 * game, where a search of the game tree has to try every move. */
void gen(int small)
{
    long long n;

    if (small) {
        n = rnd(1, 40);
    } else {
        switch ((int)(gen_seed() % 3)) {
        case 0:
            n = 1000 - rnd(0, 9);
            break;
        case 1:
            n = rnd(1, 1000);
            break;
        default:
            n = 2 * rnd(450, 499) + 1;
        }
    }
    put_int(n);
}
