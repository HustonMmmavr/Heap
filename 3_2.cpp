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



struct Date
{
    short yyyy;
    char mm;
    char dd;
    Date() {}
    Date(short yyyy1, char mm1, char dd1) : yyyy(yyyy1), mm(mm1), dd(dd1) {}
    void Print(FILE *f) const { fprintf(f, "%2d %2d %d", dd, mm, yyyy); }
    static int Less(const Date &d1, const Date &d2);
    Date& operator = (const Date &d) { yyyy = d.yyyy; mm = d.mm; dd = d.dd;  return *this; }
};

int Date::Less(const Date &d1, const Date &d2)
{
    if (d1.yyyy < d2.yyyy)
        return 1;
    else if (d1.yyyy > d2.yyyy) return 0;

    if (d1.mm < d2.mm)
        return 1;
    else if (d1.mm > d2.mm)
        return 0;

    if (d1.dd < d2.dd)
        return 1;
    else
        return 0;
}


struct Event
{
    enum { BecomeEighteen, Died } typeEvent;
    Date date;
    Event() : typeEvent(BecomeEighteen) {}
    Event (const Event &e)
    {
        date.dd = e.date.dd;
        date.mm = e.date.mm;
        date.yyyy = e.date.yyyy;
        typeEvent = e.typeEvent;
    }
    void SetEighteen(const Date &birth, const Date &die)
    {
        date = birth;
        date.yyyy += 18;
        if (Date::Less(die, date)) // less
            typeEvent = Died;
        else typeEvent = BecomeEighteen;
    }

    void SetDie(const Date &birth, const Date &die)
    {
        Date becomeTooOld = birth;
        becomeTooOld.yyyy += 80; // because 0
        if (Date::Less(becomeTooOld, die))
            date = becomeTooOld;
        else
            date = die;
        typeEvent = Died;
    }

    void Print(FILE *f = stdin) const
    {
        fprintf(f, "%d %d %d\n", date.dd, date.mm, date.yyyy);
    }

    Event& operator = (const Event &e)
    {
        date.dd = e.date.dd;
        date.mm = e.date.mm;
        date.yyyy = e.date.yyyy;
        typeEvent = e.typeEvent;
        return *this;
    }
};

int comparatorUp(const void *arg1, const void *arg2)
{
    return (*(int*)arg1) < (*(int*)arg2);
}

int comparator(const void* arg1, const void *arg2)
{
    Event *e1 = (Event*)arg1;
    Event *e2 = (Event*)arg2;
    Date *d1 = &e1->date;
    Date *d2 = &e2->date;

    if (d1->yyyy < d2->yyyy)
        return 1;
    else if (d1->yyyy > d2->yyyy) return 0;

    if (d1->mm < d2->mm)
        return 1;
    else if (d1->mm > d2->mm)
        return 0;

    if (d1->dd < d2->dd)
        return 1;
    else if (d1->dd > d2->dd) return 0;

    if (e1->typeEvent == Event::Died) return 1;
    else
        return 0;
    //return Date::Less(*d1, *d2);
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
    LightArray<T>& Array() const;
    void Init(const LightArray<T> &arr, int(*funcComparator)(const void*, const void*));
    void SetArraySize(sizeT n);
    void SetHeapSize(sizeT n);
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
void Heap<T>::SetArraySize(sizeT n)
{
    this->arr = new LightArray<T>(n);
    this->elementsInHeap = n;
}

template <typename T>
void Heap<T>::SetHeapSize(sizeT n)
{
    this->elementsInHeap = n;
}

template <typename T>
void Heap<T>::Add(const T& element)
{
    int count = elementsInHeap;
    elementsInHeap++;
    //if (elementsInHeap > arr->Count())
    //	arr->Resize();
    *(arr->AtIndex(count)) = element;
    //(*arr)[count] = element;
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
        swap(arr->AtIndex(i), arr->AtIndex((i-1)/2));//&(*arr)[i], &((*arr)[(i - 1) / 2]));
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
        *(arr->AtIndex(elementsInHeap)) = extr;
        //(*arr)[elementsInHeap] = extr;
    }
}

template <typename T>
void Heap<T>::Print()
{
    for (sizeT i = 0; i < arr->Count(); i++)
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
        if (left < count && comparator(arr->AtIndex(largest), arr->AtIndex(left)))//&(*arr)[largest], &(*arr)[left]))
            largest = left;

        sizeT right = 2 * i + 2;
        if (right < count && comparator(arr->AtIndex(largest), arr->AtIndex(right)))//&(*arr)[largest], &(*arr)[right]))
            largest = right;

        if (largest != i)
        {
            swap(arr->AtIndex(i), arr->AtIndex(largest));//&(*arr)[i], &((*arr)[largest]));
            i = largest;
        }
        else flag = false;
    }
}

template <typename T>
T& Heap<T>::Extremum() const
{
    return *(arr->AtIndex(0));
}

template <typename T>
LightArray<T>& Heap<T>::Array() const
{
    return (*arr);
}

template <typename T>
T Heap<T>::ExtractExtremum()
{
    T extremum = *(arr->AtIndex(0));
    *(arr->AtIndex(0)) = *(arr->AtIndex(elementsInHeap - 1));
//    (*arr)[0] = (*arr)[elementsInHeap - 1];
    elementsInHeap--;
    SiftDown(0);
    return extremum;
}

inline FILE* TestFunc(char *fname, char* rights)
{
    FILE *f;
#ifdef TEST
    f = fopen(fname, rights);
#else
    f = stdin;
#endif
    return f;
}

void ReadEvents(Heap<Event>& eventLine)
{
    FILE *f = TestFunc("E:\\test.txt", "r");
    int n;
    fscanf(f, "%d", &n);
    sizeT size = 2 * n;
    eventLine.SetArraySize(size);
    LightArray<Event> *arr = &eventLine.Array();
    for (int i = 0; i < n; i++)
    {
        int dd1, dd2, mm1, mm2;
        int  yyyy1, yyyy2;
        fscanf(f, "%d %d %d %d %d %d", &dd1, &mm1, &yyyy1, &dd2, &mm2, &yyyy2);
        Date birth = Date(short(yyyy1), char(mm1), char(dd1));  // date become 18
        Date die = Date(short(yyyy2), char(mm2), char(dd2));
        Event birthEvent;
        birthEvent.SetEighteen(birth, die);
        if (birthEvent.typeEvent == Event::BecomeEighteen)
        {
            Event dieEvent;
            dieEvent.SetDie(birth, die);
            arr->PushBack(birthEvent);
            arr->PushBack(dieEvent);
        }
    }

    eventLine.SetHeapSize(arr->Count());
    fclose(f);
}

sizeT GetMax(Heap<Event> &eventLine)
{
    //FILE *f = fopen("/home/comp/out.txt", "w");

    eventLine.SetComparator(comparator);
    sizeT countInHeap = eventLine.Count();
//        LightArray<Event> *arr = &eventLine.Array();
//    for (int i = 0; i < countInHeap; i++)

//        (*arr->AtIndex(i)).Print(f);

    eventLine.HeapSort();
    sizeT max = 0;
    sizeT now = 0;
//    arr->Sort(comparator, 0, arr->Count() - 1);
    LightArray<Event> *arr = &eventLine.Array();
    for (int i = 0; i < countInHeap; i++)
    {
        //(*arr->AtIndex(i)).Print(f);
        if ((*arr->AtIndex(i)).typeEvent != Event::Died) now++;
        else now--;
        if (now > max) max = now;
    }
    return max;
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
