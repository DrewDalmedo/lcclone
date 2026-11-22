// A new tree, freeing the old one's nodes as it goes.
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        TreeNode* l = invertTree(root->left);
        TreeNode* r = invertTree(root->right);
        TreeNode* n = new TreeNode(root->val, r, l);
        delete root;
        return n;
    }
};
