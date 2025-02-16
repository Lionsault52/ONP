#include "WspanialyString.h"
#include <stdio.h>
#include <cmath>

void copyString(char*& destination, const char* source) {
    int length = 0;
    while (source[length] != '\0') {
        ++length;
    }
    destination = new char[length + 1];
    for (int i = 0; i < length; ++i) {
        destination[i] = source[i];
    }
    destination[length] = '\0';
}

WspanialyString::WspanialyString(const char* text) 
{
    int temp_length = 0;
    while (text[temp_length++] != '\0');
    this->length = temp_length - 1;
    this->text = new char[temp_length];
    for (int i = 0; i < temp_length; i++) 
    { 
        this->text[i] = text[i]; // zmieniamy warunek na < temp_length aby skopiowaæ znak '\0'
    }
}

WspanialyString::~WspanialyString() 
{
    delete[] this->text;
}

int WspanialyString::getLength() 
{
    return this->length;
}

void WspanialyString::printStr() 
{
    for (int i = 0; i < this->length; i++) printf("%c", this->text[i]);
    printf("\n");
}

void WspanialyString::printStrLine() 
{
    for (int i = 0; i < this->length; i++) printf("%c", this->text[i]);
}

WspanialyString& WspanialyString::operator=(const WspanialyString& other) {
    if (this != &other) 
    {
        delete[] this->text;
        this->length = other.length;
        this->text = new char[length + 1];
        for (int i = 0; i < this->length; i++) this->text[i] = other.text[i];
        this->text[this->length] = '\0';
    }
    return *this;
}

WspanialyString& WspanialyString::operator=(const char* newText)
{
    delete[] text;
    length = 0;
    while (newText[length] != '\0') {
        ++length;
    }
    copyString(text, newText);
    return *this;
}

WspanialyString& WspanialyString::operator+=(const WspanialyString& other) 
{
    int prevLength = this->length;
    char* prevText = new char[prevLength + 1];
    for (int i = 0; i < prevLength; i++)  prevText[i] = this->text[i];
    for (int i = 0; i < other.length; i++) prevText[prevLength + i] = other.text[i];
    delete[] this->text;
    this->length += other.length;
    this->text = prevText;
    return *this;
}

WspanialyString& WspanialyString::operator+=(const char* text)
{
    int textLength = 0;
    while (text[textLength++] != '\0');
    textLength--;
    int prevLength = this->length;
    char* newText = new char[prevLength + textLength + 1];
    for (int i = 0; i < prevLength; i++)  newText[i] = this->text[i];
    for (int i = 0; i < textLength; i++)  newText[prevLength + i] = text[i];
    newText[prevLength + textLength] = '\0';
    delete[] this->text;
    this->text = newText;
    this->length += textLength;
    return *this;
}


void WspanialyString::appendChar(char character) 
{
    char* new_text = new char[this->length + 2];
    for (int i = 0; i < this->length; i++) new_text[i] = this->text[i];
    new_text[this->length] = character;
    new_text[this->length + 1] = '\0';
    delete[] this->text;
    this->text = new_text;
    this->length++;
}

void WspanialyString::appendText(char* text)
{
	int textLength = 0;
	while (text[textLength++] != '\0');
	textLength--;
	char* new_text = new char[this->length + textLength + 1];
    for (int i = 0; i < this->length; i++) new_text[i] = this->text[i];
    for (int i = 0; i < textLength; i++) new_text[this->length + i] = text[i];
	new_text[this->length + textLength] = '\0';
	delete[] this->text;
	this->text = new_text;
	this->length += textLength;
}

char WspanialyString::getChar(int index) 
{
    if (index >= 0 && index < this->length) return this->text[index];
    else if (index < 0 && abs(index) <= this->length) return this->text[this->length + index];
    return '\0';
}


char* WspanialyString::getText() 
{
    return this->text;
}

void WspanialyString::setText(const char* text)
{
    int textLength = 0;
    while (text[textLength++] != '\0');
    textLength--;
    delete[] this->text;
    this->length = textLength;
    this->text = new char[this->length + 1];
    for (int i = 0; i < this->length; i++) this->text[i] = text[i];
    this->text[this->length] = '\0';
}

void WspanialyString::deleteChar(int index)
{
    if (index >= 0 && index < this->length)
    {
		char* new_text = new char[this->length];
        for (int i = 0; i < index; i++)
        {
			new_text[i] = this->text[i];
		}
        for (int i = index + 1; i < this->length; i++)
        {
			new_text[i - 1] = this->text[i];
		}
		new_text[this->length - 1] = '\0';
		delete[] this->text;
		this->text = new_text;
		this->length--;
	}
}   

bool WspanialyString::operator==(const WspanialyString& other) 
{
	if (this->length != other.length) return false;
    for (int i = 0; i < this->length; i++) 
    {
		if (this->text[i] != other.text[i]) return false;
	}
	return true;
}

bool WspanialyString::operator==(const char* text) 
{
	int textLength = 0;
    while (text[textLength++] != '\0'); 
    textLength--;
	if (this->length != textLength) return false;
    for (int i = 0; i < this->length; i++) 
    {
		if (this->text[i] != text[i]) return false;
	}
	return true;
}