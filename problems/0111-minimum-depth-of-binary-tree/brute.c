/* From the definition: a walk goes on through whichever children exist. */
int minDepth(struct TreeNode* root) {
    int l, r;

    if (!root)
        return 0;
    if (!root->left)
        return 1 + minDepth(root->right);
    if (!root->right)
        return 1 + minDepth(root->left);
    l = minDepth(root->left);
    r = minDepth(root->right);
    return 1 + (l < r ? l : r);
}
