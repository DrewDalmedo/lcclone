/* Walk down from the root: the ordering says which side can hold val. */
struct TreeNode* searchBST(struct TreeNode* root, int val) {
    while (root && root->val != val)
        root = val < root->val ? root->left : root->right;
    return root;
}
