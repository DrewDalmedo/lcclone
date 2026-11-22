/* Recursive: 100000 calls deep on a path. */
struct TreeNode* invertTree(struct TreeNode* root) {
    struct TreeNode* t;

    if (!root)
        return root;
    t = invertTree(root->left);
    root->left = invertTree(root->right);
    root->right = t;
    return root;
}
