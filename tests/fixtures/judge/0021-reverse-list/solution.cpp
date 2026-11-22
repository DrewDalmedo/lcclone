// Recursive: 100000 calls deep on the full-size case.
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode* rest = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return rest;
    }
};
