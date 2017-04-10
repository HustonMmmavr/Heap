#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
#define ALPH_SIZE 256
//#define TEST
// void CountingSort(vector<string> &arr, vector<string> &addArr, int low, int high, int *c, int pos)
// {
// 	// Creating c arr
// 	for (int i = low; i < high; i++)
// 	{
// 		if (pos < arr[i].size())
// 			c[arr[i][pos]]++;
// 	}
	
// 	// Get granci
// 	int sum = 0;
// 	for (int i = 0; i < ALPH_SIZE; i++)
// 	{
// 		int tmp = c[i];
// 		c[i] = sum;
// 		sum += tmp;
// 	}

// 	for (int i = low; i < high; i++) // check size
// 	{
// 		if (pos < arr[i].size())
// 			addArr[c[arr[i][pos]]++ + low] = arr[i];
// 	}
// 	for (int i = low; i < high; i++)
// 		arr[i] = addArr[i];
// }
// //
// void MSDSort(vector<string> &arr, vector<string> &addArr, int left, int right, int pos)//vector<string> &arr)
// {
// 	int *c = new int[ALPH_SIZE +1];
// 	memset(c, 0, (ALPH_SIZE +1)*sizeof(int));
	
// 	int maxLen = -1;
// 	for (int i = left; i < right; i++)
// 	{
// 		if (maxLen < arr[i].size()) maxLen = arr[i].size();
// 	}

// 	if (pos < maxLen)
// 	{
// 		CountingSort(arr, addArr, left, right, c, pos);
// 		for (int i = 0; i < ALPH_SIZE; i++)
// 		{
// 			if (c[i] < c[i + 1])// && pos < maxLen)
// 				MSDSort(arr, addArr, c[i] + left, c[i + 1] + left, pos + 1);
// 		}
// 	}

// 	delete[] c;
// }

void MSDSort(vector<string> &arr, size_t lo, size_t hi, size_t pos)//, vector<string>::iterator last, int offset) {
{
	static int l = 0;
	//printf("%d\n", l++);
   	vector<vector<string> > buf(ALPH_SIZE);
    //vector<vector<string>> buf(ALPHA);

   	for (size_t i = lo; i < hi; i++)
   	{
   		//if (arr[i].size() > pos)
   			buf[arr[i][pos]].push_back(arr[i]);
   	}

    // for (vector<string>::iterator i = first; i != last; ++i)
        // buf[static_cast<int>((*i)[offset])].push_back(*i);
   	//printf("a");
   	for (size_t i = 1; i < ALPH_SIZE; i++)
   	{	if (buf[i].size() > 1)
   			MSDSort(buf[i], 0, buf[i].size(), pos + 1);
    // for (int i = 1; i < ALPHA; ++i) 
        // if (buf[i].size() > 1)
            // msd_sort(buf[i].begin(), buf[i].end(), offset + 1);
    }
    size_t k = 0;
    for (size_t i = lo; i < hi;)
    {
    	for (size_t j = 0; j < buf[k].size(); j++)//buf[k])
    		arr[i++] = buf[k][j];
    	k++;
    }
    // int k = 0;
    // for (vector<string>::iterator i = first; i != last;) {
        // for (vector<string>::iterator j = buf[k].begin(); j != buf[k].end(); ++j)
            // *i++ = *j;
        // ++k;
    // }
}



void MSDSort(vector<string> &arr)
{
	vector<string> addArr(arr.size());
	MSDSort(arr, 0, arr.size(), 0);
}

// int GetString(FILE *f, vector<String> &strArr)
// {
// 	char ch;
// 	String str;

// 	for (ch = fgetc(f); ; ch = fgetc(f))
// 	{
// 		if (ch == EOF || ch == 26)
// 		{
// 			if (str.Count() > 0)
// 			{
// 				str.PushBack('\0');
// 				strArr.PushBack(str);
// 			}
// 			return INPUT_FINISHED;
// 		}

// 		if (ch == '\n')
// 		{
// 			str.PushBack('\0');
// 			strArr.PushBack(str);
// 			return READ_STRING;
// 		}
// 		str.PushBack(ch);
// 	}
// }

// vector<String> GetStringArray(FILE *f)
// {
// 	vector<String> arrStr;
// 	int wrkState = READ_STRING;
// 	while (wrkState != INPUT_FINISHED)
// 		wrkState = GetString(f, arrStr);
// 	return arrStr;
// }

void FillStringArray(vector<string> &arr)
{
	string buf;
    cin >> buf;
    while (!cin.eof()) {
        if (!buf.empty())
            arr.push_back(buf);

        buf.clear();
        cin >> buf;
    }
}

int main()
{
	vector<string> arr;
	FillStringArray(arr);
	MSDSort(arr);//, 0, strArr.Count() - 1, 0);
	for (int i = 0; i < arr.size(); i++)
		printf("%s\n", arr[i].c_str());
	return 0;
}
