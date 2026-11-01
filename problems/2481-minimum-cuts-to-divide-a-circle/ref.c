/* Reference solution: n equal slices are wedges whose n edges are spaced
 * evenly around the centre. When n is even the edges pair up into
 * opposite ones, and a long cut makes each pair; when n is odd no edge
 * has an opposite one, so each takes a short cut. One slice needs none. */
int numberOfCuts(int n)
{
    if (n == 1)
        return 0;
    return n % 2 == 0 ? n / 2 : n;
}
