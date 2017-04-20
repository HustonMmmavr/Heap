#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
using namespace std;
#define MIN_SIZE 16
#define MEMORY_STEP 2
typedef long sizeT;

//////////////////////////////////////////////////////////////////////////
// Exception
class Exception
{
	char message[1024];
public:
	Exception() {}
	Exception(const char *message, const char* file, const char* func, int line);
	const char* GetMessage() const;
};

// Exception Realisation
Exception::Exception(const char *message, const char* file, const char* func, int line)
{
	sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
}

const char* Exception::GetMessage() const
{
	return message;
}

#define ThrowException(msg) throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);
/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// LightArray (my vector)
typedef long sizeT;
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
    sizeT Count() const { return elementsInBuffer; }
    sizeT Allocated() const { return allocatedSize; }
    T* GetPointer();
    void Sort(int comparator(const void *, const void *), sizeT b, sizeT e);
    void Swap(T *a, T* b) { T temp = *a; *a = *b, *b = temp; }
    const T& operator[] (sizeT i) const { return ptr[i]; }
    T& operator[] (sizeT i) { return ptr[i]; }
    LightArray& operator = (const LightArray& arr);
    const T& AtIndex(sizeT i) const { return ptr[i]; }
    T& AtIndex(sizeT i) { return ptr[i]; }

};


template <typename T>
LightArray<T>::LightArray()
{
    allocatedSize = 0;
    ptr = NULL;
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
    allocatedSize = sizeToAlloc;
    if (allocatedSize == 0)
    {
        ptr = NULL;
    }
    else
    {
        ptr = new T[allocatedSize];

        if (!ptr)
            ThrowException("cant allocate");
    }
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
    if (allocatedSize == 0)
    {
        allocatedSize = 1;
        ptr = new T[allocatedSize];
        if (!ptr) ThrowException("err");
        return;
    }
    sizeT newSize = allocatedSize * MEMORY_STEP;
    T *oldPtr = ptr;
    T *newPtr = new T[newSize];
    if (!newPtr) ThrowException("Cant reallcate");
    for (int i = 0; i < allocatedSize; i++)
        newPtr[i] = oldPtr[i];
    delete[] oldPtr;
    ptr = newPtr;
    allocatedSize *= MEMORY_STEP;
}

template <typename T>
T *LightArray<T>::GetPointer()
{
    return ptr;
}

template <typename T>
LightArray<T>& LightArray<T>::operator = (const LightArray<T>& arr)
{
    delete[] ptr;
    allocatedSize = arr.allocatedSize;
    elementsInBuffer = arr.elementsInBuffer;
    if (allocatedSize == 0)
    {
        ptr = NULL;
        return *this;
    }
    ptr = new T[allocatedSize];
    if (!ptr) ThrowException("Cant alloc");
    for (int i = 0; i < elementsInBuffer; i++)
        ptr[i] = arr.ptr[i];
    //memcpy(ptr, arr.ptr, elementsInBuffer * sizeof(T));
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


template <typename T, bool less = false, template<typename, bool> class Comparator = DefaultComparator>
class Heap
{
	sizeT elementsInHeap;
	LightArray<T> *arr;
	IComparator<T, less> *comparator;
private:
	void Swap(T& a, T& b) { arr->Swap(&a, &b); }
	void SiftUp(sizeT pos);
	void SiftDown(sizeT pos);
public:
	Heap();
	Heap(sizeT cnt, Comparator<T, less> *comparator = NULL);
	~Heap();
	void Init(const LightArray<T> &arr);
	void Print();
	sizeT CountInHeap() const { return elementsInHeap; }
	void Add(const T& element);
	void MakeHeap();
	LightArray<T>& Array() const { return *arr; }
	T ExtractExtremum();
	const T& Extremum() const;
};

//template <typename T, template<typename T, bool less> class Comparator , bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

Heap<T, less, Comparator>::Heap()
{
	comparator = NULL;
	arr = new LightArray<T>();
	if (!arr) ThrowException("Cant alloc arr");
	comparator = new Comparator<T, less>();
	if (!comparator) ThrowException("Cant alloc");
	elementsInHeap = 0;
}

//template <typename T, template<typename T, bool less> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

void Heap<T, less,Comparator>::Add(const T& element)
{
	int count = elementsInHeap;
	elementsInHeap++;
	if (elementsInHeap > arr->Allocated())
		arr->Resize();
	arr->AtIndex(count) = element;
	if (count > 0)
		SiftUp(count);
}

//template <typename T, template<typename T, bool less> class Comparator, bool less
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::Init(const LightArray<T> &arr)
{
	elementsInHeap = arr.Count();
	*this->arr = arr;
}

//template <typename T, template<typename T, bool less> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::SiftUp(sizeT pos)
{
	sizeT i = pos;
	while (comparator->Compare(arr->AtIndex((i - 1) / 2), arr->AtIndex(i)))
	{
		Swap(arr->AtIndex(i), arr->AtIndex((i - 1) / 2));
		i = (i - 1) / 2;
	}
}

//template <typename T, template<typename T, bool less> class Comparator , bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
Heap<T, less, Comparator>::Heap(sizeT cnt, Comparator<T, less> *comparator)
{
	if (comparator == NULL) this->comparator = new Comparator<T,less>();
	arr = new LightArray<T>(cnt);
	if (!arr) ThrowException("Cant alloc");
	elementsInHeap = 0;
}

//template <typename T, template<typename T, bool > class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
Heap<T,less, Comparator>::~Heap()
{
	delete arr;
	delete comparator;
}

//template <typename T, template<typename T, bool > class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T,  less, Comparator>::Print()
{
	for (sizeT i = 0; i < arr->Count(); i++)
		arr->AtIndex(i).Print();	//printf("%d ", arr[i]);
}

//template <typename T, template<typename T, bool > class Comparator,bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::MakeHeap()
{
	for (sizeT i = elementsInHeap / 2 - 1; i >= 0; i--)
		SiftDown(i);
}

//template <typename T, template</*typename*/ T, bool > class Comparator,bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::SiftDown(sizeT pos)
{
	sizeT largest;
	bool flag = 1;
	sizeT count = elementsInHeap;//arr->Count();
	for (sizeT i = pos; 2 * i + 1 < count && flag;) // while nodes not empty
	{
		largest = i;
		sizeT left = 2 * i + 1;
		if (left < count && comparator->Compare(arr->AtIndex(largest), arr->AtIndex(left)))
			largest = left;

		sizeT right = 2 * i + 2;
		if (right < count && comparator->Compare(arr->AtIndex(largest), arr->AtIndex(right)))
			largest = right;

		if (largest != i)
		{
			Swap(arr->AtIndex(i), arr->AtIndex(largest));
			i = largest;
		}
		else flag = false;
	}
}

//template <typename T, template<typename T, bool> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
const T& Heap<T, less, Comparator>::Extremum() const
{
	if (elementsInHeap == 0) ThrowException("No elements in heap");
	return (*arr)[0];
}

//template <typename T, template<typename T, bool> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
T Heap<T, less, Comparator>::ExtractExtremum()
{
	if (elementsInHeap == 0) ThrowException("No elements in heap");
	T extremum = (*arr)[0];
	(*arr)[0] = (*arr)[elementsInHeap - 1];
	elementsInHeap--;
	SiftDown(0);
	return extremum;
}

LightArray<int>
