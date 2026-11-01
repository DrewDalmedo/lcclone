/* Reference solution: number the files and ranks from 0. Squares with the
 * same parity of file + rank share a colour, and h1 (7 + 0, odd) is
 * light, so a square is light exactly when file + rank is odd. */
bool squareIsWhite(char *coordinates)
{
    int file = coordinates[0] - 'a', rank = coordinates[1] - '1';

    return (file + rank) % 2 == 1;
}
