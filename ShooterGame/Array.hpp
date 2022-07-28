#pragma once

template<typename T>
class Array {

public:

	Array<T>(int length);
	Array<T>(T* data, int length);
	//Array<T>(T o1);
	Array<T>(T o1, T o2);
	Array<T>(T o1, T o2, T o3);
	Array<T>(T o1, T o2, T o3, T o4);

	T& operator[](int i);
	T& at(int i);

	T* getData();
	int size();

private:

	T* array;
	int length;

};

template<typename T>
Array<T>::Array(int length) {
	array = new T[length];
	this->length = length;
}

template<typename T>
Array<T>::Array(T* data, int length) {
	array = data;
	this->length = length;
}

//template<typename T>
//Array<T>::Array(T o1) {
//	this->length = 1;
//	array = new T[length];
//	array[0] = o1;
//}

template<typename T>
Array<T>::Array(T o1, T o2) {
	this->length = 2;
	array = new T[length];
	array[0] = o1;
	array[1] = o2;
}

template<typename T>
Array<T>::Array(T o1, T o2, T o3) {
	this->length = 3;
	array = new T[length];
	array[0] = o1;
	array[1] = o2;
	array[2] = o3;
}

template<typename T>
Array<T>::Array(T o1, T o2, T o3, T o4) {
	this->length = 4;
	array = new T[length];
	array[0] = o1;
	array[1] = o2;
	array[2] = o3;
	array[4] = o4;
}

template<typename T>
T& Array<T>::operator[](int i) {
	return array[i];
}

template<typename T>
T& Array<T>::at(int i) {
	return array[i];
}

template<typename T>
T* Array<T>::getData() {
	return array;
}

template<typename T>
int Array<T>::size() {
	return length;
}
