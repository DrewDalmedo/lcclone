/* Two amounts, up to 30 in small cases and up to 10^5 otherwise. Three
 * cases in eight are any two amounts; the rest have one of these shapes:
 * one of them 0, both equal, one at least twice the other and a multiple
 * of it, 1 against a large amount (the most moves), or two neighbouring
 * Fibonacci numbers (the jars trade the lead every move). Either amount
 * may come first. */
void gen(int small)
{
    int top = small ? 30 : 100000;
    int shape = (int)rnd(0, 7);
    int a, b, k, t, f0 = 1, f1 = 1;

    a = (int)rnd(0, top);
    b = (int)rnd(0, top);
    switch (shape) {
    case 3:
        b = 0;
        break;
    case 4:
        b = a;
        break;
    case 5:
        b = (int)rnd(1, top / 2);
        k = (int)rnd(2, top / b);
        a = b * k;
        break;
    case 6:
        a = 1;
        b = rnd_chance(25) ? top : (int)rnd(top / 2, top);
        break;
    case 7:
        k = (int)rnd(1, small ? 6 : 23);
        while (k-- > 0) {
            t = f0 + f1;
            f0 = f1;
            f1 = t;
        }
        a = f1;
        b = f0;
        break;
    }
    if (rnd_chance(50)) {
        t = a;
        a = b;
        b = t;
    }
    put_int(a);
    put_int(b);
}
