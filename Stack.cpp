#include <iostream>
#include "Stack.h"
#include "WspanialyString.h"

Node::Node(WspanialyString* data) 
{
    this->data = data;
    this->next = nullptr;
}

Node::~Node() 
{
    delete data;
}

WspanialyString* Node::getData() 
{
    return data;
}

Node* Node::getNext() 
{
    return next;
}

void Node::setNext(Node* next) 
{
    this->next = next;
}

Stack::Stack() 
{
    top = nullptr;
    size = 0;
}

Stack::~Stack() 
{
    while (top != nullptr) 
    {
        Node* temp = top;
        top = top->getNext();
        delete temp;
    }
}

void Stack::push(WspanialyString* data) 
{
    Node* temp = new Node(data);
    temp->setNext(top);
    top = temp;
    size++;
}

WspanialyString* Stack::pop() 
{
    if (isEmpty()) return nullptr;
    Node* temp = top;
    top = top->getNext();
    WspanialyString* data = temp->getData(); 
    delete temp; 
    size--;
    return data; 
}

void Stack::chuff() 
{
    if (!isEmpty()) pop();
}

WspanialyString* Stack::peek() {
    if (isEmpty()) return nullptr;
    return top->getData();
}

void Stack::printStack() {
    if (isEmpty()) 
    {
        printf("Stack is empty.\n");
        return;
    }
    Node* temp = top;
    int i = 0;
    while (temp != nullptr) 
    {
        printf("i: %i\t", i);
        temp->getData()->printStr();
        temp = temp->getNext();
        i++;
    }
}

bool Stack::isEmpty()
{
    return size == 0;
}

int Stack::stackLength() 
{
    return size;
}

void Stack::flipStack() 
{
    if (isEmpty()) return;
	Node* current = top;
	Node* previous = nullptr;
	Node* next = nullptr;
    while (current != nullptr) 
    {
		next = current->getNext();
		current->setNext(previous);
		previous = current;
		current = next;
	}
	top = previous;
}

WspanialyString* Stack::getIndex(int index) 
{
    if (isEmpty() || index >= size || index < 0) return nullptr;
	Node* current = top;
    for (int i = 0; i < index; i++) current = current->getNext();
	WspanialyString* data = current->getData();
	return data;
}

void Stack::deleteIndex(int index) 
{
    if (isEmpty() || index >= size || index < 0) return;
	Node* current = top;
	Node* previous = nullptr;
    for (int i = 0; i < index; i++) 
    {
		previous = current;
		current = current->getNext();
	}
    if (previous == nullptr) top = current->getNext();
    else previous->setNext(current->getNext());
	delete current;
	size--;
}


