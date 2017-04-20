#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
using namespace std;
#define MIN_SIZE 16
#define MEMORY_STEP 2
typedef long sizeT;

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
///////////////////////////////////////////////////////////////////////


template <typename T>
void MedianOfThree(LightArray<T> &arr, int left, int right)
{
	int mid = (right + left) / 2;
	if (arr[left] > arr[right]) swap(arr[right], arr[left]);
	if (arr[left] > arr[mid]) swap(arr[mid], arr[left]);
	if (arr[mid] < arr[right]) swap(arr[mid], arr[right]);
}

template <typename T>
int Partition(LightArray<T> &arr, int left, int right)
{
	MedianOfThree(arr, left, right);
	T piv = arr[right];// - 1];
	int i = 0;

	while (i <= right && arr[i] <= piv) i++; // search group > piv
	if (i >= right) return right;// -1;
	int j = i + 1 ;

	while (j < right)
	{
		if (arr[j] > piv) j++;
		else
		{
			swap(arr[j], arr[i]);
			j++;
			i++;
		}
	}
	swap(arr[i], arr[right]);
	return i;
}

 void InsertionSort(LightArray<int> &arr, int left, int right)//SSsize_t size, int (*funcComparator)(const void*, const void*))
{
    for (size_t i = 1; i < size; i++)
    {
        T temp = arr[i];
        size_t j;
        for (j = i; j > 0 && temp < arr[j-1]; j--)//funcComparator(&temp, &arr[j - 1]); j--)
            arr[j] = arr[j-1];
        arr[j] = temp;
    }
}


void QuickSort(LightArray<int> &arr)
{
	stack<int> idx;
	idx.
}