#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <stack>
//#define TEST
using namespace std;
#define MIN_SIZE 16
#define MEMORY_STEP 2
typedef long sizeT;
//#define TEST
//#define TEST_DATA
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
// Array (my vector)
typedef long sizeT;
template <typename T>
class Array
{
	T* ptr;
	sizeT allocatedSize;
	sizeT elementsInBuffer;
public:
	Array();
	Array(sizeT allocatedSize);
	Array(const Array& arr);
	~Array();
	void PushBack(const T& data);
	void Resize();
	sizeT Count() const { return elementsInBuffer; }
	sizeT Allocated() const { return allocatedSize; }
	T* GetPointer();
	void Swap(T *a, T* b) { T temp = *a; *a = *b, *b = temp; }
	const T& operator[] (sizeT i) const { return ptr[i]; }
	T& operator[] (sizeT i) { return ptr[i]; }
	Array& operator = (const Array& arr);
	const T& AtIndex(sizeT i) const { return ptr[i]; }
	T& AtIndex(sizeT i) { return ptr[i]; }

};


template <typename T>
Array<T>::Array()
{
	allocatedSize = 0;
	ptr = NULL;
	elementsInBuffer = 0;
}

template <typename T>
Array<T>::Array(const Array<T>& arr)
{
	ptr = NULL;
	*this = arr;
}

template <typename T>
Array<T>::Array(sizeT sizeToAlloc)
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
Array<T>::~Array()
{
	delete[] ptr;
}

template <typename T>
void Array<T>::PushBack(const T& data)
{
	if (elementsInBuffer == allocatedSize)
		Resize();
	ptr[elementsInBuffer++] = data;
}

template <typename T>
void Array<T>::Resize()
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
T *Array<T>::GetPointer()
{
	return ptr;
}

template <typename T>
Array<T>& Array<T>::operator = (const Array<T>& arr)
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
//int MedianOfThree(int *arr, int left, int right)
//{
//	int mid = (right + left) / 2;
//	if (arr[left] > arr[right]) swap(right, left);
//	if (arr[left] > arr[mid]) swap(left, mid);
//	if (arr[mid] > arr[right]) swap(mid, right);//arr[mid], arr[right]);
//	return mid;
//}

template <typename T>
int MedianOfThree(Array<T> &arr, int left, int right)
{
	int mid = (right + left) / 2;
	if (arr[left] > arr[right]) swap(right, left);
	if (arr[left] > arr[mid]) swap(left, mid);
	if (arr[mid] > arr[right]) swap(mid, right);//arr[mid], arr[right]);
	return mid;
}

template <typename T>
int Partition(Array<T> &arr, int left, int right)
{
	int mid = MedianOfThree(arr, left, right);
	swap(arr[mid], arr[right]);
	T piv = arr[right];// - 1];
	int i = left;

	while (i < right && arr[i] < piv) i++; // search group > piv
	if (i >= right) return right;
	int j = i + 1;

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
void InsertionSort(Array<T> &arr, int left, int right)
{
	for (int i = left; i < right; i++)
	{
		int temp = arr[i];
		int j;
		for (j = i; j > 0 && temp < arr[j - 1]; j--)
			arr[j] = arr[j - 1];
		arr[j] = temp;
	}
}

template <typename T>
void QuickSort(Array<T> &arr, int l, int h) 
{
	stack<int> idx;
	idx.push(l);
	idx.push(h);

	while (idx.size() > 0) 
	{
		h = idx.top();
		idx.pop();
		l = idx.top();
		idx.pop();

		if (h - l <= 30) 
		{
			InsertionSort(arr , l, h + 1);
			continue;
		}

		int p = Partition(arr, l, h);

		if (p - 1 > l) 
		{
			idx.push(l);
			idx.push(p - 1);
		}

		if (p + 1 < h) 
		{
			idx.push(p + 1);
			idx.push(h);
		}
	}
}


template <class T>
void QuickSort(Array<T> &arr)//, int l, int h)
{
	stack<int> idx;
	idx.push(0);
	idx.push(arr.Count() - 1);
	int h, l;
	while (idx.size() > 0)
	{
		int h = idx.top();
		idx.pop();
		int l = idx.top();
		idx.pop();

		if (h - l <= 30)
		{
			InsertionSort(arr + l, 0, h - l + 1);
			continue;
		}

		int p = Partition(arr, l, h);

		if (p - 1 > l)
		{
			idx.push(l);
			idx.push(p - 1);
		}

		if (p + 1 < h)
		{
			idx.push(p + 1);
			idx.push(h);
		}
	}
}

void ReadArray(Array<int> &arr)
{
	int n;
	FILE *f;
#ifdef TEST_DATA
	f = fopen("/home/comp/test.txt", "r");
#else
	f = stdin;
#endif
	int i = 0;
	while (fscanf(f, "%d", &n) == 1)
		arr.PushBack(n);
}

int main()
{
	Array<int> arr;
	//int *arr = new int[25000000];
	ReadArray(arr);
	FILE *f;
	QuickSort(arr, 0, arr.Count() - 1);//count - 1);
#ifdef TEST_DATA
	for (int i = 0; i < count; i++)
		printf("%d ", arr[i]);
#endif
	//Sort(arr, 0, arr.Count());
	int count = arr.Count();
	for (int i = 9; i < count; i += 10)
	{
		printf("%d ", arr[i]);
	}
	return 0;
}
