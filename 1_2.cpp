#include <stdio.h>
#include <stdlib.h>
#define MEM_ERR -1
#define SCS 1

struct Point
{
    int x;
    int y;
};

int comparator(const void *arg1, const void *arg2)
{
    Point p1 = *((Point*)arg1);
    Point p2 = *((Point*)arg2);
    return p1.x == p2.x ? p1.y < p2.y : p1.x < p2.x;
}

template <typename T>
class SortClass
{
public:
    static void InsertionSort(T* arr, size_t size, int (*funcComparator)(const void*, const void*));
};

template <typename T>
 void SortClass<T>::InsertionSort(T* arr, size_t size, int (*funcComparator)(const void*, const void*))
{
    for (size_t i = 1; i < size; i++)
    {
        T temp = arr[i];
        size_t j;
        for (j = i; j > 0 && funcComparator(&temp, &arr[j - 1]); j--)
            arr[j] = arr[j-1];
        arr[j] = temp;
    }
}


int ReadPoints(Point **arr, size_t *n)
{
    scanf("%lu", n);
    Point *ar = new Point[*n];
    if (!ar) return MEM_ERR;
    int x, y;
    for(size_t i = 0; i < *n; i++)
    {
        scanf("%d %d", &x, &y);
        ar[i] = {x,y};
    }

    *arr = ar;

    return SCS;
}

void GetLine(int n, Point* arr)
{
    SortClass<Point>::InsertionSort(arr, n, comparator);
}

int main()
{
    size_t n;
    Point *arr;
    if(ReadPoints(&arr, &n) == MEM_ERR)
    {
        printf("error");
        return 0;
    }

    GetLine(n, arr);
    for (size_t i = 0; i < n; i++)
        printf("%d %d\n", arr[i].x, arr[i].y);

    delete[] arr;
    return 0;
}
