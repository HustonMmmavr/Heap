// ConsoleApplication12.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

//#include "stdafx.h"
//#includBe <st
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    //ptr = new T[allocatedSize];
    //if (!ptr) ThrowException("Cant allocate memory");
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
    //if (sizeToAlloc < MIN_SIZE) sizeToAlloc = MIN_SIZE;
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
///////////////////////////////////////////////////////////////////////

LightArray<int> ReadArray(int *k)
{
	int n;
	scanf("%d %d", &n, k);
	LightArray<int> arr(n);
	for (int i = 0; i < n; i++)
	{
		int b;
		scanf("%d", &b);
		arr.PushBack(b);
	}
	return arr;
}

void Merge(LightArray<int> &arr, int left, int mid, int right)
{
	int i1 = 0, i2 = 0;
	LightArray<int> addArr(right - left );

	while ((left + i1 < mid) && (mid + i2 < right))
	{
		if (arr[left + i1] < arr[mid + i2])
		{
			addArr[i1 + i2] = arr[left + i1];
			i1++;
		}
		else
		{
			addArr[i1 + i2] = arr[mid + i2];
			i2++;
		}
	}

	while (left + i1 < mid)
	{
		addArr[i1 + i2] = arr[left + i1];
		i1++;
	}

	while (mid + i2 < right)
	{
		addArr[i1 + i2] = arr[mid + i2];
		i2++;
	}

	for (int i = 0; i < i1 + i2; i++)
	{
		arr[left + i] = addArr[i];
	}
}

void MergeSort(LightArray<int> &arr, int left, int right)
{
	if (left + 1 >= right) return;
	int mid = (left + right) / 2;
	MergeSort(arr, left, mid);
	MergeSort(arr, mid, right);
	Merge(arr, left, mid, right);
}

void MergeSort(LightArray<int> &a)
{
    MergeSort(a, 0, a.Count());
}

void PartSort(LightArray<int> &arr, int k)
{
	int n = arr.Count();
    MergeSort(arr, 0, k);
	for (int i = 0; i < n; i += k)
	{
		//int left =  i == 0 ? 0 : i + 1;
		int right = i + k < n ? i + k : arr.Count();
        //for (int i = left; i < right; i++)
        //    printf("%d ", arr[i]);
        //printf("\n");
		MergeSort(arr, i, right);//i + k, arr.Count() - 1);
	}

}

int main()
{
	try
	{
		int k;
		LightArray<int> arr = ReadArray(&k);
		PartSort(arr, k);
		for (int i = 0; i < arr.Count(); i++)
			printf("%d ", arr.AtIndex(i));
		return 0;
	}
	catch (Exception &e)
	{
		printf("%s", e.GetMessage());
	}	
}
