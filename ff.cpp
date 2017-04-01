// ConsoleApplication3.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define MIN_SIZE 16
#define MEMORY_STEP 2
//#define TEST
typedef long sizeT;


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
    T* AtIndex(sizeT i) {return &ptr[i];}
//    const T& AtIndex(sizeT i) { return ptr[i]; }
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
    //if (!allocatedSize) allocatedSize = 1;
    sizeT newSize = allocatedSize * MEMORY_STEP;
    T *oldPtr = ptr;
    T *newPtr = new T[newSize];//(T*)realloc(oldPtr, newSize * sizeof(T));
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

template <typename T>
void CountingSort(char **strigns, int n, int* c, size_t sizeC, int r)
{
    memset(c, 0, sizeC * sizeof(c*));
    for (int i = 0; i < n; i++)
        c[strings[i][r]]++;


}//const LightArray<T> &arr, )

void MSDSort(char **strings, int n, int r)
{
    if (n  <= 1) return;
    int *arr = new int[256];
    CountingSort(strings, n, arr, r);

    for (int i = 0; i < r; i++)
}

int main()
{
//    LightArray<int> a;
//    for (int i = 0; i < 10; i++)
//        a.PushBack(rand() % 1000);
//    for (int i = 0; i < a.Count(); i++)
//        printf("%d ", a[i]);
//    printf("\n");

//    Heap<int> h;
//    h.Init(a, comparatorUp);
//    h.HeapSort();

//    LightArray<int> *arr = &h.Array();
//    //arr->Sort(comparatorUp, 0, arr->Count() -1);
//    for (int i = 0; i < arr->Count(); i++)
//        printf("%d ", *(arr->AtIndex(i)));


    Heap<Event> eventLine;
    ReadEvents(eventLine);
    sizeT max = GetMax(eventLine);
    printf("%u", max);
    return 0;
    //	getchar();
}
