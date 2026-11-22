class Solution {
public:
    Node* mirror(Node* root) {
        if (!root) return root;
        reverse(root->children.begin(), root->children.end());
        for (Node* c : root->children) mirror(c);
        return root;
    }
};
