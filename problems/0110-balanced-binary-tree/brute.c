/* Straight from the definition: measure both branches of each node. */
static int height(struct TreeNode* p) {
    int l, r;

    if (!p)
        return 0;
    l = height(p->left);
    r = height(p->right);
    return 1 + (l > r ? l : r);
}

bool isBalanced(struct TreeNode* root) {
    int l, r;

    if (!root)
        return true;
    l = height(root->left);
    r = height(root->right);
    if (l - r > 1 || r - l > 1)
        return false;
    return isBalanced(root->left) && isBalanced(root->right);
}
