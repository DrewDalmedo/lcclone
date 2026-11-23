/* Straight from the definition: one more than the deepest child. */
int maxDepth(struct Node* root) {
    int best = 0, i;

    if (!root)
        return 0;
    for (i = 0; i < root->numChildren; i++) {
        int d = maxDepth(root->children[i]);

        if (d > best)
            best = d;
    }
    return best + 1;
}
