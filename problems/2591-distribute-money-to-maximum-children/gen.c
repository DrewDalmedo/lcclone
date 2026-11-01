/* An amount of money and a number of children. The seed picks the shape:
 * exactly 8 dollars a child, a little more than that, 8 for all but one
 * and 4 over (the forbidden 4), a leftover of 3 shared by two or more
 * children, about 1 dollar a child (just enough, or 1 short), or any
 * amount. Small cases have 2 to 5 children; full-size ones have 2, 30 or
 * any number of children up to 30, and sometimes 200 dollars. Amounts
 * outside 1 to 200 are pulled back inside. */
void gen(int small)
{
    int children, money, pick, extra, top;

    if (small) {
        children = (int)rnd(2, 5);
    } else {
        pick = (int)rnd(0, 3);
        children = pick == 0 ? 2 : pick == 1 ? 30 : (int)rnd(2, 30);
    }
    top = 8 * children + 8;
    if (top > 200)
        top = 200;
    pick = (int)rnd(0, small ? 7 : 8);
    switch (pick) {
    case 0: /* 8 dollars each */
        money = 8 * children;
        break;
    case 1: /* more than 8 each */
        extra = (int)rnd(1, 8);
        money = 8 * children + extra;
        break;
    case 2: /* all but one at 8, and 4 for the last */
        money = 8 * (children - 1) + 4;
        break;
    case 3: /* some children at 8, and 3 dollars over for two or more */
        extra = (int)rnd(0, children - 2);
        money = children + 7 * extra + 3;
        break;
    case 4: /* just enough, or 1 short */
        extra = (int)rnd(0, 1);
        money = children - extra;
        break;
    case 8: /* the most money */
        money = 200;
        break;
    default:
        money = (int)rnd(1, top);
    }
    if (money < 1)
        money = 1;
    if (money > 200)
        money = 200;
    put_int(money);
    put_int(children);
}
