/* Any number of lights; a full-size case asks for 3 to 6, the counts with
 * the most times (112 to 190 of them). */
void gen(int small)
{
    put_int(small ? rnd(0, 10) : rnd(3, 6));
}
