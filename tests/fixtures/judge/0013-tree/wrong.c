/* Counts the left side only. */
int depth(struct TreeNode* root) {
    int n = 0;

    for (; root; root = root->left)
        n++;
    return n;
}
