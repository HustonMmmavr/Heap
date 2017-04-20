#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <list>
#include <vector>
using namespace std;
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


void SiftDown(LightArray<int> &arr, int *idxs, int pos, int size, int k)//vector<list<int> > &arr, int pos, int size)
{
    int largest;
    bool flag = 1;
    int count = size;
    for (int i = pos; 2 * i + 1 < count && flag;) // while nodes not empty
    {
        largest = i;
        int left = 2 * i + 1;
        if (left < count && arr[idxs[i]] > arr[idxs[left]])
            largest = left;

        int right = 2 * i + 2;
        if (right < count && arr[idxs[largest]] > arr[idxs[right]])
            largest = right;

        if (largest != i)
        {
            swap(idxs[i], idxs[largest]);
            i = largest;
        }
        else flag = false;
    }
}

void MakeHeap(LightArray<int> &arr, int *idxs, int size, int k)//vector<list<int> > &arr)
{
    for (int i = size / 2 - 1; i >= 0; i--)
        SiftDown(arr, idxs, i, size, k);//arr.size());
}

void ReadArray(LightArray<int> &arr, int *n, int *k)
{
    scanf("%d %d", n, k);
    arr = LightArray<int>(*n);
    for (int i = 0; i < *n; i++)
    {
        int data;
        scanf("%d", &data);
        arr.PushBack(data);
    }
}

void KTrackingMerge(LightArray<int> &arr, int k)
{
    int *idxs = new int[k];
    for (int i = 0; i < k; i++)
        idxs[i] = i;

    int n = arr.Count();
    MakeHeap(arr, idxs, k, k);
    int elements = k;
    while(elements > 0)
    {
		printf("%d ", arr[idxs[0]]);  
        idxs[0] += k;
        if (idxs[0] >= n)
        {
            idxs[0] = idxs[--elements];
        }
        if (elements)
            SiftDown(arr, idxs, 0, elements, k);
    }
    delete[] idxs;
}

int main()
{
    LightArray<int> arr;
    int n;
    int k;
    ReadArray(arr, &n, &k);
    KTrackingMerge(arr, k);
    return 0;
}
