/* Find the middle with two pointers, reverse the second half, and compare
 * it with the first. No recursion and no extra memory. */
bool isPalindrome(struct ListNode* head) {
    struct ListNode *slow = head, *fast = head, *back = NULL;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    while (slow) {
        struct ListNode *next = slow->next;

        slow->next = back;
        back = slow;
        slow = next;
    }
    for (; back; back = back->next, head = head->next)
        if (back->val != head->val)
            return false;
    return true;
}
