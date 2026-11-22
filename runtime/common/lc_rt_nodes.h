/*
 * lc_rt_nodes.h: lists and trees in the harness, for C and C++ builds
 * alike. lc places it after lc_rt_core.h, when the problem uses a node
 * type (LC_LISTNODE, LC_TREENODE or LC_NARYNODE, which the prelude
 * declares the structs for), and lc_rt.c or lc_rt.cpp builds on it.
 *
 * Nothing here recurses: a solution may be handed, or may return, a list
 * of 10^5 nodes or a tree as deep as it is long, and the harness itself
 * must not run out of stack on it. Each node is allocated on its own, with
 * malloc in C and new in C++, so that a solution may free or delete the
 * nodes it is given, as on LeetCode. The harness never frees them itself.
 *
 * A list or a tree that a solution returns is checked as it is written: a
 * list with a cycle, or a tree that reaches a node twice (a cycle, or a
 * node with two parents), can't be written, and the harness stops with a
 * message that says where (lc_rt_fail: a Runtime Error), where LeetCode's
 * own harness would run until it ran out of time or memory.
 */

#if defined(LC_LISTNODE) || defined(LC_TREENODE) || defined(LC_NARYNODE)

#ifdef __cplusplus
#ifdef LC_LISTNODE
typedef ListNode lc_rt_list;
#endif
#ifdef LC_TREENODE
typedef TreeNode lc_rt_tree;
#endif
#ifdef LC_NARYNODE
typedef Node lc_rt_nary;
#endif
#else
#ifdef LC_LISTNODE
typedef struct ListNode lc_rt_list;
#endif
#ifdef LC_TREENODE
typedef struct TreeNode lc_rt_tree;
#endif
#ifdef LC_NARYNODE
typedef struct Node lc_rt_nary;
#endif
#endif

/* ------------------------------------------------------------------ */
/* Tables of nodes, growable arrays and sets of pointers               */
/* ------------------------------------------------------------------ */

/*
 * The nodes the harness built for an input, in the order of its literal
 * (level order for a tree), for a builder (build:) or a node reference
 * (ref:) to find them by index.
 */
typedef struct lc_rt_nodes {
    void **at;
    int n;
} lc_rt_nodes;

LC_RT void lc_rt_nodes_add(lc_rt_nodes *t, void *node)
{
    if (t)
        t->at[t->n++] = node;
}

/* Node i of an input; lc sends only indexes that it checked. */
LC_RT void *lc_rt_node_at(const lc_rt_nodes *t, int i)
{
    if (i < 0 || i >= t->n)
        lc_rt_fail("node %d is not in a list or tree of %d nodes", i, t->n);
    return t->at[i];
}

/* Fails unless `node` is NULL or one of t's: the result of a function
 * that must return a node of an input (ref: return in NAME). */
LC_RT void lc_rt_check_ref(const char *fn, const void *node,
                           const lc_rt_nodes *t, const char *name)
{
    int i;

    if (!node)
        return;
    for (i = 0; i < t->n; i++)
        if (t->at[i] == node)
            return;
    lc_rt_fail("%s returned a node that is not one of %s's", fn, name);
}

/* A queue of nodes: appends p to q[0..*len), of capacity *cap. */
LC_RT const void **lc_rt_push(const void **q, size_t *len, size_t *cap,
                              const void *p)
{
    if (*len == *cap) {
        const void **b;

        *cap = *cap ? *cap * 2 : 64;
        b = (const void **)realloc((void *)q, *cap * sizeof *q);
        if (!b)
            lc_rt_fail("out of memory");
        q = b;
    }
    q[(*len)++] = p;
    return q;
}

/* A set of pointers: open addressing, at most half full. */
typedef struct lc_rt_set {
    uintptr_t *slot;
    size_t cap, n;
} lc_rt_set;

LC_RT size_t lc_rt_hash(uintptr_t p, size_t cap)
{
    unsigned long long h = (unsigned long long)p * 0x9e3779b97f4a7c15ull;

    return (size_t)(h >> 20) & (cap - 1);
}

/* Adds p (not NULL); returns 0 if it was there already. */
LC_RT int lc_rt_set_add(lc_rt_set *s, const void *p)
{
    uintptr_t v = (uintptr_t)p;
    size_t i;

    if (2 * (s->n + 1) > s->cap) {
        size_t cap = s->cap ? s->cap * 2 : 256, k;
        uintptr_t *slot = (uintptr_t *)lc_rt_alloc(cap, sizeof *slot);

        for (k = 0; k < s->cap; k++) {
            if (!s->slot[k])
                continue;
            for (i = lc_rt_hash(s->slot[k], cap); slot[i];
                 i = (i + 1) & (cap - 1))
                ;
            slot[i] = s->slot[k];
        }
        free(s->slot);
        s->slot = slot;
        s->cap = cap;
    }
    for (i = lc_rt_hash(v, s->cap); s->slot[i]; i = (i + 1) & (s->cap - 1))
        if (s->slot[i] == v)
            return 0;
    s->slot[i] = v;
    s->n++;
    return 1;
}

LC_RT void lc_rt_set_free(lc_rt_set *s)
{
    free(s->slot);
    s->slot = NULL;
    s->cap = s->n = 0;
}

/* The items of a list or tree on the wire: the count, then each value,
 * or for a tree a null as `n`. *isnull may be NULL for a list. */
LC_RT int lc_rt_read_items(int **vals, unsigned char **isnull)
{
    int n = lc_rt_count(), i;

    *vals = (int *)lc_rt_alloc((size_t)n, sizeof **vals);
    if (isnull)
        *isnull = (unsigned char *)lc_rt_alloc((size_t)n, 1);
    for (i = 0; i < n; i++) {
        lc_rt_skip_space();
        if (isnull && lc_rt_in_pos < lc_rt_in_len &&
            lc_rt_in[lc_rt_in_pos] == 'n') {
            (void)lc_rt_token();
            (*isnull)[i] = 1;
        } else {
            (*vals)[i] = (int)lc_rt_ll(INT_MIN, INT_MAX);
        }
    }
    return n;
}

/* The tokens of a tree being written: values, and nulls as LC_RT_NULL. */
#define LC_RT_NULL LLONG_MIN

typedef struct lc_rt_tokens {
    long long *v;
    size_t n, cap;
} lc_rt_tokens;

LC_RT void lc_rt_tokens_add(lc_rt_tokens *t, long long v)
{
    if (t->n == t->cap) {
        long long *p;

        t->cap = t->cap ? t->cap * 2 : 64;
        p = (long long *)realloc(t->v, t->cap * sizeof *p);
        if (!p)
            lc_rt_fail("out of memory");
        t->v = p;
    }
    t->v[t->n++] = v;
}

/* Writes the tokens as a tree's wire form, without the trailing nulls,
 * and frees them. */
LC_RT void lc_rt_tokens_put(lc_rt_tokens *t)
{
    size_t i, n = t->n;

    while (n > 0 && t->v[n - 1] == LC_RT_NULL)
        n--;
    lc_rt_put_ll((long long)n);
    for (i = 0; i < n; i++) {
        if (t->v[i] == LC_RT_NULL) {
            lc_rt_sep();
            lc_rt_put("n", 1);
        } else {
            lc_rt_put_ll(t->v[i]);
        }
    }
    free(t->v);
}

/* ------------------------------------------------------------------ */
/* ListNode                                                            */
/* ------------------------------------------------------------------ */

#ifdef LC_LISTNODE
LC_RT lc_rt_list *lc_rt_new_list(int v)
{
#ifdef __cplusplus
    return new ListNode(v);
#else
    lc_rt_list *p = (lc_rt_list *)lc_rt_alloc(1, sizeof *p);

    p->val = v;
    p->next = NULL;
    return p;
#endif
}

/* Reads a list; t (may be NULL) gets its nodes. */
LC_RT lc_rt_list *lc_rt_read_list(lc_rt_nodes *t)
{
    int *vals, n = lc_rt_read_items(&vals, NULL), i;
    lc_rt_list *head = NULL, *tail = NULL;

    if (t) {
        t->at = (void **)lc_rt_alloc((size_t)n, sizeof *t->at);
        t->n = 0;
    }
    for (i = 0; i < n; i++) {
        lc_rt_list *p = lc_rt_new_list(vals[i]);

        if (tail)
            tail->next = p;
        else
            head = p;
        tail = p;
        lc_rt_nodes_add(t, p);
    }
    free(vals);
    return head;
}

/*
 * Writes a list, after making sure it ends: Brent's method finds a cycle
 * in one pass and no extra memory, and then where it starts (mu) and its
 * length (lambda).
 */
LC_RT void lc_rt_write_list(const char *fn, const lc_rt_list *head)
{
    const lc_rt_list *slow = head, *fast, *p;
    long long power = 1, lambda = 1, mu = 0, n = 0, i;

    fast = head ? head->next : NULL;
    while (fast && fast != slow) {
        if (power == lambda) {
            slow = fast;
            power *= 2;
            lambda = 0;
        }
        fast = fast->next;
        lambda++;
    }
    if (fast) {
        slow = fast = head;
        for (i = 0; i < lambda; i++)
            fast = fast->next;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
            mu++;
        }
        lc_rt_fail("%s returned a list with a cycle: its node at index %lld "
                   "links back to the one at index %lld", fn,
                   mu + lambda - 1, mu);
    }
    for (p = head; p; p = p->next)
        n++;
    lc_rt_put_ll(n);
    for (p = head; p; p = p->next)
        lc_rt_put_ll(p->val);
}

/* build: cycle LIST POS. */
LC_RT void lc_rt_cycle(const lc_rt_nodes *t, int pos)
{
    if (pos >= 0)
        ((lc_rt_list *)lc_rt_node_at(t, t->n - 1))->next =
            (lc_rt_list *)lc_rt_node_at(t, pos);
}

/*
 * build: intersect A B SKIPA SKIPB: B's nodes from skipB on are replaced
 * by A's from skipA on (lc made sure they hold the same values), unless
 * the lists don't join (skipA is A's length). B's own nodes there are
 * freed.
 */
LC_RT void lc_rt_intersect(const lc_rt_nodes *a, const lc_rt_nodes *b,
                           lc_rt_list **headB, int skipA, int skipB)
{
    int i;

    if (skipA >= a->n)
        return;
    if (skipB == 0)
        *headB = (lc_rt_list *)lc_rt_node_at(a, skipA);
    else
        ((lc_rt_list *)lc_rt_node_at(b, skipB - 1))->next =
            (lc_rt_list *)lc_rt_node_at(a, skipA);
    for (i = skipB; i < b->n; i++) {
#ifdef __cplusplus
        delete (lc_rt_list *)b->at[i];
#else
        free(b->at[i]);
#endif
    }
}
#endif /* LC_LISTNODE */

/* ------------------------------------------------------------------ */
/* TreeNode                                                            */
/* ------------------------------------------------------------------ */

#ifdef LC_TREENODE
LC_RT lc_rt_tree *lc_rt_new_tree(int v)
{
#ifdef __cplusplus
    return new TreeNode(v);
#else
    lc_rt_tree *p = (lc_rt_tree *)lc_rt_alloc(1, sizeof *p);

    p->val = v;
    p->left = p->right = NULL;
    return p;
#endif
}

/*
 * Reads a tree in level order: item i, after the root, is child slot
 * i - 1, the left (even) or right (odd) child of the (i - 1) / 2-th node
 * made. t (may be NULL) gets the nodes, in that order.
 */
LC_RT lc_rt_tree *lc_rt_read_tree(lc_rt_nodes *t)
{
    int *vals, n, i, made = 0;
    unsigned char *isnull;
    lc_rt_tree **nodes, *root = NULL;

    n = lc_rt_read_items(&vals, &isnull);
    nodes = (lc_rt_tree **)lc_rt_alloc((size_t)n, sizeof *nodes);
    if (t) {
        t->at = (void **)lc_rt_alloc((size_t)n, sizeof *t->at);
        t->n = 0;
    }
    for (i = 0; i < n; i++) {
        lc_rt_tree *p;

        if (isnull[i]) {
            if (i == 0)
                lc_rt_fail("a tree starts with null");
            continue;
        }
        p = lc_rt_new_tree(vals[i]);
        if (i == 0) {
            root = p;
        } else {
            int parent = (i - 1) / 2;

            if (parent >= made)
                lc_rt_fail("item %d of a tree has no parent", i);
            if ((i - 1) % 2 == 0)
                nodes[parent]->left = p;
            else
                nodes[parent]->right = p;
        }
        nodes[made++] = p;
        lc_rt_nodes_add(t, p);
    }
    free(nodes);
    free(vals);
    free(isnull);
    return root;
}

/* Writes a tree in level order, breadth first; fails on a node reached
 * twice. */
LC_RT void lc_rt_write_tree(const char *fn, const lc_rt_tree *root)
{
    const void **queue = NULL;
    size_t len = 0, cap = 0, i, k;
    lc_rt_tokens tok;
    lc_rt_set seen;

    memset(&tok, 0, sizeof tok);
    memset(&seen, 0, sizeof seen);
    if (root) {
        queue = lc_rt_push(queue, &len, &cap, root);
        lc_rt_set_add(&seen, root);
        lc_rt_tokens_add(&tok, root->val);
    }
    for (i = 0; i < len; i++) {
        const lc_rt_tree *p = (const lc_rt_tree *)queue[i], *kid[2];

        kid[0] = p->left;
        kid[1] = p->right;
        for (k = 0; k < 2; k++) {
            if (!kid[k]) {
                lc_rt_tokens_add(&tok, LC_RT_NULL);
                continue;
            }
            if (!lc_rt_set_add(&seen, kid[k]))
                lc_rt_fail("%s returned a tree that reaches a node twice (a "
                           "cycle, or a node with two parents): the node "
                           "whose value is %d", fn, kid[k]->val);
            queue = lc_rt_push(queue, &len, &cap, kid[k]);
            lc_rt_tokens_add(&tok, kid[k]->val);
        }
    }
    lc_rt_tokens_put(&tok);
    lc_rt_set_free(&seen);
    free((void *)queue);
}
#endif /* LC_TREENODE */

/* ------------------------------------------------------------------ */
/* NaryNode                                                            */
/* ------------------------------------------------------------------ */

#ifdef LC_NARYNODE
LC_RT lc_rt_nary *lc_rt_new_nary(int v)
{
#ifdef __cplusplus
    return new Node(v);
#else
    lc_rt_nary *p = (lc_rt_nary *)lc_rt_alloc(1, sizeof *p);

    p->val = v;
    p->numChildren = 0;
    p->children = NULL;
    return p;
#endif
}

#ifdef __cplusplus
#define LC_RT_KIDS(p) ((int)(p)->children.size())
#define LC_RT_KID(p, i) ((p)->children[(size_t)(i)])
#else
#define LC_RT_KIDS(p) ((p)->numChildren)
#define LC_RT_KID(p, i) ((p)->children[i])
#endif

/*
 * Reads an N-ary tree in LeetCode's level order: the root, a null, then
 * the children of each node in turn, each group ended by a null. Every
 * null moves on to the next node made. In C, a node's children are an
 * array of numChildren pointers (NULL when it has none); in C++, a vector.
 */
LC_RT lc_rt_nary *lc_rt_read_nary(lc_rt_nodes *t)
{
    int *vals, n, i, made = 0, parent = -1;
    unsigned char *isnull;
    lc_rt_nary **nodes, *root = NULL;
#ifndef __cplusplus
    int *kids;
#endif

    n = lc_rt_read_items(&vals, &isnull);
    nodes = (lc_rt_nary **)lc_rt_alloc((size_t)n, sizeof *nodes);
    if (t) {
        t->at = (void **)lc_rt_alloc((size_t)n, sizeof *t->at);
        t->n = 0;
    }
#ifndef __cplusplus
    /* C: count each node's children first, for its array. */
    kids = (int *)lc_rt_alloc((size_t)n, sizeof *kids);
    for (i = 1; i < n; i++) {
        if (isnull[i])
            parent++;
        else if (parent >= 0 && parent < n)
            kids[parent]++;
    }
    parent = -1;
#endif
    for (i = 0; i < n; i++) {
        lc_rt_nary *p;

        if (isnull[i]) {
            if (i == 0)
                lc_rt_fail("an N-ary tree starts with null");
            parent++;
            continue;
        }
        p = lc_rt_new_nary(vals[i]);
        if (i == 0) {
            root = p;
        } else {
            lc_rt_nary *up;

            if (parent < 0 || parent >= made)
                lc_rt_fail("item %d of an N-ary tree has no parent", i);
            up = nodes[parent];
#ifdef __cplusplus
            up->children.push_back(p);
#else
            if (!up->children)
                up->children = (lc_rt_nary **)lc_rt_alloc(
                    (size_t)kids[parent], sizeof *up->children);
            up->children[up->numChildren++] = p;
#endif
        }
        nodes[made++] = p;
        lc_rt_nodes_add(t, p);
    }
#ifndef __cplusplus
    free(kids);
#endif
    free(nodes);
    free(vals);
    free(isnull);
    return root;
}

/*
 * Writes an N-ary tree in the same order, breadth first; fails on a node
 * reached twice or a NULL child.
 */
LC_RT void lc_rt_write_nary(const char *fn, const lc_rt_nary *root)
{
    const void **queue = NULL;
    size_t len = 0, cap = 0, i;
    lc_rt_tokens tok;
    lc_rt_set seen;
    int k;

    memset(&tok, 0, sizeof tok);
    memset(&seen, 0, sizeof seen);
    if (root) {
        queue = lc_rt_push(queue, &len, &cap, root);
        lc_rt_set_add(&seen, root);
        lc_rt_tokens_add(&tok, root->val);
        lc_rt_tokens_add(&tok, LC_RT_NULL);
    }
    for (i = 0; i < len; i++) {
        const lc_rt_nary *p = (const lc_rt_nary *)queue[i];

#ifndef __cplusplus
        if (p->numChildren < 0)
            lc_rt_fail("%s returned a tree with a node whose numChildren is "
                       "%d (its value is %d)", fn, p->numChildren, p->val);
        if (p->numChildren > 0 && !p->children)
            lc_rt_fail("%s returned a tree with a node whose numChildren is "
                       "%d but whose children is NULL (its value is %d)", fn,
                       p->numChildren, p->val);
#endif
        for (k = 0; k < LC_RT_KIDS(p); k++) {
            const lc_rt_nary *c = LC_RT_KID(p, k);

            if (!c)
                lc_rt_fail("%s returned a tree with a NULL child (of the node "
                           "whose value is %d)", fn, p->val);
            if (!lc_rt_set_add(&seen, c))
                lc_rt_fail("%s returned a tree that reaches a node twice (a "
                           "cycle, or a node with two parents): the node "
                           "whose value is %d", fn, c->val);
            queue = lc_rt_push(queue, &len, &cap, c);
            lc_rt_tokens_add(&tok, c->val);
        }
        lc_rt_tokens_add(&tok, LC_RT_NULL);
    }
    lc_rt_tokens_put(&tok);
    lc_rt_set_free(&seen);
    free((void *)queue);
}
#endif /* LC_NARYNODE */

#endif /* any node type */
