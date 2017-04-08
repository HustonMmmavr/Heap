// ConsoleApplication8.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

// ConsoleApplication12.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

//#include "stdafx.h"
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
	const T* GetPointer() const;
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
	//if (sizeToAlloc < MIN_SIZE) sizeToAlloc = MIN_SIZE;
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
const T *LightArray<T>::GetPointer() const
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

LightArray<int> ReadArray(int *k)
{
	int n;
	scanf("%d %d", &n, k);
	LightArray<int> arr;
	for (int i = 0; i < n; i++)
	{
		int b;
		scanf("%d", &b);
		arr.PushBack(b);
	}
	return arr;
}

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

template <typename T>
int KStat(LightArray<int> &arr, int k)
{
	if (k < 0 || k > arr.Count() - 1) ThrowException("Bad k");
	int left = 0, right = arr.Count() - 1;

	while (true)
	{
		int mid = Partition(arr, left, right);
		if (k == mid)
			return arr[k];
		if (mid < k)
			left = mid + 1;
		else
			right = mid - 1;// -1;
	}
}

int main()
{
	try
	{
		int k;
		LightArray<int> arr = ReadArray(&k);
		int res = KStat<int>(arr, k);
		printf("%d", res);
		return 0;
	}
	catch (Exception &e)
	{
		printf("%s", e.GetMessage());
	}
}
