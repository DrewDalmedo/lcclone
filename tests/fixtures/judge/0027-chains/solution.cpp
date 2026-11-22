class Solution {
public:
    vector<TreeNode*> chains(vector<ListNode*>& lists) {
        vector<TreeNode*> out;
        for (ListNode* l : lists) {
            TreeNode dummy;
            TreeNode* at = &dummy;
            for (; l; l = l->next) at = at->right = new TreeNode(l->val);
            out.push_back(dummy.right);
        }
        return out;
    }
};
