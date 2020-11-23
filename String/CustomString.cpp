#include "CustomString.h"

void CustomString::CopyFrom(const CustomString& other) {

	str = new char[strlen(other.str) + 1];
	strcpy(str, other.str);

	len = other.len;
}

void CustomString::Free() {
	delete[] str;
}

CustomString::CustomString() {

	str = new char[1];
	str[0] = '\0';
	len = 0;
}

CustomString::CustomString(char ch)
{
	str = new char[2];
	str[0] = ch;
	str[1] = '\0';
	len = 1;
}

CustomString::CustomString(const char* str) {

	len = strlen(str);

	this->str = new char[len + 1];
	strcpy(this->str, str);
}

CustomString::CustomString(const CustomString& other) {
	CopyFrom(other);
}

CustomString& CustomString::operator=(const CustomString& other) {

	if (this != &other) {
		Free();
		CopyFrom(other);
	}
	return *this;
}

CustomString::~CustomString() {
	Free();
}

int CustomString::getLenght() const {
	return len;
}

CustomString CustomString::SubString(int start) {

	if (start < 0 || start > len)
		throw std::logic_error("Invalid index.\n");

	char* newStr = new char[len - start + 2];
	int j = 0;
	for (int i = start; i < len; i++) 
		newStr[j++] = str[i];
	newStr[j] = '\0';
	
	CustomString newString(newStr);
	delete[] newStr;

	return newString;
}

CustomString CustomString::SubString(int start, int end) {

	if (start < 0 || start > len || end <= 0 || end > len)
		throw std::logic_error("Invalid index.\n");

	char* newStr = new char[end - start + 2];
	int j = 0;
	for (int i = start; i <= end; i++)
		newStr[j++] = str[i];
	newStr[j] = '\0';

	CustomString newString(newStr);
	delete[] newStr;

	return newString;
}

int CustomString::IndexOf(const CustomString& other, int index) {

	if(index < 0)
		throw std::logic_error("Invalid index.\n");

	int iter = 0;
	for (int i = index; i <= len; i++) {

		if (iter == other.len)
			return i - other.len;
		if (str[i] != other.str[iter])
			iter = 0;
		else
			iter++;
	}
	return -1;
}

int CustomString::IndexOf(const char* str, int index) {

	CustomString searched(str);
	return IndexOf(searched, index);
}

int CustomString::CountOcurences(CustomString str)
{	
	int count = 0;
	int i = IndexOf(str,0);
	while (i!=-1)
	{
		count++;
		i = IndexOf(str, i + 1);
	}
	return count;
}

inline int CustomString::GetOcurence(CustomString str, int num)
{
	int count = 0;
	int i = IndexOf(str, 0);
	while (i != -1)
	{
		count++;
		if (count == num)
			return i;
		i = IndexOf(str, i + 1);
	}
	return count;
}

const char& CustomString::operator[](int index) const {

	if(index >= len)
		throw std::out_of_range("Out of range!");
	return str[index];
}

char& CustomString::operator[](int index) {
	return str[index];
}

CustomString& CustomString::operator+=(const CustomString& other) {

	char* temp = str;
	str = new char[len + other.len + 1];
	strcpy(str, temp);
	strcat(str, other.str);
	len += other.len;
	delete[] temp;
	return *this;
}

bool operator==(const CustomString& lhs, const CustomString& rhs) {
	return !strcmp(lhs.str, rhs.str);
}

bool operator!=(const CustomString& lhs, const CustomString& rhs) {
	return strcmp(lhs.str, rhs.str);
}

std::ostream& operator<<(std::ostream& os, const CustomString& obj) {
	return os << obj.str;
}

std::istream& operator>>(std::istream& is, CustomString& obj) {

	char buff[MAX_SIZE];
	is.getline(buff, MAX_SIZE);
	obj.str = new char[strlen(buff) + 1];
	strcpy(obj.str, buff);

	return is;
}

const char* CustomString::c_str()
{
	return str;
}
const CustomString operator+(const CustomString& lhs, const CustomString& rhs) {
	if (lhs.getLenght() == 0)
		return rhs;
	else if (rhs.getLenght() == 0)
		return lhs;
	return CustomString(lhs) += rhs;
}
