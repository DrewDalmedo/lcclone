/* Slow and obviously right: serve the customers in order, and for each 20
 * try both ways of paying 15 back (a 10 and a 5, or three 5s), with no
 * rule for choosing between them. */
static bool serve(const int *bills, int n, int i, int fives, int tens)
{
    if (i == n)
        return true;
    if (bills[i] == 5)
        return serve(bills, n, i + 1, fives + 1, tens);
    if (bills[i] == 10)
        return fives >= 1 && serve(bills, n, i + 1, fives - 1, tens + 1);
    return (tens >= 1 && fives >= 1
            && serve(bills, n, i + 1, fives - 1, tens - 1))
           || (fives >= 3 && serve(bills, n, i + 1, fives - 3, tens));
}

bool lemonadeChange(int *bills, int billsSize)
{
    return serve(bills, billsSize, 0, 0, 0);
}
