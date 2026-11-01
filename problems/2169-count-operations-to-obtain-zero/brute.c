/* Slow and obviously right: make the moves one at a time, as the
 * statement describes them. */
int countOperations(int num1, int num2)
{
    int moves = 0;

    while (num1 != 0 && num2 != 0) {
        if (num1 >= num2)
            num1 -= num2;
        else
            num2 -= num1;
        moves++;
    }
    return moves;
}
