#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_numbers(int n){
  static int wrappAround = 0;       //Static "behåller" värdet

  printf("%10d ", n);
  wrappAround++;

  if (wrappAround % COLUMNS == 0){
    printf("\n");
    wrappAround = 0;
  }
}
int counter = 0;

void print_sieves(int n){
    int *arr = (int*)malloc((n+1)*sizeof(int));

    for (int i = 0; i <= n;i++){
        arr[i] = 1;
    }

    for (int i = 2; i*i < n;i++){
        if (arr[i]==1){
            for(int j = i*i; j <= n; j = j+i){
                arr[j] = 0;
            }
        }
    }

    for (int i = 2; i <= n; i++){
        if (arr[i] == 1){
            //print_numbers(i);
            counter++;
        }
    }
    printf("number of primes: %d \n",counter);
    
    free(arr);
}



// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}