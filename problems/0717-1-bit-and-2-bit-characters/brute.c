/* Slow and obviously right, from the definition: the final 0 is read on
 * its own exactly when everything before it splits into whole
 * instructions. Search for such a split, trying at each position every
 * instruction that could start there and still fit. */
static bool splits(const int *bits, int from, int end)
{
    if (from == end)
        return true;
    if (bits[from] == 0 && splits(bits, from + 1, end))
        return true;
    if (bits[from] == 1 && from + 2 <= end && splits(bits, from + 2, end))
        return true;
    return false;
}

bool isOneBitCharacter(int *bits, int bitsSize)
{
    return splits(bits, 0, bitsSize - 1);
}
