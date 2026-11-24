/* Recursion straight from the definition, always evaluating both inputs. */
bool evaluateTree(struct TreeNode* root) {
    bool a, b;

    if (!root->left)
        return root->val == 1;
    a = evaluateTree(root->left);
    b = evaluateTree(root->right);
    return root->val == 2 ? (a || b) : (a && b);
}
