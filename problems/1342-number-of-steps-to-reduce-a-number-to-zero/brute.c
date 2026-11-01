/* Slow and obviously right: run the counter step by step and count. */
int numberOfSteps(int num)
{
    int steps = 0;

    while (num != 0) {
        if (num % 2 == 0)
            num = num / 2;
        else
            num = num - 1;
        steps++;
    }
    return steps;
}
