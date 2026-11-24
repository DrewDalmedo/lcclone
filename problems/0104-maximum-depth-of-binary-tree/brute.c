/* Straight from the definition: walk the whole tree, noting the level of
 * each node. */
static void walk(struct TreeNode* p, int level, int* deepest) {
    if (!p)
        return;
    if (level > *deepest)
        *deepest = level;
    walk(p->left, level + 1, deepest);
    walk(p->right, level + 1, deepest);
}

int maxDepth(struct TreeNode* root) {
    int deepest = 0;

    walk(root, 1, &deepest);
    return deepest;
}
