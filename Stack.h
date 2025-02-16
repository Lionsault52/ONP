#pragma once
#include "WspanialyString.h"
class Node {
private:
	WspanialyString* data;
	Node* next;
public:
	Node(WspanialyString* data);
	~Node();
	WspanialyString* getData();
	Node* getNext();
	void setNext(Node* next);
};

class Stack {
private:
    Node* top;
    int size;

public:
    Stack();
    ~Stack();
    void push(WspanialyString* data);
    WspanialyString* pop();
    WspanialyString* peek();
    void printStack();
    bool isEmpty();
    int stackLength();
    void flipStack();
    WspanialyString* getIndex(int index);
    void deleteIndex(int index);
    void chuff();
};


