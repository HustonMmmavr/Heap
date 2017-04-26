// ConsoleApplication5.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//
//#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <utility>
#include <stdlib.h>
#include <string.h>
#define MIN_SIZE 16
#define MEMORY_STEP 2
typedef long sizeT;

struct Train
{
    long in;
    long out;
};

template <typename T>
class IComparator
{
public:
    virtual int Compare(const T& a, const T& b) const = 0;
    virtual ~IComparator() {}
};

template <typename T>
class DefaultComparator: public IComparator<T>
{
public:
    int Compare(const T& a, const T& b) const {return a < b;}
    ~DefaultComparator(){}
};

template <typename T = Train>
class TrainComparator: public IComparator<T>
{
public:
    int Compare(const T& a, const T& b) const { return a.out > b.out;}
    ~TrainComparator(){}
};


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
    sizeT Count() const {return elementsInBuffer;}
    sizeT Allocated() const {return allocatedSize;}
    T* GetPointer();
    void Sort(int comparator(const void *, const void *), sizeT b, sizeT e);
    void Swap(T *a, T* b) { T temp = *a; *a = *b, *b = temp; }
    const T& operator[] (sizeT i) const { return ptr[i]; }
    T& operator[] (sizeT i) { return ptr[i]; }
    LightArray& operator = (const LightArray& arr);
    const T& AtIndex(sizeT i) const { return ptr[i]; }
    T& AtIndex(sizeT i) {return ptr[i];}

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

// template <typename T>
// sizeT LightArray<T>::Count() const
// {
// 	return elementsInBuffer;
// }

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
template <typename T, template<typename > class Comparator = DefaultComparator >
class Heap
{
    sizeT elementsInHeap;
    LightArray<T> *arr;
    IComparator<T> *comparator;
private:
    void Swap(T& a, T& b) { arr->Swap(&a, &b); }
    void SiftUp(sizeT pos);
    void SiftDown(sizeT pos);
    T operator[](sizeT i) { return (*arr)[i]; }
public:
    Heap();
    Heap(sizeT cnt, Comparator<T> *comparator = NULL);
    ~Heap();
    void Init(const LightArray<T> &arr, Comparator<T> *comparator = NULL);
    void SetComparator(Comparator<T> *comparator){ this->comparator = comparator;}
    void Print();
    sizeT CountInHeap() const { return elementsInHeap; }
    void Add(const T& element);
    void MakeHeap();
    LightArray<T>& Array() const {return *arr;}
    T ExtractExtremum();
    const T& Extremum() const;
};


template <typename T, template<typename T> class Comparator>
Heap<T, Comparator>::Heap()
{
    comparator = NULL;
    arr = new LightArray<T>();
    if (!arr) ThrowException("Cant alloc arr");
    comparator = new Comparator<T>();
    if(!comparator) ThrowException("Cant alloc");
    elementsInHeap = 0;
}

template <typename T, template<typename T> class Comparator>
void Heap<T, Comparator>::Add(const T& element)
{
    int count = elementsInHeap;
    elementsInHeap++;
    if (elementsInHeap > arr->Allocated())
        arr->Resize();
    arr->AtIndex(count) = element;
    if (count > 0)
        SiftUp(count);
}

template <typename T, template<typename T> class Comparator>
void Heap<T, Comparator>::Init(const LightArray<T> &arr, Comparator<T> *comparator)
{
    if (comparator == NULL)
    {
        delete this->comparator;
        this->comparator = new Comparator<T>();
    }
    elementsInHeap = arr.Count();
    *this->arr = arr;
}

template <typename T, template<typename T> class Comparator>
void Heap<T, Comparator>::SiftUp(sizeT pos)
{
    sizeT i = pos;
    while (comparator->Compare(arr->AtIndex((i - 1) / 2), arr->AtIndex(i)))
    {
        Swap(arr->AtIndex(i), arr->AtIndex((i - 1) / 2));
        i = (i - 1) / 2;
    }
}

template <typename T, template<typename T> class Comparator >
Heap<T, Comparator>::Heap(sizeT cnt, Comparator<T> *comparator)
{
    if (comparator == NULL) this->comparator = new Comparator<T>();
    arr = new LightArray<T>(cnt);
    if (!arr) ThrowException("Cant alloc");
    elementsInHeap = 0;
}

template <typename T, template<typename T> class Comparator>
Heap<T, Comparator>::~Heap()
{
    delete arr;
    delete comparator;
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

template <typename T, template<typename T> class Comparator>
void Heap<T, Comparator>::Print()
{
    for (sizeT i = 0; i < arr->Count(); i++)
        arr->AtIndex(i).Print();	//printf("%d ", arr[i]);
}

template <typename T, template<typename T> class Comparator>
void Heap<T, Comparator>::MakeHeap()
{
    for (sizeT i = elementsInHeap / 2 - 1; i >= 0; i--)
        SiftDown(i);
}

template <typename T, template<typename T> class Comparator>
void Heap<T, Comparator>::SiftDown(sizeT pos)
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

template <typename T, template<typename T> class Comparator>
const T& Heap<T, Comparator>::Extremum() const
{
    if (elementsInHeap == 0) ThrowException("No elements in heap");
    return (*arr)[0];
}

template <typename T, template<typename T> class Comparator>
T Heap<T, Comparator>::ExtractExtremum()
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
    Heap<Train, TrainComparator> trainsHeap(trainsArr.Count());
//	trainsHeap.SetComparator(comparator);
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
//    Heap<int> h;
//    LightArray<int> b;
//    for (int i = 0; i < 18; i++)
//    {
//        int rnd = rand() % 1000;
//        b.PushBack(rand() % 1000);
//        printf("%d ", rnd);
//    }
//    printf("\n");
//    //h.Array() = b;
//    //h.MakeHeap();
//    h.Init(b);
//    h.MakeHeap();
//    LightArray<int> *a = &h.Array();
//    for (int i = 0; i < 18; i++)
//        printf("%d ", a->AtIndex(i));
    int count = GetMinEnd(trains);
    printf("%d", count);
    return 0;
}
