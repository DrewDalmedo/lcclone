/* One staircase. Small cases have up to 20 stairs. Full-size ones have
 * 45, 44 or 43, as the seed picks: near the limit of 45, where the answer
 * comes close to the int limit, and where counting the ways one at a
 * time, as plain recursion does, takes hundreds of millions of calls. */
void gen(int small)
{
    put_int(small ? rnd(1, 20) : 45 - (long long)(gen_seed() % 3));
}
