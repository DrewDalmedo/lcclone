/* Reference solution: keep the label of the snake's square and change it
 * by n for a move between rows and by 1 for a move between columns. The
 * first letter tells the four commands apart. Since no move leaves the
 * board, the label never needs its row and column split out. */
int finalPositionOfSnake(int n, char **commands, int commandsSize)
{
    int pos = 0, i;

    for (i = 0; i < commandsSize; i++)
        switch (commands[i][0]) {
        case 'U':
            pos -= n;
            break;
        case 'D':
            pos += n;
            break;
        case 'L':
            pos--;
            break;
        default:
            pos++;
            break;
        }
    return pos;
}
