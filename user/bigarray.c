#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"

#define SIZE (1 << 16)  // 65536
#define Q_SIZE (SIZE / 4)  // quarter size

int arr[SIZE];  // Global allocation so it's placed in the .bss/.data segment
int statuses[64];
int summerize(int start_index, int end_index, int* array);

int main(void) {
    // Initialize the array with consecutive numbers
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
    
    int total_sum = 0;    
    int pid_s[4];  // Correctly allocate 4 integers for PIDs
    int forked_result = forkn(4, pid_s);
    if (forked_result < 0) {
        printf("Forkn failed\n");
        exit(1, 0);
    }
    
    if (forked_result == 0) {
        // Parent process: wait for children using waitall (or wait in a loop for now)
        // For this example, we assume waitall is implemented.
          // Assuming NPROC is 64 or adjust accordingly
        int nchildren;
        if (waitall(&nchildren, statuses) < 0) {
            printf("waitall failed\n");
            exit(1, 0);
        }
        if(nchildren != 4){
            printf("Error: expected 4 children, got %d\n", nchildren);
            exit(1, 0);
        }
        for (int i = 0; i < nchildren; i++) {
            total_sum += statuses[i];
        }
        printf("%d", total_sum);
        exit(0, "");
    } else {
        // Child process: forked_result is 1, 2, 3, or 4 indicating the child number.
        int my_sum = summerize(Q_SIZE * (forked_result - 1), Q_SIZE * forked_result, arr);
        // You might not need to print here if the assignment specifies an empty exit message
        // But for debugging you could:
        printf("%d\n", my_sum);
        exit(my_sum, 0);
    }
}

int summerize(int start_index, int end_index, int* array){
    int sum = 0;
    for (int i = start_index; i < end_index; i++) {
        sum += array[i];
    }
    return sum;
}
