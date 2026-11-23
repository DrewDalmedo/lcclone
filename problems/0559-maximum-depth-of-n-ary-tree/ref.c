/* Breadth first, one level at a time. */
int maxDepth(struct Node* root) {
    struct Node **queue;
    int head = 0, tail = 0, levels = 0, i;

    if (!root)
        return 0;
    queue = malloc(10001 * sizeof *queue);
    queue[tail++] = root;
    while (head < tail) {
        int end = tail;

        levels++;
        for (; head < end; head++)
            for (i = 0; i < queue[head]->numChildren; i++)
                queue[tail++] = queue[head]->children[i];
    }
    free(queue);
    return levels;
}
