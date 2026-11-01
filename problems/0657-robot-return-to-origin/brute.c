/* Slow and obviously right: walk the robot step by step, keeping its
 * position, and look at where it ends. */
bool judgeCircle(char *moves)
{
    long long x = 0, y = 0;
    size_t i;

    for (i = 0; i < strlen(moves); i++) {
        switch (moves[i]) {
        case 'U':
            y++;
            break;
        case 'D':
            y--;
            break;
        case 'R':
            x++;
            break;
        case 'L':
            x--;
            break;
        }
    }
    return x == 0 && y == 0;
}
