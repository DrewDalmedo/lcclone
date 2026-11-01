/* Slow and obviously right: paint the whole board from the definition.
 * h1 is light; each square to its left on rank 1 has the other colour
 * from its right-hand neighbour, and each square above rank 1 the other
 * colour from the square below it. Then look the square up. */
bool squareIsWhite(char *coordinates)
{
    int light[8][8], file, rank;

    light[0][7] = 1;
    for (file = 6; file >= 0; file--)
        light[0][file] = !light[0][file + 1];
    for (rank = 1; rank < 8; rank++)
        for (file = 0; file < 8; file++)
            light[rank][file] = !light[rank - 1][file];
    return light[coordinates[1] - '1'][coordinates[0] - 'a'] != 0;
}
