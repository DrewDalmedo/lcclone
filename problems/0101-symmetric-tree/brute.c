/* Straight from the definition: two positions are counterparts when their
 * children are, crosswise. */
static bool counterparts(struct TreeNode *a, struct TreeNode *b)
{
    if (!a || !b)
        return a == b;
    return a->val == b->val && counterparts(a->left, b->right) &&
           counterparts(a->right, b->left);
}

bool isSymmetric(struct TreeNode* root)
{
    return counterparts(root->left, root->right);
}
