/* Each pointer walks its own list and then the other one, so both travel
 * m + n steps and reach the first shared node (or the end) together. */
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    struct ListNode *a = headA, *b = headB;

    while (a != b) {
        a = a ? a->next : headB;
        b = b ? b->next : headA;
    }
    return a;
}
