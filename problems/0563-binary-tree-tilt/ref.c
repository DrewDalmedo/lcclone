/* Lists the nodes breadth first, then goes through them backwards, so that
 * each node has the totals of its two subtrees when it is reached. No
 * recursion. A subtree's total is at most 10^7 in size; the sum of tilts is
 * kept in a long long and fits an int by the constraints. */
int findTilt(struct TreeNode* root) {
    struct item {
        struct TreeNode* node;
        int parent, left, right;
    } *q;
    int head, tail = 1;
    long long total = 0;

    if (!root)
        return 0;
    q = malloc(10001 * sizeof *q);
    q[0].node = root;
    q[0].parent = -1;
    q[0].left = q[0].right = 0;
    for (head = 0; head < tail; head++) {
        struct TreeNode* kids[2];
        int k;

        kids[0] = q[head].node->left;
        kids[1] = q[head].node->right;
        for (k = 0; k < 2; k++) {
            if (kids[k]) {
                q[tail].node = kids[k];
                q[tail].parent = head;
                q[tail].left = q[tail].right = 0;
                tail++;
            }
        }
    }
    for (head = tail - 1; head >= 0; head--) {
        int l = q[head].left, r = q[head].right, p = q[head].parent;
        int sum = l + r + q[head].node->val;

        total += l > r ? l - r : r - l;
        if (p >= 0 && q[p].node->left == q[head].node)
            q[p].left = sum;
        else if (p >= 0)
            q[p].right = sum;
    }
    free(q);
    return (int)total;
}
