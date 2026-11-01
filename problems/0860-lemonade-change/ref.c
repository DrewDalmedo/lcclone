/* Reference solution: only the numbers of 5s and 10s in the box matter,
 * since a 20 is never given back. A 10 takes a 5. A 20 takes a 10 and a 5
 * if there is a 10, else three 5s: keeping the 5s is never worse, because
 * every change needs them and a 10 is good only for a 20. */
bool lemonadeChange(int *bills, int billsSize)
{
    int fives = 0, tens = 0, i;

    for (i = 0; i < billsSize; i++) {
        if (bills[i] == 5) {
            fives++;
        } else if (bills[i] == 10) {
            if (fives == 0)
                return false;
            fives--;
            tens++;
        } else if (tens > 0 && fives > 0) {
            tens--;
            fives--;
        } else if (fives >= 3) {
            fives -= 3;
        } else {
            return false;
        }
    }
    return true;
}
