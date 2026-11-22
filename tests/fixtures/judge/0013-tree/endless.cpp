class Solution {
public:
    int depth(TreeNode* root) {
        volatile int pad[16];
        pad[0] = root ? root->val : 0;
        return depth(root) + pad[0];
    }
};
