/* Look everywhere, ignoring the ordering: the node itself, then each side. */
struct TreeNode* searchBST(struct TreeNode* root, int val) {
    struct TreeNode *hit;

    if (!root || root->val == val)
        return root;
    hit = searchBST(root->left, val);
    return hit ? hit : searchBST(root->right, val);
}
