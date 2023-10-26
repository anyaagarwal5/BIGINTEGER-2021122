#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

struct BigInteger {
    struct node* l;
    int sign;
    int length;
};

struct BigInteger create(char* s);
void display(struct BigInteger num);
struct BigInteger pushDigit(struct BigInteger num, char digit);
struct BigInteger add(struct BigInteger a, struct BigInteger b);
struct BigInteger sub(struct BigInteger a, struct BigInteger b);
struct BigInteger mul(struct BigInteger a, struct BigInteger b);
struct BigInteger div1(struct BigInteger a, struct BigInteger b);
struct BigInteger modu(struct BigInteger a, struct BigInteger b);
int compare(struct BigInteger a, struct BigInteger b);

#endif // BIG_INTEGER_H