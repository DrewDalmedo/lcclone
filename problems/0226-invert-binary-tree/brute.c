/* Build a new tree: each copy's left is the turned right subtree, and its
 * right the turned left subtree. */
struct TreeNode* invertTree(struct TreeNode* root) {
    struct TreeNode *copy;

    if (!root)
        return NULL;
    copy = malloc(sizeof *copy);
    copy->val = root->val;
    copy->left = invertTree(root->right);
    copy->right = invertTree(root->left);
    return copy;
}
