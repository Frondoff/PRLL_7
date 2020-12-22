#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

const int N = 100000;
const int M = 50000;
const int numOfThreads = 4;

void initArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = i;
	}
}

double findFirstSum(int arr[]) {

	double sum = 0;
	double partSum = 0;
	int numbOfThreadElement = ceil(N * 1.0 / numOfThreads);

#pragma omp parallel for reduction(+: sum) private(partSum) num_threads(numOfThreads)
	for (int i = 0; i < numOfThreads; i++) {

		partSum = 0;

		for (int j = i * numbOfThreadElement; j < (i + 1) * numbOfThreadElement; j++) {
			partSum += arr[j];
		}
		sum += partSum;
	}

	return sum;
}

int findSecondSum(int arr[]) {
	int activeSize = M;
	int lastSize = M;
	int temp;
	do {
		if (activeSize % 2 == 0) {
			activeSize /= 2;
		}
		else {
			activeSize = activeSize / 2 + 1;
		}

#pragma omp parallel for private(temp)
		for (int i = 0; i < activeSize; i++) {
			if (i != lastSize - i - 1) {
				temp = arr[lastSize - i - 1];
				arr[i] += temp;
				arr[lastSize - i - 1] = 0;
			}
		}
		lastSize = activeSize;

	} while (activeSize > 1);

	return arr[0];
}

int Min(int *arr)
{
	int minIndRezult;
#pragma omp parallel
	{
		int minInd = N - 1;
#pragma omp parallel for
		for (int i = N - 1; i >= 0; i--) //З кінця для того, щоб мінімальне значення і його індекс змінювались не один раз за весь хід виконання
			if (arr[minInd] > arr[i]) {
				minInd = i;
			}
#pragma omp critical
		minIndRezult = minInd;
	}
	return minIndRezult;
}

int main()
{
	int arr[N];
	int arr2[M];

	initArray(arr, N);
	cout << "First sum is >> " << findFirstSum(arr) << endl;

	initArray(arr2, M);
	cout << "Second sum is >> " << findSecondSum(arr2) << endl;

	srand(time(NULL));
	int minInd = rand() % N;
	arr[minInd] = -1;

	int foundMinInd = Min(arr);
	cout << "Minimum value is >> " << arr[foundMinInd] << " and it`s index is >> " << foundMinInd << endl;
	cout << "Control index is >> " << minInd;
}