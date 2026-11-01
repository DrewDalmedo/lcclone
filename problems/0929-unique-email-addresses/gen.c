/* A case picks a few domains and a few base users (the user parts of the
 * mailboxes), then writes each address as a base user dressed up with
 * dots, and sometimes a '+' and junk after it, at one of the domains, so
 * that different addresses often share a mailbox. A base user may be
 * empty; its address then starts with '.'. Small cases have up to 6
 * addresses over 1 to 3 base users of a and b and 1 to 3 domains from a
 * tiny list (one of them holds a '+'). Full-size domains are up to 16
 * characters long, or one time in five from 34 to 60, so that a user part
 * of 30 characters always still fits. Full-size cases have 100 addresses,
 * in a shape the seed picks: mostly fresh users, a fifth of them as long
 * as an address allows (100 characters); 1 to 5 base users at 1 to 3
 * domains; or 30 base users at up to 20 domains. */
static char doms[20][64];
static char bases[30][32];
static char addr[100][101];

static void make_domain(char *d, int small)
{
    static const char *const tiny[] = {
        "a.com", "b.com", "ab.com", "a.b.com", "b+a.com",
    };
    int len, i;

    if (small) {
        i = (int)rnd(0, 4);
        strcpy(d, tiny[i]);
        return;
    }
    len = rnd_chance(20) ? (int)rnd(30, 56) : (int)rnd(1, 12);
    for (i = 0; i < len; i++) {
        if (i > 0 && i < len - 1 && rnd_chance(15))
            d[i] = '.';
        else
            d[i] = rnd_char("abcxyz");
    }
    if (len >= 3 && rnd_chance(10)) {
        i = (int)rnd(1, len - 2);
        d[i] = '+';
    }
    strcpy(d + len, ".com");
}

/* Writes to out a user part of at most room characters whose mailbox user
 * is base: dots between and around base's letters, then perhaps a '+' and
 * up to max_junk characters. A leading '.' is added when the result would
 * be empty or start with '+'. strlen(base) < room. */
static void dress(char *out, const char *base, int room, int dot_pct,
                  int max_junk)
{
    int b = (int)strlen(base), len = 0, i, k, extra;

    room--;
    for (i = 0; i <= b; i++) {
        while (len < room - (b - i) && rnd_chance(dot_pct))
            out[len++] = '.';
        if (i < b)
            out[len++] = base[i];
    }
    if (len < room && rnd_chance(50)) {
        out[len++] = '+';
        extra = (int)rnd(0, room - len < max_junk ? room - len : max_junk);
        for (k = 0; k < extra; k++)
            out[len++] = rnd_char("abc.+");
    }
    if (len == 0 || out[0] == '+') {
        memmove(out + 1, out, (size_t)len);
        out[0] = '.';
        len++;
    }
    out[len] = '\0';
}

void gen(int small)
{
    char *ptrs[100];
    char user[101];
    int n = small ? (int)rnd(1, 6) : 100, shape = 0, nd, nb, i, k, len;
    int room, d, b;

    if (small) {
        nd = (int)rnd(1, 3);
        nb = (int)rnd(1, 3);
    } else {
        shape = (int)(gen_seed() % 3);
        nd = shape == 1 ? (int)rnd(1, 3) : (int)rnd(2, 20);
        nb = shape == 1 ? (int)rnd(1, 5) : 30;
    }
    for (i = 0; i < nd; i++)
        make_domain(doms[i], small);
    for (i = 0; i < nb; i++) {
        if (small)
            len = (int)rnd(0, 3);
        else
            len = rnd_chance(3) ? 0 : (int)rnd(1, 30);
        for (k = 0; k < len; k++)
            bases[i][k] = rnd_char(small ? "ab" : "abcde");
        bases[i][len] = '\0';
    }
    for (i = 0; i < n; i++) {
        d = (int)rnd(0, nd - 1);
        room = 99 - (int)strlen(doms[d]);
        if (shape == 0 && !small) {
            len = rnd_chance(20) ? room : (int)rnd(1, 30);
            for (k = 0; k < len; k++)
                user[k] = rnd_char("abcdefghijklmnopqrstuvwxyz");
            user[len] = '\0';
        } else {
            b = (int)rnd(0, nb - 1);
            dress(user, bases[b], room, small ? 30 : 10, small ? 2 : room);
        }
        strcpy(addr[i], user);
        strcat(addr[i], "@");
        strcat(addr[i], doms[d]);
        ptrs[i] = addr[i];
    }
    put_strings(ptrs, n);
}
