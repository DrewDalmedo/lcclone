/* Reference solution: say the top row's color has a balls and the other
 * color b. The rows alternate, so o odd rows (1, 3, 5, ...) use o * o
 * balls of the first color and e even rows (2, 4, ...) use e * (e + 1)
 * of the second. Find the largest o and e that fit. Height h needs
 * ceil(h / 2) odd rows and floor(h / 2) even ones, so the best height is
 * min(2o, 2e + 1). Try both colors on top and keep the taller. */
static int tallest_with_top(int a, int b)
{
    int o = 0, e = 0;

    while ((o + 1) * (o + 1) <= a)
        o++;
    while ((e + 1) * (e + 2) <= b)
        e++;
    return 2 * o < 2 * e + 1 ? 2 * o : 2 * e + 1;
}

int maxHeightOfTriangle(int red, int blue)
{
    int x = tallest_with_top(red, blue), y = tallest_with_top(blue, red);

    return x > y ? x : y;
}
