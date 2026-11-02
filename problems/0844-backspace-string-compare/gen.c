/* Two keystroke logs. Small cases have at most 8 keys, with letters from
 * "ab" or "abc" so that equal texts come up often, in a shape the seed
 * picks: two unrelated logs, two logs that type the same text with
 * different typos and backspaces, or a near miss whose texts differ in
 * one letter. Full-size logs have up to 200 keys: 200 letters and no
 * backspace (equal, or differing in the first or the last letter), logs
 * filled to 200 keys with corrections, a near miss at the first letter,
 * or a log with a run of 128 or more backspaces that empties the box. */

static const char *letters;

/* Appends to log[0..*len) keys that type text[0..m) into an empty box,
 * with typos and backspaces mixed in, using at most cap keys in all
 * (m <= cap - *len). With fill set, it adds corrections until the log is
 * full, give or take a key; burst is the longest typo it makes. */
static void type_text(char *log, int *len, int cap, const char *text,
                      int m, int fill, int burst)
{
    int n = *len, p = 0, room, act, k, kmax, i;

    for (;;) {
        /* The box holds text[0..p); room is what is left to spare. */
        room = cap - n - (m - p);
        if (p == m && (room < 2 || (!fill && rnd_chance(40))))
            break;
        act = (int)rnd(0, 9);
        if (p < m && (act < 6 || room < 2)) {
            log[n++] = text[p++];
        } else if (act < 8 && room >= 2) {
            /* A typo of k letters, erased right away. */
            kmax = room / 2 < burst ? room / 2 : burst;
            k = (int)rnd(1, kmax);
            for (i = 0; i < k; i++)
                log[n++] = rnd_char(letters);
            for (i = 0; i < k; i++)
                log[n++] = '#';
        } else {
            /* k backspaces, maybe more than the box holds; the letters
             * they erase must be typed again. */
            kmax = room >= 2 * p ? room - p : room / 2;
            if (kmax > p + burst)
                kmax = p + burst;
            if (kmax < 1)
                continue;
            k = (int)rnd(1, kmax);
            for (i = 0; i < k; i++)
                log[n++] = '#';
            p = k < p ? p - k : 0;
        }
    }
    *len = n;
}

/* Changes text[0..*m) in one place, at the start when first is set:
 * one letter replaced by another, one letter left out or one added. */
static void mutate(char *text, int *m, int first)
{
    int op = (int)rnd(0, 2), pos, i;
    char c;

    if (*m == 0)
        op = 2;
    if (op == 2) {
        pos = first ? 0 : (int)rnd(0, *m);
        for (i = *m; i > pos; i--)
            text[i] = text[i - 1];
        text[pos] = rnd_char(letters);
        (*m)++;
        return;
    }
    pos = first ? 0 : (int)rnd(0, *m - 1);
    if (op == 1) {
        for (i = pos; i + 1 < *m; i++)
            text[i] = text[i + 1];
        (*m)--;
        return;
    }
    do
        c = rnd_char(letters);
    while (c == text[pos]);
    text[pos] = c;
}

static void put_log(char *log, int n)
{
    if (n == 0)
        log[n++] = '#';
    log[n] = '\0';
    put_string(log);
}

void gen(int small)
{
    char s[202], t[202], text[202], other[202];
    int cap = small ? 8 : 200, burst = small ? 2 : 8;
    int shape = small ? (int)rnd(0, 9) : (int)rnd(0, 3);
    int ns = 0, nt = 0, m, mo, k, run, pct, i;

    letters = small ? (rnd_chance(50) ? "ab" : "abc")
                    : "abcdefghijklmnopqrstuvwxyz";
    if (small && shape <= 2) {
        /* Two unrelated logs. */
        pct = (int)rnd(20, 50);
        ns = (int)rnd(1, cap);
        for (i = 0; i < ns; i++)
            s[i] = rnd_chance(pct) ? '#' : rnd_char(letters);
        nt = (int)rnd(1, cap);
        for (i = 0; i < nt; i++)
            t[i] = rnd_chance(pct) ? '#' : rnd_char(letters);
    } else if (!small && shape == 0) {
        /* 200 letters each, no backspace. */
        ns = nt = cap;
        for (i = 0; i < ns; i++)
            s[i] = t[i] = rnd_char(letters);
        k = (int)rnd(0, 2);
        if (k > 0) {
            i = k == 1 ? 0 : nt - 1;
            do
                t[i] = rnd_char(letters);
            while (t[i] == s[i]);
        }
    } else if (!small && shape == 3) {
        /* A few letters, then a run of 128 or more backspaces that
         * erases them and then finds the box empty, then the text. */
        k = (int)rnd(1, 30);
        run = (int)rnd(128, 165);
        for (i = 0; i < k; i++)
            s[ns++] = rnd_char(letters);
        for (i = 0; i < run; i++)
            s[ns++] = '#';
        m = (int)rnd(0, cap - ns);
        for (i = 0; i < m; i++)
            text[i] = rnd_char(letters);
        type_text(s, &ns, cap, text, m, 1, burst);
        memcpy(other, text, (size_t)m);
        mo = m;
        if (rnd_chance(30))
            mutate(other, &mo, 0);
        type_text(t, &nt, cap, other, mo, 1, burst);
    } else {
        /* The same text typed twice, or a near miss (small shapes 3 to 6
         * and full-size shape 1 type the same text). */
        int same = small ? shape <= 6 : shape == 1;

        m = small ? (int)rnd(0, 5) : (same ? (int)rnd(0, 150) : (int)rnd(40, 150));
        for (i = 0; i < m; i++)
            text[i] = rnd_char(letters);
        memcpy(other, text, (size_t)m);
        mo = m;
        if (!same)
            mutate(other, &mo, !small);
        type_text(s, &ns, cap, text, m, !small, burst);
        type_text(t, &nt, cap, other, mo, !small, burst);
    }
    put_log(s, ns);
    put_log(t, nt);
}
