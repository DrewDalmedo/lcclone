/* Recursion that records each route's bits and, at a leaf, reads the numeral
 * again from its first 1. */
static long long read_route(const char* bits, int len) {
    long long value = 0;
    int i = 0;

    while (i < len && !bits[i])
        i++;
    for (; i < len; i++)
        value = value * 2 + bits[i];
    return value;
}

static long long over_leaves(struct TreeNode* p, char* bits, int len) {
    long long sum = 0;

    bits[len++] = (char)p->val;
    if (!p->left && !p->right)
        return read_route(bits, len);
    if (p->left)
        sum += over_leaves(p->left, bits, len);
    if (p->right)
        sum += over_leaves(p->right, bits, len);
    return sum;
}

int sumRootToLeaf(struct TreeNode* root) {
    char bits[1001];

    return (int)over_leaves(root, bits, 0);
}
