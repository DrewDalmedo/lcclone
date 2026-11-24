/* Keep root1's nodes. A stack holds the pairs at a shared address: add the
 * values, and where root1 has no child, graft root2's whole subtree. */
struct TreeNode* mergeTrees(struct TreeNode* root1, struct TreeNode* root2) {
    struct TreeNode *mine[2000], *theirs[2000];
    int top = 1;

    if (!root1)
        return root2;
    if (!root2)
        return root1;
    mine[0] = root1;
    theirs[0] = root2;
    while (top > 0) {
        struct TreeNode *a, *b;

        top--;
        a = mine[top];
        b = theirs[top];
        a->val += b->val;
        if (!a->left) {
            a->left = b->left;
        } else if (b->left) {
            mine[top] = a->left;
            theirs[top] = b->left;
            top++;
        }
        if (!a->right) {
            a->right = b->right;
        } else if (b->right) {
            mine[top] = a->right;
            theirs[top] = b->right;
            top++;
        }
    }
    return root1;
}
