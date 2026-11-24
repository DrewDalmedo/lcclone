/* Straight from the definition: a leaf must hold what is left of the target,
 * and any other node hands the rest to whichever children it has. */
bool hasPathSum(struct TreeNode* root, int targetSum) {
    if (!root)
        return false;
    if (!root->left && !root->right)
        return root->val == targetSum;
    return hasPathSum(root->left, targetSum - root->val) ||
           hasPathSum(root->right, targetSum - root->val);
}
