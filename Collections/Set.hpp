#ifndef SET_HDR
#define SET_HDR

template<typename T>
class Set {

private:
	T * elements;
	size_t size;
	size_t capacity;

public:
	Set();
	~Set();
	Set(const Set<T>& other);
	Set<T>& operator=(const Set<T>& other);


private:
	void CopyFrom(const Set<T>& other);
	void Free();
	void Resize(size_t new_cap);

public:
	size_t getSize() const;
	T getElement(int pos) const;
	bool Add(T elem);
	bool Contains(T elem) const;
	void Print();
	T getAt(int ind) const;
	int IndexOf(const T& elem) const;

	bool Equals(const Set<T>& other);
	//friend bool operator==(const Set<T>& rhs, const Set<T>& lhs);


};

template<typename T>
Set<T> Union(const Set<T>& set1, const Set<T>& set2);

template<typename T>
Set<T> Intersection(const Set<T>& set1, const Set<T>& set2);

template<typename T>
Set<T> SetDifference(const Set<T>& set1, const Set<T>& set2);


//definitions
template<typename T>
Set<T>::Set() {

	capacity = 8;
	size = 0;
	elements = new T[capacity];
}

template<typename T>
Set<T>::~Set() {
	Free();
}

template<typename T>
Set<T>::Set(const Set<T>& other) {
	CopyFrom(other);
}

template<typename T>
Set<T>& Set<T>::operator=(const Set<T>& other) {

	if (this != &other) {

		Free();
		CopyFrom(other);
	}
	return *this;
}

template<typename T>
void Set<T>::CopyFrom(const Set<T>& other) {

	elements = new T[other.capacity];

	for (int i = 0; i < other.size; i++)
		elements[i] = other.elements[i];

	size = other.size;
	capacity = other.capacity;
}

template<typename T>
void Set<T>::Free() {

	delete[] elements;

	elements = nullptr;
	size = 0;
	capacity = 0;
}

template<typename T>
void Set<T>::Resize(size_t new_cap) {

	T* temp = elements;
	elements = new T[new_cap];
	for (int i = 0; i < size; i++)
		elements[i] = temp[i];

	capacity = new_cap;
	delete[] temp;
}

template<typename T>
size_t Set<T>::getSize() const {
	return size;
}

template<typename T>
T Set<T>::getElement(int pos) const {
	return elements[pos];
}

template<typename T>
bool Set<T>::Add(T elem) {

	if (!Contains(elem)) {
		if (size >= capacity) {
			size_t new_cap = capacity * 2;
			Resize(new_cap);
		}

		elements[size++] = elem;
		return true;
	}
	return false;
}

template<typename T>
bool Set<T>::Contains(T elem) const {

	for (int i = 0; i < size; i++) {
		if (elements[i] == elem)
			return true;
	}
	return false;
}

template<typename T>
void Set<T>::Print() {

	for (int i = 0; i < getSize(); i++)
		std::cout << elements[i] << " ";
	std::cout << std::endl;
}

template <typename T>
T Set<T>::getAt(int ind) const
{
	if (ind >= size)
		throw  std::length_error("Invalid index!");
	return elements[ind];
}

template<typename T>
Set<T> Union(const Set<T>& set1, const Set<T>& set2) {

	Set<T> new_set;

	for (int i = 0; i < set1.getSize(); i++)
		new_set.Add(set1.getElement(i));

	for (int i = 0; i < set2.getSize(); i++)
		new_set.Add(set2.getElement(i));

	return new_set;
}

template<typename T>
Set<T> Intersection(const Set<T>& set1, const Set<T>& set2) {

	Set<T> new_set;

	for (int i = 0; i < set1.getSize(); i++) {
		for (int j = 0; j < set2.getSize(); j++) {
			if (set1.getElement(i) == set2.getElement(j))
				new_set.Add(set1.getElement(i));
		}
	}
	return new_set;
}

template<typename T>
Set<T> SetDifference(const Set<T>& set1,const Set<T>& set2) {

	Set<T> new_set;
	bool isIn = false;

	for (int i = 0; i < set1.getSize(); i++) {
		for (int j = 0; j < set2.getSize(); j++) {
			if (set1.getElement(i) == set2.getElement(j)) 
				isIn = true;
		}

		if (!isIn) 
			new_set.Add(set1.getElement(i));
		
		isIn = false;
	}
	return new_set;
}

template<typename T>
bool Set<T>::Equals(const Set<T>& other) {

	if (getSize() != other.getSize())
		return  false;
	for (int i = 0; i < other.getSize(); ++i)
	{
		if (!Contains(other.getAt(i)))
			return false;
	}
	return true;
}

template<typename T>
int Set<T>::IndexOf(const T& elem) const
{
	for (int i = 0; i < size; i++) {
		if (elements[i] == elem)
			return i;
	}
	return -1;
}
#endif // !SET_HDR