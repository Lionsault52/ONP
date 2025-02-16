#pragma once
class WspanialyString {
	private:
		int length;
		char* text;

	public:
		WspanialyString(const char* text);
		~WspanialyString();
		int getLength();
		char getChar(int index);
		char* getText();
		void setText(const char* text);
		void printStr();
		void printStrLine();
		void appendChar(char character);
		void appendText(char* text);
		void deleteChar(int index);
		WspanialyString& operator=(const WspanialyString& other);
		WspanialyString& operator=(const char* text);
		WspanialyString& operator+=(const WspanialyString& other);
		WspanialyString& operator+=(const char* text);
		bool operator==(const WspanialyString& other);
		bool operator==(const char* text);
};

