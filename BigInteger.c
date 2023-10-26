#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInteger.h"

struct node {
    char data;
    struct node* next;
};

struct BigInteger create(char* s) {
    struct BigInteger num;
    num.l = NULL;
    num.length = 0;
    num.sign = 1; // Initialize as positive
    if (*s == '-') {
        num.sign = -1; // Set sign to negative
        s++;
    }

    while (*s != '\0') {
        if (*s >= '0' && *s <= '9') {
            num = pushDigit(num, *s);
        }
        s++;
    }

    return num;
}

void display(struct BigInteger num) {
    if (num.sign == -1) {
        printf("-");
    }

    struct node* current = num.l;
    while (current != NULL) {
        printf("%c", current->data);
        current = current->next;
    }
    printf("\n");
}

struct BigInteger pushDigit(struct BigInteger num, char digit) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->data = digit;
    newNode->next = num.l;
    num.l = newNode;
    num.length++;
    return num;
}

struct BigInteger add(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result;
    result.l = NULL;
    result.length = 0;
    result.sign = 1;

    struct node* p1 = a.l;
    struct node* p2 = b.l;

    int carry = 0;

    while (p1 != NULL || p2 != NULL || carry != 0) {
        int sum = carry;

        if (p1 != NULL) {
            sum += p1->data - '0';
            p1 = p1->next;
        }

        if (p2 != NULL) {
            sum += p2->data - '0';
            p2 = p2->next;
        }

        carry = sum / 10;
        sum %= 10;

        result = pushDigit(result, sum + '0');
    }

    return result;
}

struct BigInteger sub(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result;
    result.l = NULL;
    result.length = 0;
    result.sign = 1;

    struct node* p1 = a.l;
    struct node* p2 = b.l;

    int borrow = 0;

    while (p1 != NULL || p2 != NULL) {
        int diff = borrow;

        if (p1 != NULL) {
            diff += p1->data - '0';
            p1 = p1->next;
        }

        if (p2 != NULL) {
            diff -= p2->data - '0';
            p2 = p2->next;
        }

        if (diff < 0) {
            diff += 10;
            borrow = -1;
        } else {
            borrow = 0;
        }

        result = pushDigit(result, diff + '0');
    }

    // Remove leading zeros
    while (result.l != NULL && result.l->data == '0') {
        struct node* temp = result.l;
        result.l = result.l->next;
        free(temp);
    }

    // Set the sign of the result
    if (result.l == NULL) {
        result.sign = 1;  // Result is zero
    } else if (a.length > b.length || (a.length == b.length && a.l->data > b.l->data)) {
        result.sign = a.sign;
    } else {
        result.sign = b.sign;
    }

    return result;
}

struct BigInteger mul(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result;
    result.l = NULL;
    result.length = 0;
    if (a.sign == b.sign) {
        result.sign = 1;
    } else {
        result.sign = -1;
    }

    struct node* p1 = a.l;

    int zeros = 0;
    while (p1 != NULL) {
        int carry = 0;
        int digit1 = p1->data - '0';
        struct node* p2 = b.l;

        for (int i = 0; i < zeros; i++) {
            result = pushDigit(result, '0');
        }

        while (p2 != NULL) {
            int digit2 = p2->data - '0';
            int product = digit1 * digit2 + carry;

            if (result.l == NULL) {
                result = pushDigit(result, (product % 10) + '0');
            } else {
                struct node* newNode = (struct node*)malloc(sizeof(struct node));
                newNode->data = (product % 10) + '0';
                newNode->next = result.l;
                result.l = newNode;
            }

            carry = product / 10;
            p2 = p2->next;
        }

        while (carry > 0) {
            result = pushDigit(result, (carry % 10) + '0');
            carry /= 10;
        }

        zeros++;
        p1 = p1->next;
    }

    return result;
}

struct BigInteger div1(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result;
    result.l = NULL;
    result.length = 0;
    if (a.sign == b.sign) {
        result.sign = 1;
    } else {
        result.sign = -1;
    }

    struct BigInteger temp = create("0");
    struct BigInteger one = create("1");

    while (compare(a, temp) >= 0) {
        temp = add(temp, b);
        result = add(result, one);
    }

    // Adjust the result and set the sign
    result = sub(result, create("1"));
    if (a.sign != b.sign) {
        result.sign = -result.sign;
    }

    return result;
}

int compare(struct BigInteger a, struct BigInteger b) {
    // First, compare signs
    if (a.sign < b.sign) {
        return -1;
    } else if (a.sign > b.sign) {
        return 1;
    }

    // Signs are equal, compare lengths
    if (a.length > b.length) {
        return a.sign; // If a is positive, return 1; if negative, return -1.
    } else if (a.length < b.length) {
        return -a.sign; // If a is positive, return -1; if negative, return 1.
    } else {
        // Both have the same sign and length, compare individual digits
        struct node* p1 = a.l;
        struct node* p2 = b.l;

        while (p1 != NULL) {
            if (p1->data > p2->data) {
                return a.sign;
            } else if (p1->data < p2->data) {
                return -a.sign;
            }
            p1 = p1->next;
            p2 = p2->next;
        }

        // If all digits are equal, return 0 (equal)
        return 0;
    }
}