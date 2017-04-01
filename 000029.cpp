// ConsoleApplication5.cpp: определяет точку входа для консольного приложения.
//
//#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define MIN_SIZE 16
#define MEMORY_STEP 2
typedef long sizeT;

//template <typename T>
// struct Train
// {
// 	T in;
// 	T out;
// 	Train() {}
// 	Train(const T& in, const T& out) { this->in = in; this->out = out;}
// 	Train& operator = (const T& t) {this->in = t.in; this->out = t.out; return *this;}
// };

struct Train
{
	long in;
	long out;
};

template <typename T>
class IComporator
{
	virtual const int Compare(const T& a, const T& b) const = 0;
};

template <typename T>
class DefaultComporator: public IComporator<T>
{
	const int Compare(const T& a, const T& b) const {return a < b;}
};



// int comparatorDown(const void* arg1, const void* arg2)
// {
// 	return (*(int*)arg1) > (*(int*)arg2);
// }

// int defaultComparator(const void* arg1, const void *arg2)
// {
// 	return (*(int*)arg1) < (*(int*)arg2);
// }

template <typename T = Train>
class TrainComporator: public IComporator<T>
{
	const int Compare(const T& a, const T& b) const { return a.out > b.out;}
};

// template <typename T>
// int DefaultComparator(const T &arg1, const T &arg2)
// {
// 	return arg1 < arg2;
// }

// int comparator(const void * arg1, const void *arg2)
// {
// 	return ((Train*)arg1)->out > ((Train*)arg2)->out;//((Man*)arg1)->w < ((Man*)arg2)->w;
// }


//////////////////////////////////////////////////////////////////////////
// Exception 
class Exception
{
	char message[1024];
public:
	Exception() {}
	Exception(const char *message, const char* file, const char* func, int line);
	const char* GetMesssage() const;
};

// Exception Realisation
Exception::Exception(const char *message, const char* file, const char* func, int line)
{
	sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
}

const char* Exception::GetMesssage() const
{
	return message;
}

//#define ThrowException(msg)	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__)
#define ThrowException(msg) throw Exception(msg, __FILE__, __FUNCTION__, __LINE__); 
/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// LightArray (my vector)
template <typename T>
class LightArray
{
	T* ptr;
	sizeT allocatedSize;
	sizeT elementsInBuffer;
public:
	LightArray();
	LightArray(sizeT allocatedSize);
	LightArray(const LightArray& arr);
	~LightArray();
	void PushBack(const T& data);
	void Resize();
	sizeT Count() const;
	T* GetPointer();
	void Sort(int comparator(const void *, const void *), sizeT b, sizeT e);
	void Swap(T *a, T* b) { T temp = *a; *a = *b, *b = temp; }
	const T& operator[] (sizeT i) const { return ptr[i]; }
	T& operator[] (sizeT i) { return ptr[i]; }
	LightArray& operator = (const LightArray& arr);
};

template <typename T>
LightArray<T>::LightArray()
{
	allocatedSize = 2;
	ptr = new T[allocatedSize];
	if (!ptr) ThrowException("Cant allocate memory");
	elementsInBuffer = 0;
}

template <typename T>
LightArray<T>::LightArray(const LightArray<T>& arr)
{
	ptr = NULL;
	*this = arr;
}

template <typename T>
LightArray<T>::LightArray(sizeT sizeToAlloc)
{
	if (sizeToAlloc < MIN_SIZE) sizeToAlloc = MIN_SIZE;
	allocatedSize = sizeToAlloc;
	ptr = new T[allocatedSize];

	if (!ptr)
		ThrowException("cant allocate");
	elementsInBuffer = 0;
}

template <typename T>
LightArray<T>::~LightArray()
{
	delete[] ptr;
}

template <typename T>
void LightArray<T>::PushBack(const T& data)
{
	if (elementsInBuffer == allocatedSize)
		Resize();
	ptr[elementsInBuffer++] = data;
}

template <typename T>
void LightArray<T>::Resize()
{
	if (allocatedSize == 0) allocatedSize = 1;
	sizeT newSize = allocatedSize * MEMORY_STEP;
	T *oldPtr = ptr;
	T *newPtr = new T[newSize];
	if (!newPtr) ThrowException("Cant reallcate");
	memcpy(newPtr, oldPtr, allocatedSize * sizeof(T));
	delete[] oldPtr;
	ptr = newPtr;
	allocatedSize *= MEMORY_STEP;
}

template <typename T>
sizeT LightArray<T>::Count() const
{
	return elementsInBuffer;
}

template <typename T>
T *LightArray<T>::GetPointer()
{
	return ptr;
}

template <typename T>
LightArray<T>& LightArray<T>::operator = (const LightArray<T>& arr)
{
	delete ptr;
	allocatedSize = arr.allocatedSize;
	elementsInBuffer = arr.elementsInBuffer;
	ptr = new T[allocatedSize];
	if (!ptr) ThrowException("Cant alloc");
	memcpy(ptr, arr.ptr, elementsInBuffer * sizeof(T));
	return *this;
}

template <typename T>
void LightArray<T>::Sort(int comparator(const void*, const void*), sizeT b, sizeT e)
{
	int l = b, r = e;
	T piv = ptr[(l + r) / 2];
	while (l <= r)
	{
		while (comparator(&ptr[l], &piv))
			l++;
		while (comparator(&piv, &ptr[r]))
			r--;
		if (l <= r)
		{
			Swap(&ptr[l], &ptr[r]);
			l++, r--;
		}
	}

	if (b < r)
		Sort(comparator, b, r);
	if (e > l)
		Sort(comparator, l, e);
}
///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////DefaultComporator
// Heap (based on LightArray)
template <typename T, template<typename > class Comparator = DefaultComporator > 
class Heap
{
	sizeT elementsInHeap;
	LightArray<T> *arr;
	IComporator<T> *comparator;
	int(*comparator)(const T&, const T&);	// Pointer to function which depends on type data
private:
	void Swap(T* a, T* b) { arr->Swap(a, b); }//T temp = *a; *a = *b, *b = temp; }
	void SiftUp(sizeT pos);
	void SiftDown(sizeT pos);
	T operator[](sizeT i) { return (*arr)[i]; }
public:
	Heap() {comparator = IsLess;}
	Heap(sizeT cnt, int (*comparator)(const T &, const T&) = IsLess);
	~Heap();
	void Init(const LightArray<T> &arr, int(*funcComparator)(const T&, const T&) = DefaultComparator);
	void SetComparator(int (funcComparator)(const T&, const T&)) { comparator = funcComparator; }
	void Print();
	const sizeT CountInHeap() const { return elementsInHeap; }
	void Add(const T& element);
	void MakeHeap();
	T& ExtractExtremum();
	const T& Extremum() const;
	//void HeapSort();
};


template <typename T, template<typename T> class Comparator = DefaultComparator>
Heap<T>::Heap()
{
	arr = new LightArray<T>();
	if (!arr) ThrowException("Cant alloc arr");
	elementsInHeap = 0;
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
void Heap<T>::Add(const T& element)
{
	int count = elementsInHeap;
	elementsInHeap++;
	if (elementsInHeap > arr->Count())
		arr->Resize();
	arr->AtIndex(count) = element;
	if (count > 0)
		SiftUp(count);
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
void Heap<T>::Init(const LightArray<T> &arr, int(*funcComparator)(const T&, const T&))
{
	elementsInHeap = arr.Count();
	*this->arr = arr;
	comparator = funcComparator;
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
void Heap<T>::SiftUp(sizeT pos)
{
	sizeT i = pos;
	while (comparator(arr->AtIndex((i - 1) / 2), arr->AtIndex(i)))
	{
		Swap(arr->AtIndex(i), arr->AtIndex((i - 1) / 2));
		i = (i - 1) / 2;
	}
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
Heap<T>::Heap(sizeT cnt)
{
	arr = new LightArray<T>(cnt);
	if (!arr) ThrowException("Cant alloc");
	elementsInHeap = 0;
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
Heap<T>::~Heap()
{
	delete arr;
}



// Todo not class
// template <typename T>
// void Heap<T>::HeapSort()
// {
// 	MakeHeap();
// 	while (elementsInHeap > 0)
// 	{
// 		T extr = ExtractExtremum();
// 		arr->AtIndex(elementsInHeap) = extr;
// 	}
//}

template <typename T, template<typename T> class Comparator = DefaultComparator>
void Heap<T>::Print()
{
	for (sizeT i = 0; i < arr->Count(); i++)
		arr->AtIndex(i).Print();	//printf("%d ", arr[i]);
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
void Heap<T>::MakeHeap()
{
	for (sizeT i = elementsInHeap / 2 - 1; i >= 0; i--)
		SiftDown(i);
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
void Heap<T>::SiftDown(sizeT pos)
{
	sizeT largest;
	bool flag = 1;
	sizeT count = elementsInHeap;//arr->Count();
	for (sizeT i = pos; 2 * i + 1 < count && flag;) // while nodes not empty
	{
		largest = i;
		sizeT left = 2 * i + 1;
		if (left < count && comparator(&(*arr)[largest], &(*arr)[left]))
			largest = left;

		sizeT right = 2 * i + 2;
		if (right < count && comparator(&(*arr)[largest], &(*arr)[right]))
			largest = right;

		if (largest != i)
		{
			Swap(&(*arr)[i], &((*arr)[largest]));
			i = largest;
		}
		else flag = false;
	}
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
T& Heap<T>::Extremum() const
{
	if (elementsInHeap == 0) ThrowException("No elements in heap");
	return (*arr)[0];
}

template <typename T, template<typename T> class Comparator = DefaultComparator>
T& Heap<T>::ExtractExtremum()
{
	if (elementsInHeap == 0) ThrowException("No elements in heap");
	T extremum = (*arr)[0];
	(*arr)[0] = (*arr)[elementsInHeap - 1];
	elementsInHeap--;
	SiftDown(0);
	return extremum;
}

LightArray<Train> ReadTrains()
{
	int n;
	scanf("%d", &n);
	LightArray<Train> arr(n);

	for (int i = 0; i < n; i++)
	{
		int in, out;
		scanf("%d %d", &in, &out);
		arr.PushBack({ in, out });
	}
	return arr;
}

sizeT GetMinEnd(LightArray<Train> &trainsArr)//Heap<Train> &trains)
{
	Heap<Train> trainsHeap(trainsArr.Count());
	trainsHeap.SetComparator(comparator);
	sizeT count = trainsArr.Count();
	trainsHeap.Add(trainsArr[0]);
	sizeT max = 1;

	for (sizeT i = 1; i < count; i++)
	{
		while (trainsHeap.CountInHeap() && trainsHeap.Extremum().out < trainsArr[i].in)
			trainsHeap.ExtractExtremum();
		
		trainsHeap.Add(trainsArr[i]);
		if (trainsHeap.CountInHeap() > max)
			max = trainsHeap.CountInHeap();
	}
	if (trainsHeap.CountInHeap() > max)
		max = trainsHeap.CountInHeap();
	return max;
}

int main()
{
	LightArray<Train> trains = ReadTrains();
	int count = GetMinEnd(trains);
	printf("%d", count);
	return 0;
}