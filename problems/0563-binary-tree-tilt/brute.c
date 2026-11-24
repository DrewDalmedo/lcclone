/* Recursively: each call returns its subtree's total and adds its node's
 * tilt to the running sum. */
static long long weigh(struct TreeNode* p, long long* tilt) {
    long long l, r;

    if (!p)
        return 0;
    l = weigh(p->left, tilt);
    r = weigh(p->right, tilt);
    *tilt += l > r ? l - r : r - l;
    return l + r + p->val;
}

int findTilt(struct TreeNode* root) {
    long long tilt = 0;

    weigh(root, &tilt);
    return (int)tilt;
}
