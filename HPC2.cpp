#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

// Sequential Bubble Sort
void bubbleSort(vector<int> &arr)
{
    int n = arr.size();

    for(int i = 0; i < n-1; i++)
    {
        for(int j = 0; j < n-i-1; j++)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort (Odd-Even Sort)
void oddEvenSort(vector<int> &arr)
{
    int n = arr.size();

    bool sorted = false;

    while(!sorted)
    {
        sorted = true;

        // Odd Phase
        #pragma omp parallel for
        for(int i = 1; i < n-1; i += 2)
        {
            if(arr[i] > arr[i+1])
            {
                swap(arr[i], arr[i+1]);

                sorted = false;
            }
        }

        // Even Phase
        #pragma omp parallel for
        for(int i = 0; i < n-1; i += 2)
        {
            if(arr[i] > arr[i+1])
            {
                swap(arr[i], arr[i+1]);

                sorted = false;
            }
        }
    }
}

// Merge Function
void merge(vector<int> &arr, int left, int mid, int right)
{
    vector<int> temp;

    int i = left;
    int j = mid + 1;

    while(i <= mid && j <= right)
    {
        if(arr[i] < arr[j])
            temp.push_back(arr[i++]);

        else
            temp.push_back(arr[j++]);
    }

    while(i <= mid)
        temp.push_back(arr[i++]);

    while(j <= right)
        temp.push_back(arr[j++]);

    for(int k = 0; k < temp.size(); k++)
    {
        arr[left + k] = temp[k];
    }
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int> &arr, int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;

        sequentialMergeSort(arr, left, mid);

        sequentialMergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int> &arr, int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// Print Vector
void printVector(vector<int> &arr)
{
    for(int x : arr)
    {
        cout << x << " ";
    }

    cout << endl;
}

int main()
{
    vector<int> arr(1000);

    for(int i = 0; i < 1000; i++)
    {
        arr[i] = rand() % 1000;
    }

    vector<int> temp;

    // Sequential Bubble Sort
    temp = arr;

    auto start = high_resolution_clock::now();
    bubbleSort(temp);
    auto end = high_resolution_clock::now();
    double time1 = duration<double>(end - start).count();
    cout << "Sequential Bubble Sort: ";
    printVector(temp);
    cout << "Time: " << time1 << endl << endl;

    // Parallel Bubble Sort
    temp = arr;

    start = high_resolution_clock::now();
    oddEvenSort(temp);
    end = high_resolution_clock::now();
    double time2 = duration<double>(end - start).count();
    cout << "Parallel Bubble Sort: ";
    printVector(temp);
    cout << "Time: " << time2 << endl;

    cout << "SpeedUp: " << time1 / time2 << endl << endl;

    // Sequential Merge Sort
    temp = arr;

    start = high_resolution_clock::now();
    sequentialMergeSort(temp, 0, temp.size()-1);
    end = high_resolution_clock::now();
    double time3 = duration<double>(end - start).count();
    cout << "Sequential Merge Sort: ";
    printVector(temp);
    cout << "Time: " << time3 << endl << endl;

    // Parallel Merge Sort
    temp = arr;

    start = high_resolution_clock::now();
    parallelMergeSort(temp, 0, temp.size()-1);
    end = high_resolution_clock::now();
    double time4 = duration<double>(end - start).count();
    cout << "Parallel Merge Sort: ";
    printVector(temp);
    cout << "Time: " << time4 << endl;
    
    cout << "SpeedUp: " << time3 / time4 << endl;
    return 0;
}