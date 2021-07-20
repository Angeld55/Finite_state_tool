#include <iostream>
#include <set>
#ifndef SET_HDR
#define SET_HDR

template<typename T>
class CustomSet 
{

private:
	std::set<T> elements;

public:
	CustomSet() = default;

	size_t getSize() const;
	bool add(const T& elem);
	bool contains(const T& elem) const;

	bool equals(const CustomSet<T>& other);

	bool operator<(const CustomSet<T>& other) const
	{
		return elements < other.elements;
	}

	const std::set<T>& getRaw() const;
};

template<typename T>
CustomSet<T> Union(const CustomSet<T>& set1, const CustomSet<T>& set2);

template<typename T>
CustomSet<T> Intersection(const CustomSet<T>& set1, const CustomSet<T>& set2);

template<typename T>
CustomSet<T> SetDifference(const CustomSet<T>& set1, const CustomSet<T>& set2);




template<typename T>
size_t CustomSet<T>::getSize() const
{
	return elements.size();
}


template<typename T>
bool CustomSet<T>::add(const T& elem)
{
	auto it = elements.insert(elem);
	return it.second;
}

template<typename T>
bool CustomSet<T>::contains(const T& elem) const 
{

	auto it = elements.find(elem);

	return !(it == elements.end());
}

template<typename T>
CustomSet<T> Union(const CustomSet<T>& set1, const CustomSet<T>& set2) {

	CustomSet<T> new_set;

	for (auto it = set1.getRaw().begin(); it != set1.getRaw().end(); it++)
		new_set.add(*it);
	
	for (auto it = set2.getRaw().begin(); it != set2.getRaw().end(); it++)
		new_set.add(*it);
	
	return new_set;
}

template<typename T>
CustomSet<T> Intersection(const CustomSet<T>& set1, const CustomSet<T>& set2) {

	CustomSet<T> new_set;

	for (auto it = set1.getRaw().begin(); it != set1.getRaw().end(); it++)
	{
		if (set2.getRaw().find(*it) != set2.getRaw().end())
			new_set.add(*it);
	}
	return new_set;
}

template<typename T>
CustomSet<T> SetDifference(const CustomSet<T>& set1, const CustomSet<T>& set2) {

	CustomSet<T> new_set;

	for (auto it = set1.getRaw().begin(); it != set1.getRaw().end(); it++)
	{
		if (set2.getRaw().find(*it) == set2.getRaw().end())
			new_set.add(*it);
	}
	return new_set;
}

template<typename T>
bool CustomSet<T>::equals(const CustomSet<T>& other) {


	for (auto it = set1.elements.begin(); it != set1.elements.end(); it++)
	{
		if (set2.elements.find(*it) != set2.elements.end())
			return false;
	}

	for (auto it = set2.elements.begin(); it != set2.elements.end(); it++)
	{
		if (set1.elements.find(*it) != set1.elements.end())
			return false;
	}
	return true;
}

template<typename T>
const std::set<T>& CustomSet<T>::getRaw() const
{
	return elements;
}
#endif // !SET_HDR
