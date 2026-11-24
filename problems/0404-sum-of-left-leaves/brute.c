/* Recursion that tells each node which side of its parent it hangs on. */
static int leaves_from(struct TreeNode* p, int is_left) {
    if (!p)
        return 0;
    if (!p->left && !p->right)
        return is_left ? p->val : 0;
    return leaves_from(p->left, 1) + leaves_from(p->right, 0);
}

int sumOfLeftLeaves(struct TreeNode* root) {
    return leaves_from(root, 0);
}
