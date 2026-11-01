/* Reference solution: the first flip loses exactly the zeros at the end of
 * num, and the second flip puts every other digit back in place. So num
 * comes back unless it ends in 0, except for 0 itself, which flips to 0. */
bool isSameAfterReversals(int num)
{
    return num == 0 || num % 10 != 0;
}
