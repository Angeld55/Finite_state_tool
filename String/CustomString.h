#ifndef STRING_HDR
#define STRING_HDR

#include<iostream>
#include<fstream>

const int MAX_SIZE = 1024;

class CustomString {

private:
	char* str;
	int len;

	void CopyFrom(const CustomString& other);
	void Free();

public:

	CustomString();
	CustomString(char ch);
	CustomString(const char* str);
	CustomString(const CustomString& other);
	CustomString& operator=(const CustomString& other);
	~CustomString();

	int getLenght() const;
	CustomString SubString(int start) const;
	CustomString SubString(int start, int end) const;
	int IndexOf(const CustomString& other, int index); //return the index of the first encounter of other in the object
	int IndexOf(const char* str, int index); //reusing IndexOf(const CustomString& other, int index)
	int CountOcurences(CustomString str); // How many times a string is found as substring
	int GetOcurence(CustomString str,int i); //Get if the i-th substring (count start from 1)

	const char& operator[](int index) const;
	char& operator[](int index);
	CustomString& operator+=(const CustomString& other); //return the concatenation of the object with other
	friend bool operator==(const CustomString& lhs, const CustomString& rhs); //return true if two strings are equivalent
	friend bool operator!=(const CustomString& lhs, const CustomString& rhs); //return true if two strings are not equivalent 
	friend std::ostream& operator<<(std::ostream& os, const CustomString& obj);
	friend std::istream& operator>>(std::istream& is, CustomString& obj);

	CustomString* Split(CustomString str);
	const char* c_str();
};
const CustomString operator+(const CustomString& lhs, const CustomString& rhs);



#endif // !STRING_HDR
