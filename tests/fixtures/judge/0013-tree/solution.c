/* Recursive, as most solutions are: 100000 levels deep on a path. */
int depth(struct TreeNode* root) {
    int l, r;

    if (!root)
        return 0;
    l = depth(root->left);
    r = depth(root->right);
    return 1 + (l > r ? l : r);
}
