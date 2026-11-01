/* Slow and obviously right: for each color on top and every height up to
 * 200, add up the balls each color needs row by row, and keep the
 * greatest height both colors can pay for. */
int maxHeightOfTriangle(int red, int blue)
{
    int best = 0, top, h, k;

    for (top = 0; top < 2; top++)
        for (h = 1; h <= 200; h++) {
            int need_red = 0, need_blue = 0;

            for (k = 1; k <= h; k++)
                if ((k % 2 == 1) == (top == 0))
                    need_red += k;
                else
                    need_blue += k;
            if (need_red <= red && need_blue <= blue && h > best)
                best = h;
        }
    return best;
}
