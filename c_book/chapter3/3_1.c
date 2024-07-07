#include <stdio.h>
#include <time.h>

int binsearch_old(int x, int v[], int n) {
  int low, mid, high;

  low = 0;
  high = n - 1;

  while(low <= high) {
    mid = (low + high) / 2;

    if (x < v[mid]) {
      high = mid - 1;
    } else if (x > v[mid]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
}

int binsearch(int x, int v[], int n) {
  int low, mid, high;

  low = 0;
  high = n - 1;

  while(low <= high) {
    mid = (low + high) / 2;

    if (x < v[mid]) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  if (x == v[mid]) {
    return mid;
  }

  return -1;
}

int main() {
  int arr[7];
  int counter = 0;
  for (int i = 0; i < 7; i++) {
    counter++;
    arr[i] = counter;
  }

  clock_t time;
  time = clock();

  printf("%d\n", binsearch_old(3, arr, 7));
  printf("%d\n", binsearch_old(10, arr, 7));
  printf("%d\n", binsearch_old(7, arr, 7));
  printf("%d\n", binsearch_old(4, arr, 7));
  time = clock() - time;
  double time_taken = ((double)time)/CLOCKS_PER_SEC;
  printf("old took %f\n", time_taken);
  printf("\n");

  clock_t time2;
  time2 = clock();
  printf("%d\n", binsearch(3, arr, 7));
  printf("%d\n", binsearch(10, arr, 7));
  printf("%d\n", binsearch(7, arr, 7));
  printf("%d\n", binsearch_old(4, arr, 7));
  time2 = clock() - time2;
  double time_taken2 = ((double)time2)/CLOCKS_PER_SEC;
  printf("new took %f\n", time_taken2);

  return 0;
}