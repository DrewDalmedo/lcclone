/* Reference solution: two plots overlap exactly when their spans along x
 * share more than a point and so do their spans along y. Only coordinates
 * are compared, never subtracted or multiplied, so nothing can overflow. */
bool isRectangleOverlap(int *rec1, int rec1Size, int *rec2, int rec2Size)
{
    int west = rec1[0] > rec2[0] ? rec1[0] : rec2[0];
    int east = rec1[2] < rec2[2] ? rec1[2] : rec2[2];
    int south = rec1[1] > rec2[1] ? rec1[1] : rec2[1];
    int north = rec1[3] < rec2[3] ? rec1[3] : rec2[3];

    (void)rec1Size;
    (void)rec2Size;
    return west < east && south < north;
}
