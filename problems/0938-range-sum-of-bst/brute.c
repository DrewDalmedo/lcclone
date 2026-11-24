/* Visit every node by recursion, ignoring the ordering. */
int rangeSumBST(struct TreeNode* root, int low, int high) {
    if (!root)
        return 0;
    return (low <= root->val && root->val <= high ? root->val : 0) +
           rangeSumBST(root->left, low, high) +
           rangeSumBST(root->right, low, high);
}
