/* Takes the left item off the total and looks for the rest among all the
 * values an item can have. */
bool checkTree(struct TreeNode* root) {
    int rest = root->val - root->left->val, v;

    for (v = -100; v <= 100; v++)
        if (v == root->right->val)
            return v == rest;
    return false;
}
