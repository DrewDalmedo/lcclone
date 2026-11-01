/* A string holding m different letters twice each, in a random order, and
 * a distance array. Half the time every letter of s gets its true count;
 * otherwise one letter, often the first or last of s (the places careless
 * solutions miss), is off by one either way or gets some other value up
 * to 50. Letters not in s get 0, 1 or any value up to 50. Small cases use
 * 1 to 5 letters; full-size ones 20 to 26, half the time all 26 (52
 * characters), sometimes with one letter at both ends (count 50). */
void gen(int small)
{
    int order[26], pos[52], first[26], gap[26], distance[26];
    char s[53];
    int m, len, i, k, c, tmp, pick, victim, how, d;

    for (i = 0; i < 26; i++)
        order[i] = i;
    rnd_shuffle(order, 26);
    if (small)
        m = (int)rnd(1, 5);
    else if (rnd_chance(50))
        m = 26;
    else
        m = (int)rnd(20, 25);
    len = 2 * m;
    for (i = 0; i < len; i++)
        pos[i] = order[i / 2];
    rnd_shuffle(pos, len);
    if (!small && rnd_chance(30)) {
        /* Move the other copy of the first letter to the end. */
        for (k = 1; pos[k] != pos[0]; k++)
            ;
        tmp = pos[k];
        pos[k] = pos[len - 1];
        pos[len - 1] = tmp;
    }
    for (c = 0; c < 26; c++)
        first[c] = gap[c] = -1;
    for (i = 0; i < len; i++) {
        c = pos[i];
        s[i] = (char)('a' + c);
        if (first[c] < 0)
            first[c] = i;
        else
            gap[c] = i - first[c] - 1;
    }
    s[len] = '\0';
    for (c = 0; c < 26; c++) {
        how = (int)rnd(0, 2);
        if (gap[c] >= 0)
            distance[c] = gap[c];
        else
            distance[c] = how == 0 ? 0 : how == 1 ? 1 : (int)rnd(0, 50);
    }
    if (rnd_chance(50)) {
        pick = (int)rnd(0, 3);
        if (pick == 0)
            victim = pos[0];
        else if (pick == 1)
            victim = pos[len - 1];
        else
            victim = pos[rnd(0, len - 1)];
        how = (int)rnd(0, 2);
        if (how == 0 && gap[victim] < 50) {
            distance[victim] = gap[victim] + 1;
        } else if (how == 1 && gap[victim] > 0) {
            distance[victim] = gap[victim] - 1;
        } else {
            do
                d = (int)rnd(0, 50);
            while (d == gap[victim]);
            distance[victim] = d;
        }
    }
    put_string(s);
    put_ints(distance, 26);
}
