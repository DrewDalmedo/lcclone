/* Tries every node of root as the top of the branch. */
static bool same(struct TreeNode *a, struct TreeNode *b)
{
    if (!a || !b)
        return a == b;
    return a->val == b->val && same(a->left, b->left) &&
           same(a->right, b->right);
}

bool isSubtree(struct TreeNode* root, struct TreeNode* subRoot)
{
    if (!root)
        return false;
    return same(root, subRoot) || isSubtree(root->left, subRoot) ||
           isSubtree(root->right, subRoot);
}
