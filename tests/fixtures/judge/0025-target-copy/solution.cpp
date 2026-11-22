class Solution {
public:
    TreeNode* getTargetCopy(TreeNode* original, TreeNode* cloned,
                            TreeNode* target) {
        if (!original) return nullptr;
        if (original == target) return cloned;
        TreeNode* l = getTargetCopy(original->left, cloned->left, target);
        return l ? l : getTargetCopy(original->right, cloned->right, target);
    }
};
