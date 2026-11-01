/* Reference solution: a square's colour is the parity of its column number
 * plus its row number, so two squares match when those four numbers add up
 * to an even total. */
bool checkTwoChessboards(char *coordinate1, char *coordinate2)
{
    int total = (coordinate1[0] - 'a') + (coordinate1[1] - '1') +
                (coordinate2[0] - 'a') + (coordinate2[1] - '1');

    return total % 2 == 0;
}
