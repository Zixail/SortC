#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SIZE    100500

void selectSort(int *array, int count, int *comparison, int *moving)
{
    *comparison = 0, *moving = 0;
    for(int i = 0; i < count; ++i){
        int minind = i;
        for(int j = i + 1; j < count; ++j){
            if(array[j] < array[minind]) {
                minind = j;
            }
            ++*comparison;
        }
        ++*moving;
        int min = array[minind];
        array[minind] = array[i];
        array[i] = min;
    }
}

void bubbleSort(int *array, int count, int *comparison, int *moving)
{
    *comparison = 0, *moving = 0;
    for(int i = 0; i < count; ++i){
        for(int j = i + 1; j < count; ++j){
            if (array[j] < array[i]){
                ++*moving;
                int temp = array[j];
                array[j] = array[i];
                array[i] = temp;
            }
            ++*comparison;
        }
    }
}

void shellSort(int *array, int count, int *comparison, int *moving)
{
    *comparison = 0; *moving = 0;
    for (int gap = count/2; gap > 0; gap /= 2) {
        for (int i = gap; i < count; i++) {
            int tmp = array[i];
            int j = i;
            while (j >= gap && tmp < array[j - gap]) {
                ++*comparison;
                array[j] = array[j - gap];
                j -= gap;
                ++*moving;
            }
            ++*comparison;
            array[j] = tmp;
        }
    }
}

void merge(int *array, int *temp, int left, int mid, int right, int *comparison, int *moving){
    int i = left;
    int j = mid + 1;
    int k = left; 
    while (i <= mid && j <= right){
        if (array[i] <= array[j]) temp[k++] = array[i++];
        else temp[k++] = array[j++];
        ++*comparison;
        ++*moving;
    }
    while (i <= mid){
        temp[k++] = array[i++];
        ++*moving;
    }
    while (j <= right){
        temp[k++] = array[j++];
        ++*moving;
    }
    for(int p = left; p <= right; ++p) array[p] = temp[p];
}

void mergeSortRec(int *array, int *temp, int left, int right, int *comparison, int *moving){
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRec(array, temp, left, mid, comparison, moving);
    mergeSortRec(array, temp, mid + 1, right, comparison, moving);
    merge(array, temp, left, mid, right, comparison, moving);
}

void mergeSort(int *array, int count, int *comparison, int *moving){
    *comparison = 0; *moving = 0;
    if (count <= 1) return;
    int temp[SIZE];
    mergeSortRec(array, temp, 0, count - 1, comparison, moving);
}

void writeResult(int *number, int length, char *name)
{
    FILE *fout = fopen(name, "w");
    for(int i = 0; i < length; ++i){
        fprintf(fout, "%d\n", number[i]);
    }

    printf("[+]SORT \"%s\":", name);
        for (int j = 0; j < length-1; j++) {
            printf("%d, ", number[j]);
        }
    printf("%d\n", number[length-1]);
}

void testSort(int *number, int length)
{
    void (*sorts[4])(int*, int, int*, int*) = {selectSort, bubbleSort, shellSort, mergeSort};
    char names[4][30] = {"SelectSort", "BubbleSort", "ShellSort", "MergeSort"};
    int comparisons[4] = {0};
    int moves[4] = {0};
    double times[4] = {0.};

    int comparison, moving;
    time_t begin, end;
    double timedif;

    for(int i = 0; i < 4; ++i){
        int copy[SIZE];
        for(int i = 0; i < length; ++i) copy[i] = number[i];

        begin = clock();
        sorts[i](copy, length, &comparison, &moving);
        end = clock();
        timedif = (double)(end - begin) / CLOCKS_PER_SEC;
        comparisons[i] = comparison;
        moves[i] = moving;
        times[i] = timedif;

        char name[30];
        strcpy(name, names[i]);
        strcat(name, ".txt");
        writeResult(copy, length, name);
    }

    for (int i = 0; i < 4; ++i) {
        printf("\n");
        printf("%s Time taken: %f seconds\n", names[i], times[i]);
        printf("%s Comparison: %d \n", names[i], comparisons[i]);
        printf("%s Moving    : %d \n", names[i], moves[i]);
    }

}

int main(int argc, char *argv[]) {
    int number[SIZE] = {0};
    int length;

    if (argc == 1){
        srand(time(NULL));
        for(length = 0; length < SIZE; ++length){
            number[length] = rand();
        }
    }
    else{
        FILE *fp = fopen(argv[1], "r");
        for(length = 0; fscanf(fp, "%d\n", &number[length]) >= 0; ++length) ;
    }

    testSort(number, length);

    return 0;
}
