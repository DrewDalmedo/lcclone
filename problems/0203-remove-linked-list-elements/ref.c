/* Walk the links themselves, so that the head needs no special case. */
struct ListNode* removeElements(struct ListNode* head, int val) {
    struct ListNode **link = &head;

    while (*link) {
        if ((*link)->val == val)
            *link = (*link)->next;
        else
            link = &(*link)->next;
    }
    return head;
}
