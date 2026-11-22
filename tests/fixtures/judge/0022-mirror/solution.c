/* Recursive: 10000 calls deep on a path. */
struct Node* mirror(struct Node* root) {
    int i;

    if (!root)
        return root;
    for (i = 0; i < root->numChildren / 2; i++) {
        struct Node* t = root->children[i];

        root->children[i] = root->children[root->numChildren - 1 - i];
        root->children[root->numChildren - 1 - i] = t;
    }
    for (i = 0; i < root->numChildren; i++)
        mirror(root->children[i]);
    return root;
}
