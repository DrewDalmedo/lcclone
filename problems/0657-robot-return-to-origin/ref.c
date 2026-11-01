/* Reference solution: count each letter. The robot is back at (0, 0)
 * exactly when it went up as often as down, and right as often as left. */
bool judgeCircle(char *moves)
{
    int count[256] = {0};

    for (; *moves; moves++)
        count[(unsigned char)*moves]++;
    return count['U'] == count['D'] && count['R'] == count['L'];
}
