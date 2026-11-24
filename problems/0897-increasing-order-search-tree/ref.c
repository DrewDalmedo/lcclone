/* Rotate each left child up until the node in hand has none; it is then the
 * next rung, and the walk moves on to its right child. No stack is needed. */
struct TreeNode* increasingBST(struct TreeNode* root) {
    struct TreeNode top, *tail = &top;

    top.left = NULL;
    top.right = root;
    while (tail->right) {
        struct TreeNode *node = tail->right, *up = node->left;

        if (up) {
            node->left = up->right;
            up->right = node;
            tail->right = up;
        } else {
            tail = node;
        }
    }
    return top.right;
}
