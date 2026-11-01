/* Slow and obviously right: start at num1 and take |num2| steps of 1,
 * upwards when num2 is positive and downwards when it is negative. */
int sum(int num1, int num2)
{
    int total = num1, i;

    for (i = 0; i < num2; i++)
        total++;
    for (i = 0; i > num2; i--)
        total--;
    return total;
}
