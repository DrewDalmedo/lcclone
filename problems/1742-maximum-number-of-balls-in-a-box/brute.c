/* Slow and obviously right: for each box label from 1 up to 60 (more than
 * any digit sum here), write every ball's number out in decimal, add up
 * its digits, and count the balls that belong in that box. */
int countBalls(int lowLimit, int highLimit)
{
    char buf[16];
    int best = 0, box, b, here, sum, i;

    for (box = 1; box <= 60; box++) {
        here = 0;
        for (b = lowLimit; b <= highLimit; b++) {
            sprintf(buf, "%d", b);
            sum = 0;
            for (i = 0; buf[i]; i++)
                sum += buf[i] - '0';
            if (sum == box)
                here++;
        }
        if (here > best)
            best = here;
    }
    return best;
}
