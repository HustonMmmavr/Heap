// ConsoleApplication10.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"

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
	Event(const Event &e);
	void SetEighteen(const Date &birth, const Date &die);
	void SetDie(const Date &birth, const Date &die);
	void Print(FILE *f = stdin) const;
	Event& operator = (const Event &e);
};

Event::Event(const Event &e)
{
	date.dd = e.date.dd;
	date.mm = e.date.mm;
	date.yyyy = e.date.yyyy;
	typeEvent = e.typeEvent;
}

void Event::SetEighteen(const Date &birth, const Date &die)
{
	date = birth;
	date.yyyy += 18;
	if (Date::Less(die, date)) // less
		typeEvent = Died;
	else typeEvent = BecomeEighteen;
}

void Event::SetDie(const Date &birth, const Date &die)
{
	Date becomeTooOld = birth;
	becomeTooOld.yyyy += 80; // because 0
	if (Date::Less(becomeTooOld, die))
		date = becomeTooOld;
	else
		date = die;
	typeEvent = Died;
}

void Event::Print(FILE *f) const
{
	fprintf(f, "%d %d %d\n", date.dd, date.mm, date.yyyy);
}


Event& Event::operator = (const Event &e)
{
	date.dd = e.date.dd;
	date.mm = e.date.mm;
	date.yyyy = e.date.yyyy;
	typeEvent = e.typeEvent;
	return *this;
}


template <typename T, bool less>
class IComparator
{
public:
	virtual int Compare(const T& a, const T& b) const = 0;
	virtual ~IComparator() {}
};

template <typename T, bool less>
class DefaultComparator : public IComparator<T, less>
{
public:
	int Compare(const T& a, const T& b) const { if (less) return a < b; return b < a; }
	~DefaultComparator() {}
};

template <typename T = Event, bool less = false>
class EventComparator : public IComparator<T, less>
{
public:
	int Compare(const T& a, const T& b) const
	{
		Date d1 = a.date;
		Date d2 = b.date;

		if (d1.yyyy < d2.yyyy)
			return 1;
		else if (d1.yyyy > d2.yyyy) return 0;

		if (d1.mm < d2.mm)
			return 1;
		else if (d1.mm > d2.mm)
			return 0;

		if (d1.dd < d2.dd)
			return 1;
		else if (d1.dd > d2.dd) return 0;

		if (a.typeEvent == Event::Died) return 1;
		else
			return 0;
	}
	~EventComparator() {}
};

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

////////////////////////////////////////////////////////////////////
// Heap (based on LightArray)

//template <typename T, template<typename, bool > class Comparator = DefaultComparator, bool less = false>//<T, less> >
template <typename T, bool less = false, template<typename, bool> class Comparator = DefaultComparator>
class Heap
{
	sizeT elementsInHeap;
	LightArray<T> *arr;
	IComparator<T, less> *comparator;
private:
	void Swap(T& a, T& b) { arr->Swap(&a, &b); }
	void SiftUp(sizeT pos);
	void SiftDown(sizeT pos);
	T operator[](sizeT i) { return (*arr)[i]; }
public:
	Heap();
	Heap(sizeT cnt, Comparator<T, less> *comparator = NULL);
	~Heap();
	void Init(const LightArray<T> &arr, Comparator<T, less> *comparator = NULL);
	void SetComparator(Comparator<T, less> *comparator) { this->comparator = comparator; }
	void Print();
	sizeT CountInHeap() const { return elementsInHeap; }
	void Add(const T& element);
	void MakeHeap();
	LightArray<T>& Array() const { return *arr; }
	T ExtractExtremum();
	const T& Extremum() const;
};

//template <typename T, template<typename T, bool less> class Comparator , bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

Heap<T, less, Comparator>::Heap()
{
	comparator = NULL;
	arr = new LightArray<T>();
	if (!arr) ThrowException("Cant alloc arr");
	comparator = new Comparator<T, less>();
	if (!comparator) ThrowException("Cant alloc");
	elementsInHeap = 0;
}

//template <typename T, template<typename T, bool less> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

void Heap<T, less,Comparator>::Add(const T& element)
{
	int count = elementsInHeap;
	elementsInHeap++;
	if (elementsInHeap > arr->Allocated())
		arr->Resize();
	arr->AtIndex(count) = element;
	if (count > 0)
		SiftUp(count);
}

//template <typename T, template<typename T, bool less> class Comparator, bool less
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::Init(const LightArray<T> &arr, Comparator<T, less> *comparator)
{
	if (comparator == NULL)
	{
		delete this->comparator;
		this->comparator = new Comparator<T, less>();
	}
	elementsInHeap = arr.Count();
	*this->arr = arr;
}

//template <typename T, template<typename T, bool less> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::SiftUp(sizeT pos)
{
	sizeT i = pos;
	while (comparator->Compare(arr->AtIndex((i - 1) / 2), arr->AtIndex(i)))
	{
		Swap(arr->AtIndex(i), arr->AtIndex((i - 1) / 2));
		i = (i - 1) / 2;
	}
}

//template <typename T, template<typename T, bool less> class Comparator , bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
Heap<T, less, Comparator>::Heap(sizeT cnt, Comparator<T, less> *comparator)
{
	if (comparator == NULL) this->comparator = new Comparator<T,less>();
	arr = new LightArray<T>(cnt);
	if (!arr) ThrowException("Cant alloc");
	elementsInHeap = 0;
}

//template <typename T, template<typename T, bool > class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

Heap<T,less, Comparator>::~Heap()
{
	delete arr;
	delete comparator;
}

//template <typename T, template<typename T, bool > class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

void Heap<T,  less, Comparator>::Print()
{
	for (sizeT i = 0; i < arr->Count(); i++)
		arr->AtIndex(i).Print();	//printf("%d ", arr[i]);
}

//template <typename T, template<typename T, bool > class Comparator,bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

void Heap<T, less, Comparator>::MakeHeap()
{
	for (sizeT i = elementsInHeap / 2 - 1; i >= 0; i--)
		SiftDown(i);
}

//template <typename T, template</*typename*/ T, bool > class Comparator,bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
void Heap<T, less, Comparator>::SiftDown(sizeT pos)
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

//template <typename T, template<typename T, bool> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>
const T& Heap<T, less, Comparator>::Extremum() const
{
	if (elementsInHeap == 0) ThrowException("No elements in heap");
	return (*arr)[0];
}

//template <typename T, template<typename T, bool> class Comparator, bool less>
template <typename T, bool less, template<typename T, bool> class Comparator>

T Heap<T, less, Comparator>::ExtractExtremum()
{
	if (elementsInHeap == 0) ThrowException("No elements in heap");
	T extremum = (*arr)[0];
	(*arr)[0] = (*arr)[elementsInHeap - 1];
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

LightArray<Event> ReadEvents()
{
	FILE *f = TestFunc("E:\\test.txt", "r");
	int n;
	fscanf(f, "%d", &n);
	sizeT size = 2 * n;
	LightArray<Event> events(size);
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
			events.PushBack(birthEvent);
			events.PushBack(dieEvent);
		}
	}
	fclose(f);
	return events;
}

template <typename T, template<typename T, bool less> class Comparator, bool less>// = //EventComparator>
void HeapSort(LightArray<T> &events)
{
	Heap<T, less, Comparator> eventLine;
	eventLine.Init(events);
	eventLine.MakeHeap();
	while (eventLine.CountInHeap() > 0)
	{
		T extr = eventLine.ExtractExtremum();
		sizeT elementsInHeap = eventLine.CountInHeap();
		events.AtIndex(elementsInHeap) = extr;
	}
}

sizeT GetMax(LightArray<Event> &events)
{
	sizeT max = 0;
	sizeT now = 0;
	HeapSort<Event, EventComparator, true>(events);
	int count = events.Count();
	for (int i = 0; i < count; i++)
	{
		if (events.AtIndex(i).typeEvent != Event::Died) now++;
		else now--;
		if (now > max) max = now;
	}
	return max;
}

int main()
{

	//LightArray<int> arr;
	//for (int i = 0; i < 15; i++)
	//{
	//	int rnd = rand() % 1000;
	//	arr.PushBack(rnd);
	//	printf("%d ", rnd);
	//}

	//HeapSort<int, DefaultComparator, false>(arr);// , c);
	//for (int i = 0; i < arr.Count(); i++)
	//	printf("%d ", arr.AtIndex(i));
	try
	{	
		LightArray<Event> events = ReadEvents();
		sizeT max = GetMax(events);
		printf("%u", max);
		return 0;
	}
	catch(Exception &e)
	{
		printf("%d", e.GetMesssage());
	}
}
