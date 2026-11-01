/* Reference solution: give every child 1 dollar first, then top children
 * up from 1 to 8 with 7 dollars each while the money lasts. Too much money
 * for everyone to stop at 8 means one child takes the excess; a single
 * leftover child who would end with exactly 4 costs one 8. O(1) time. */
int distMoney(int money, int children)
{
    int k, rest;

    if (money < children)
        return -1;
    if (money > 8 * children)
        return children - 1;
    if (money == 8 * children)
        return children;
    k = (money - children) / 7;
    rest = (money - children) % 7;
    if (children - k == 1 && rest == 3)
        return k - 1;
    return k;
}
