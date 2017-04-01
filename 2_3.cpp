// ConsoleApplication5.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//
//#include "stdafx.h"
#include <stdio.h>
#include <math.h>
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


int comparatorDown(const void* arg1, const void* arg2)
{
    return (*(int*)arg1) > (*(int*)arg2);
}

int comparatorUp(const void* arg1, const void *arg2)
{
    return (*(int*)arg1) < (*(int*)arg2);
}

int comparator(const void * arg1, const void *arg2)
{
    return ((Train*)arg1)->out > ((Train*)arg2)->out;//((Man*)arg1)->w < ((Man*)arg2)->w;
}


int comparatorDWN(const void * arg1, const void *arg2)
{
    return ((Train*)arg1)->out < ((Train*)arg2)->out;//((Man*)arg1)->w < ((Man*)arg2)->w;
}


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
    sizeT Allocated() {return allocatedSize;}
    sizeT Count() const;
    sizeT& Count();
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
sizeT& LightArray<T>::Count()
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

////////////////////////////////////////////////////////////////////
// Heap (based on LightArray)
template <typename T>
class Heap
{
    sizeT elementsInHeap;
    LightArray<T> *arr;
    int(*comparator)(const void*, const void*);	// Pointer to function which depends on type data
private:
    void swap(T* a, T* b) { arr->Swap(a, b); }//T temp = *a; *a = *b, *b = temp; }
public:
    Heap();
    Heap(sizeT cnt);
    ~Heap();
    void Init(const LightArray<T> &arr, int(*funcComparator)(const void*, const void*) = comparatorDown);
    void SetComparator(int (funcComparator)(const void*, const void*)) { comparator = funcComparator; }
    void Print();
    T operator[](sizeT i) { return (*arr)[i]; }
    const sizeT Count() const { return arr->Count(); }
    const sizeT CountInHeap() const { return elementsInHeap; }
    void SiftUp(sizeT pos);
    void SiftDown(sizeT pos);
    //void Init(T* ptr, sizeT size, int(*funcComparator)(const void*, const void*) = comparatorDown, sizeT allcoated = MIN_SIZE);
    //void SiftDown(sizeT pos, int(*funcComparator)(const void*, const void*));
    void Add(const T& element);
    void MakeHeap();
    T ExtractExtremum();
    T& Extremum() const;
    void HeapSort();
};

template <typename T>
Heap<T>::Heap()
{
    elementsInHeap = 0;
    arr = new LightArray<T>();
}

template <typename T>
void Heap<T>::Add(const T& element)
{
    int count = elementsInHeap;
    elementsInHeap++;
    if (elementsInHeap > arr->Allocated())
        arr->Resize();
    arr->Count()++;
    (*arr)[count] = element;
    if (count > 0)
        SiftUp(count);
}

template <typename T>
void Heap<T>::Init(const LightArray<T> &arr, int(*funcComparator)(const void*, const void*))
{
    elementsInHeap = arr.Count();
    *this->arr = arr;
    comparator = funcComparator;
}

template <typename T>
void Heap<T>::SiftUp(sizeT pos)
{
    sizeT i = pos;
    while (comparator(&(*arr)[(i - 1) / 2], &(*arr)[i]))
    {
        swap(&(*arr)[i], &((*arr)[(i - 1) / 2]));
        i = (i - 1) / 2;
    }
}

template <typename T>
Heap<T>::Heap(sizeT cnt)
{
    arr = new LightArray<T>(cnt);
    elementsInHeap = 0;
    if (!arr) ThrowException("Cant alloc");
}

template <typename T>
Heap<T>::~Heap()
{
    delete arr;
}


template <typename T>
void Heap<T>::HeapSort()
{
    MakeHeap();
    while (elementsInHeap > 0)
    {
        T extr = ExtractExtremum();
        (*arr)[elementsInHeap] = extr;
    }
}

template <typename T>
void Heap<T>::Print()
{
    for (sizeT i = 0; i < arr->Count(); i++)
        // todo ovject.Print();
        printf("%d ", arr[i]);
}

template <typename T>
void Heap<T>::MakeHeap()
{
    for (sizeT i = elementsInHeap / 2 - 1; i >= 0; i--)
        SiftDown(i);
}

template <typename T>
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
            swap(&(*arr)[i], &((*arr)[largest]));
            i = largest;
        }
        else flag = false;
    }
}

template <typename T>
T& Heap<T>::Extremum() const
{
    return (*arr)[0];
}

template <typename T>
T Heap<T>::ExtractExtremum()
{
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
    //trainsHeap.SetArraySize(trainsArr.Count());
    //trainsHeap.Init(trainsArr, comparator);

    int count = trainsArr.Count();
    trainsHeap.Add(trainsArr[0]);
    int countEnd = 0;
    int max = 1;

    for (int i = 1; i < count; i++)
    {
        while (trainsHeap.CountInHeap() && trainsHeap.Extremum().out < trainsArr[i].in)
            trainsHeap.ExtractExtremum();

        trainsHeap.Add(trainsArr[i]);
        if (trainsHeap.CountInHeap() > max)
            max = trainsHeap.CountInHeap();
        //if
    }
    if (trainsHeap.CountInHeap() > max)
        max = trainsHeap.CountInHeap();
    //int countEnd = trainsHeap.CountInHeap();
    return max;
}

int main()
{
    //int arr[] = { 3, 9, 7, 5, 8, 15, 20, 6, 11, 1 };// { 4, 8, 2, 5, 6, 9, 11, 10, 16, 1 };
    //sizeT n = sizeof(arr) / sizeof(int);

//    Heap<int> h;
    LightArray<Train> trains = ReadTrains();
    int count = GetMinEnd(trains);
    printf("%d", count);
//    LightArray<int> arr;// = new LightArray<T>()
//    h.SetComparator(comparatorUp);
//    for (int i = 0; i < 15; i++)
//    {
//        int rnd = rand() % 100;
//        h.Add(rnd);
//        printf("%d ", rnd);
//    }
//    printf("\n");
//	//	h.Add(rand() % 100);//arr.PushBack(rand() % 1000);
//	//h.Init(arr, comparatorUp);//(trains, comparator);
//	//h.Init(trains, comparatorDown());
//	//h.MakeHeap();
//	//h.HeapSort();
    //h.MakeHeap();
///
//    h.CountInHeap();
//    h.HeapSort();
//    for (int i = 0; i < 15; i++)
//        printf("%d ", h[i]);//.in, h[i].out);
//    //for(int i = 0;i < n; i++)
//        //if (t)rains
//    //ReadTrains()

//    printf("\n");
//    //h.Print();
//    getchar();
//    getchar();

}

//
//template <typename T>
//void Heap<T>::SiftDown(sizeT pos, int(*comparator)(const void*, const void*))
//{
//	sizeT largest;
//	bool flag = 1;
//	for (sizeT i = pos; 2 * i + 1 < count && flag;) // while nodes not empty
//	{
//		largest = i;
//		sizeT left = 2 * i + 1;
//		if (left < count && comparator(&arr[largest], &arr[left]))
//			largest = left;
//
//		sizeT right = 2 * i + 2;
//		if (right < count && comparator(&arr[largest], &arr[right]))
//			largest = right;
//
//		if (largest != i)
//		{
//			swap(&ptr[i], &ptr[largest]);
//			i = largest;
//		}
//		else flag = false;
//	}
//}

//
//Heap<int> h;
//h.Init(arr, n, comparatorUp);
////h.HeapSort();
//for (sizeT i = n / 2; i >= 0; i--)
//h.SiftDown(i);
//
//h.Add(100);
//h.Add(200);
//h.Add(12);
//h.Add(300);
//h.Add(250);
//h.Print();
//static SiftDown(T* pointer, sizeT pos, int
//enum ReallocType {Increase, Decrease};
//T* ptr; // TODO work not with simple array ex vector
//sizeT count;
//sizeT allocated;
// Methods
//void Realloc();


//template <typename T>
//void Heap<T>::Heapify(sizeT pos, int(*comparator)(const void*, const void*))
//{
//	sizeT largest;
//	bool flag = 1;
//	for (sizeT i = pos; 2 * i + 1 < count && flag;) // while nodes not empty
//	{
//		largest = i;
//		sizeT left = 2 * i + 1;
//		if (left < count && comparator(&ptr[largest], &ptr[left]))
//			largest = left;
//
//		sizeT right = 2 * i + 2;
//		if (right < count && comparator(&ptr[largest], &ptr[right]))
//			largest = right;
//
//		if (largest != i)
//		{
//			swap(&ptr[i], &ptr[largest]);
//			i = largest;
//		}
//		else flag = false;
//	}
//}
