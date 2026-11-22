/* Swaps by copying one side over the other: the left child is then also
 * the right one. */
struct TreeNode* invertTree(struct TreeNode* root) {
    if (!root)
        return root;
    root->left = root->right ? root->right : root->left;
    root->right = root->left;
    return root;
}
