/* Slide down the left edge, stacking nodes; each node popped is next on the
 * line, and the walk carries on in its right subtree. */
int* inorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *stack[100];
    int *out = malloc(100 * sizeof *out);
    int top = 0, n = 0;

    while (root || top > 0) {
        while (root) {
            stack[top++] = root;
            root = root->left;
        }
        root = stack[--top];
        out[n++] = root->val;
        root = root->right;
    }
    *returnSize = n;
    return out;
}
