/* Two walkers, one twice as fast: they meet only if the walk never ends. */
bool hasCycle(struct ListNode *head) {
    struct ListNode *slow = head, *fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return true;
    }
    return false;
}
