/* Finds each of the two nodes on its own, with its depth and parent. */
static void locate(struct TreeNode *t, struct TreeNode *up, int depth, int v,
                   int *where, struct TreeNode **parent)
{
    if (!t)
        return;
    if (t->val == v) {
        *where = depth;
        *parent = up;
    }
    locate(t->left, t, depth + 1, v, where, parent);
    locate(t->right, t, depth + 1, v, where, parent);
}

bool isCousins(struct TreeNode* root, int x, int y)
{
    struct TreeNode *px = NULL, *py = NULL;
    int dx = -1, dy = -1;

    locate(root, NULL, 0, x, &dx, &px);
    locate(root, NULL, 0, y, &dy, &py);
    return dx == dy && px != py;
}
