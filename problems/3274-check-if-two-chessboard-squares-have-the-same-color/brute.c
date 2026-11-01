/* Slow and obviously right: paint the whole board, starting from a1 and
 * flipping the colour at every step to a square that shares an edge, then
 * look both squares up. */
bool checkTwoChessboards(char *coordinate1, char *coordinate2)
{
    int colour[8][8], col, row;

    for (col = 0; col < 8; col++)
        for (row = 0; row < 8; row++) {
            if (col == 0 && row == 0)
                colour[col][row] = 0;
            else if (row == 0)
                colour[col][row] = !colour[col - 1][row];
            else
                colour[col][row] = !colour[col][row - 1];
        }
    return colour[coordinate1[0] - 'a'][coordinate1[1] - '1'] ==
           colour[coordinate2[0] - 'a'][coordinate2[1] - '1'];
}
