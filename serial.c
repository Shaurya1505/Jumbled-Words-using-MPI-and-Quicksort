#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Partition function for quicksort */
int partition(char arr[], int l, int h) {
    int i, j;
    char temp;
    char pivot = arr[h];
    i = l - 1;
    for (j = l; j < h; j++) {
        if ((arr[j]) < (pivot)) {
            i++;
            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    temp = arr[h];
    arr[h] = arr[i + 1];
    arr[i + 1] = temp;
    return (i + 1);
}

/* Main quicksort function */
void quicksort(char arr[], int f, int l) {
    int p;
    if (f < l) {
        p = partition(arr, f, l);
        quicksort(arr, f, p - 1);
        quicksort(arr, p + 1, l);
    }
}

int main() {
    // Declaration of variables
    long words_count, ptr_pos, i, j, count, word, jumble_count;
    FILE *fptr, *jumbleTxt, *resultTxt, *notFoundTxt;

    // Timing start
    clock_t begin = clock();

    // Open files for reading
    fptr = fopen("words.txt", "r");
    //change the file to jumbled10000.txt,jumbled12000.txt,jumbled14000.txt,jumbled16000.txt,jumbled18000.txt,jumbled20000.txt as required. Each of these files contain the number of jumbled words specified in the file name.
    jumbleTxt = fopen("jumbled10000.txt", "r"); 

    // Error handling for file opening
    if (fptr == NULL || jumbleTxt == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Read counts from files
    if (fscanf(fptr, "%ld", &words_count) != 1 || fscanf(jumbleTxt, "%ld", &jumble_count) != 1) {
        printf("Error reading file!\n");
        return 1;
    }

    // Memory allocation for word lists
    char **word_list = malloc(words_count * sizeof(char *));
    char **word_list_sort = malloc(words_count * sizeof(char *));
    for (i = 0; i < words_count; i++) {
        word_list[i] = malloc(46 * sizeof(char)); 
        word_list_sort[i] = malloc(46 * sizeof(char)); 
        if (word_list[i] == NULL || word_list_sort[i] == NULL) {
            printf("Memory allocation failed for word lists!\n");
            return 1;
        }
    }

    // Memory allocation for jumbled lists
    char **jumbled_list_orignal = malloc(jumble_count * sizeof(char *));
    char **jumbled_list_sort = malloc(jumble_count * sizeof(char *));
    for (i = 0; i < jumble_count; i++) {
        jumbled_list_orignal[i] = malloc(46 * sizeof(char)); 
        jumbled_list_sort[i] = malloc(46 * sizeof(char)); 
        if (jumbled_list_orignal[i] == NULL || jumbled_list_sort[i] == NULL) {
            printf("Memory allocation failed for jumbled lists!\n");
            return 1;
        }
    }

    // Memory allocation for match_found array
    int *match_found = malloc(jumble_count * sizeof(int));

    // Read words and jumbled words into lists, sort jumbled words
    for (i = 0; i < jumble_count; i++) {
        if (fscanf(jumbleTxt, "%s", jumbled_list_orignal[i]) != 1) {
            printf("Error reading jumbled words!\n");
            return 1;
        }
        strcpy(jumbled_list_sort[i], jumbled_list_orignal[i]);
        quicksort(jumbled_list_sort[i], 0, strlen(jumbled_list_sort[i]) - 1);
    }
    fclose(jumbleTxt);

    for (i = 0; i < words_count; i++) {
        if (fscanf(fptr, "%s", word_list[i]) != 1) {
            printf("Error reading words!\n");
            return 1;
        }
        strcpy(word_list_sort[i], word_list[i]);
        quicksort(word_list_sort[i], 0, strlen(word_list_sort[i]) - 1);
    }
    fclose(fptr);

    // Open result files
    resultTxt = fopen("result_serial/results.txt", "w");
    notFoundTxt = fopen("result_serial/WordNotFound_serial.txt", "w");
    if (resultTxt == NULL || notFoundTxt == NULL) {
        printf("Error creating result file!\n");
        return 1;
    }

    // Search for matches and save results
    for (i = 0; i < jumble_count; i++) {
        int found = 0;
        for (j = 0; j < words_count; j++) {
            if (strcmp(jumbled_list_sort[i], word_list_sort[j]) == 0) {
                fprintf(resultTxt, "%s -> %s\n", jumbled_list_orignal[i], word_list[j]);
                match_found[i] = 1;
                found = 1;
            }
        }
        if (!found) {
            fprintf(notFoundTxt, "%s\n", jumbled_list_orignal[i]);
        }
    }

    // Timing end
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    // Print summary
    printf("\n Operation Complete.");
    printf("\n Total Run Time: %f seconds", time_spent);
    printf("\n Found words saved in file 'results.txt' in the result_serial directory");
    printf("\n Not found words saved in file 'WordNotFound_serial.txt' in the result_serial directory");

    // Close files and free memory
    fclose(resultTxt);
    fclose(notFoundTxt);
    for (i = 0; i < words_count; i++) {
        free(word_list[i]);
        free(word_list_sort[i]);
    }
    for (i = 0; i < jumble_count; i++) {
        free(jumbled_list_orignal[i]);
        free(jumbled_list_sort[i]);
    }
    free(word_list);
    free(word_list_sort);
    free(jumbled_list_orignal);
    free(jumbled_list_sort);
    free(match_found);

    return 0;
}
