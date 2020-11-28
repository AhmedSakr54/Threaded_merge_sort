#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void merge_sort_threaded(int* , int, int);

typedef struct Thread_data {
    int * arr;
    int lo, hi;
}Thread_data;
Thread_data init_Thread_data(int * arr, int lo, int hi) {
    Thread_data to_thread;
    to_thread.arr = arr;
    to_thread.lo = lo;
    to_thread.hi = hi;
    return to_thread;
}

int * read_array_from_file(int * size_of_array) {
    FILE * fp;
    fp = fopen("input.txt", "r");
    int size, i;
    fscanf(fp,"%d", &size);
    int * array = (int *) malloc(size * sizeof(int));
    for (i = 0; i < size; i++) {
        fscanf(fp, "%d ", &array[i]);
    }
    fclose(fp);
    *size_of_array = size;
    return array;
}
void print_array(int * arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void merge(int * arr, int lo, int mid, int hi) {
    int size_half1 = mid - lo + 1;
    int size_half2 = hi - mid;
    int * arr1 = (int *) malloc(size_half1 * sizeof(int));
    int * arr2 = (int *) malloc(size_half2 * sizeof(int));
    int i;
    for (i = 0; i < size_half1; i++) 
        arr1[i] = arr[lo + i];
    for (i = 0; i < size_half2; i++) 
        arr2[i] = arr[mid + 1 + i];
    i = 0;
    int j = 0;
    int k = lo;
    while (i < size_half1 && j < size_half2) {
        if (arr1[i] <= arr2[j]) {
            arr[k] = arr1[i++];
        }
        else {
            arr[k] = arr2[j++];
        }
        k++;
    }
    while (i < size_half1)
        arr[k++] = arr1[i++];
    while (j < size_half2)
        arr[k++] = arr2[j++];
    free(arr1);
    free(arr2);
}

//Thread function
void * merge_sort(void * t_data) {
    Thread_data * in_thread = (Thread_data *) t_data;

    if (in_thread->lo >= in_thread->hi) {
        return NULL;
    }
    int mid = (in_thread->lo + in_thread->hi) / 2;
    pthread_t th1, th2;
    Thread_data to_thread1 = init_Thread_data(in_thread->arr, in_thread->lo, mid);
    pthread_create(&th1, NULL, merge_sort, &to_thread1);
    Thread_data to_thread2 = init_Thread_data(in_thread->arr, mid+1, in_thread->hi);
    pthread_create(&th2, NULL, merge_sort, &to_thread2);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    merge(in_thread->arr, in_thread->lo, mid, in_thread->hi);
    
    pthread_exit(NULL);
}

void merge_sort_threaded(int * arr, int lo, int hi) {
    Thread_data to_thread = init_Thread_data(arr, lo, hi);
    pthread_t th_big_child;
    pthread_create(&th_big_child, NULL, merge_sort, &to_thread);
    pthread_join(th_big_child, NULL);
}

int main() {
    int size_of_array = 0;
    int *arr = read_array_from_file(&size_of_array);
    merge_sort_threaded(arr, 0, size_of_array-1);
    print_array(arr, size_of_array);
    return 0;
}