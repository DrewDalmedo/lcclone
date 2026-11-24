/* Goes through the tree a level at a time, noting the parents of x and y among
 * each level's children. The first level holding either one settles it. */
bool isCousins(struct TreeNode* root, int x, int y)
{
    struct TreeNode *queue[101];
    int head = 0, tail = 0;

    queue[tail++] = root;
    while (head < tail) {
        struct TreeNode *px = NULL, *py = NULL;
        int end = tail;

        for (; head < end; head++) {
            struct TreeNode *p = queue[head];
            int k;

            for (k = 0; k < 2; k++) {
                struct TreeNode *c = k ? p->right : p->left;

                if (!c)
                    continue;
                if (c->val == x)
                    px = p;
                if (c->val == y)
                    py = p;
                queue[tail++] = c;
            }
        }
        if (px || py)
            return px && py && px != py;
    }
    return false;
}
