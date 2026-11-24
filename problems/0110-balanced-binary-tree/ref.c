/* Lists the nodes breadth first, so that each parent comes before its
 * children, then goes through them backwards: by the time a node is
 * reached, its children have reported their heights. No recursion. */
bool isBalanced(struct TreeNode* root) {
    struct item {
        struct TreeNode* node;
        int parent, left, right;
    } *q;
    int head, tail = 0;
    bool ok = true;

    if (!root)
        return true;
    q = malloc(5001 * sizeof *q);
    q[0].node = root;
    q[0].parent = -1;
    q[0].left = q[0].right = 0;
    tail = 1;
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
    for (head = tail - 1; head >= 0 && ok; head--) {
        int l = q[head].left, r = q[head].right, p = q[head].parent;
        int height = 1 + (l > r ? l : r);

        if (l - r > 1 || r - l > 1)
            ok = false;
        else if (p >= 0 && q[p].node->left == q[head].node)
            q[p].left = height;
        else if (p >= 0)
            q[p].right = height;
    }
    free(q);
    return ok;
}
