// ConsoleApplication7.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

//#include "stdafx.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#define ALPH_SIZE 256
#define MIN_LEN 32
#define INPUT_FINISHED 0
#define READ_STRING 1
#define MIN_SIZE 32
#define MEM_STEP 2
using namespace std;
#include <exception>
class Exception : public std::exception
{
    char message[1024];
public:
    Exception() {}
    Exception(const char *message, const char* file, const char* func, int line);
     ~Exception() throw() {}
    const char* what() const throw();
};

// Exception Realisation
Exception::Exception(const char *message, const char* file, const char* func, int line)
{
    sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
}

const char* Exception::what() const throw()
{
    return message;
}

#define ThrowException(msg) throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);

#define MEMORY_STEP 2
//#define MIN_SIZE 16
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

inline void PrintArr(int *c, int size, FILE *f = stdin)
{
    for (int i = 0; i < size; i++) fprintf(f, "%d\n", c[i]);
}

typedef LightArray<char> String;

void MSDSort(LightArray<String> &arr, int lo, int hi, int pos)
{
    LightArray<LightArray<String> > buf(ALPH_SIZE);
    for (int i = lo; i < hi; i++)
        if (pos < arr[i].Count())
            buf[arr[i][pos]].PushBack(arr[i]);

    for (int i = 0; i < ALPH_SIZE; i++)
        if (buf[i].Count() > 1)
            MSDSort(buf[i], 0, buf[i].Count(), pos + 1);

    int k = 0;
    for (int i = lo; i < hi && k < ALPH_SIZE;)
    {
        for (int j = 0; j < buf[k].Count(); j++)
            arr[i++] = buf[k][j];
        k++;
    }
}



void MSDSort(LightArray<String> &arr)
{
    MSDSort(arr, 0, arr.Count(), 0);
}

int GetString(FILE *f, LightArray<String> &strArr)
{
    char ch;
    String str;

    for (ch = fgetc(f); ; ch = fgetc(f))
    {
        if (ch == EOF || ch == 26)
        {
            if (str.Count() > 0)
            {
                str.PushBack('\0');
                strArr.PushBack(str);
            }
            return INPUT_FINISHED;
        }

        if (ch == '\n')
        {
            str.PushBack('\0');
            strArr.PushBack(str);
            return READ_STRING;
        }
        str.PushBack(ch);
    }
}

LightArray<String> GetStringLightArray(FILE *f)
{
    LightArray<String> arrStr;
    int wrkState = READ_STRING;
    while (wrkState != INPUT_FINISHED)
        wrkState = GetString(f, arrStr);
    return arrStr;
}

//#define TEST
int main()
{
    try
    {
    FILE *f;
#ifdef TEST
    f = fopen("/home/comp/Heap/5_2.cpp", "r");
#else
    f = stdin;
#endif
    LightArray<String> strArr = GetStringLightArray(f);
    MSDSort(strArr);//, 0, strArr.Count() - 1, 0);
    for (int i = 0; i < strArr.Count(); i++)
        printf("%s\n", strArr[i].GetPointer());
    return 0;
    }
    catch(Exception &e)
    {
        printf("%s", e.what());
    }
}
