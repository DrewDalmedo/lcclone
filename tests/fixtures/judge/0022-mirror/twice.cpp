// Gives the root's first child a second parent.
class Solution {
public:
    Node* mirror(Node* root) {
        if (!root) return root;
        reverse(root->children.begin(), root->children.end());
        if (root->children.size() >= 2)
            root->children.back()->children.push_back(root->children[0]);
        return root;
    }
};
