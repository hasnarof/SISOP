/*
  Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan.
*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

typedef unsigned long long int ULLI;

/*
  Function to calculate factorial of x.
*/
ULLI factorial(int x) {
  if (x == 1)
    return 1;
  else
    return (ULLI)factorial(x-1)*x;
}

/*
  Find factorial.
*/
void *findFactorial(void *arg) {
  int x = *((int*)arg);
  printf("%d! = %llu\n", x, factorial(x));
}


int main(int argc, char *argv[]) {
  pthread_t tid[argc-1];

  for (int i = 1; i < argc; i++) {
    int *x;
    x = (int*)malloc(sizeof(*x));
    if (x == NULL) {
      // Fail to allocate memory
      exit(EXIT_FAILURE);
    }
    *x = (int)atoi(argv[i]);

    int err = pthread_create(&(tid[i-1]), NULL, &findFactorial, x);
    if (err != 0) {
      // Fail to make thread
      exit(EXIT_FAILURE);
    }

    // Join thread
    pthread_join(tid[i-1], NULL);

    // Deallocate allocated memory
    free(x);
  }
  return 0;
}
