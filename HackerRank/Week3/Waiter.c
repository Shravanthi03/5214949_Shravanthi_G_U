#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'waiter' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY number
 *  2. INTEGER q
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
bool isPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}

int* waiter(int number_count, int* number, int q, int* result_count) {
    // Generate first q primes dynamically
    int* primes = malloc(q * sizeof(int));
    int count = 0;
    int candidate = 2;
    while (count < q) {
        if (isPrime(candidate)) {
            primes[count++] = candidate;
        }
        candidate++;
    }

    int **A = malloc((q+1) * sizeof(int*));
    int *A_tops = malloc((q+1) * sizeof(int));
    int **B = malloc((q+1) * sizeof(int*));
    int *B_tops = malloc((q+1) * sizeof(int));

    for (int i = 0; i <= q; i++) {
        A[i] = malloc(number_count * sizeof(int));
        B[i] = malloc(number_count * sizeof(int));
        A_tops[i] = -1;
        B_tops[i] = -1;
    }

    // Initialize A0 stack with plates (top = last element)
    for (int i = 0; i < number_count; i++) {
        A[0][i] = number[i];
    }
    A_tops[0] = number_count - 1;

    int *result = malloc(number_count * sizeof(int));
    int res_index = 0;

    for (int i = 1; i <= q; i++) {
        int prime = primes[i-1];
        // Process A[i-1]
        while (A_tops[i-1] >= 0) {
            int plate = A[i-1][A_tops[i-1]--];
            if (plate % prime == 0) {
                B[i][++B_tops[i]] = plate;
            } else {
                A[i][++A_tops[i]] = plate;
            }
        }
        // Add plates from B[i] to result
        while (B_tops[i] >= 0) {
            result[res_index++] = B[i][B_tops[i]--];
        }
    }
    // Add leftover plates from A[q]
    while (A_tops[q] >= 0) {
        result[res_index++] = A[q][A_tops[q]--];
    }
    // Free memory except result
    for (int i = 0; i <= q; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    free(A_tops);
    free(B_tops);
    free(primes);

    *result_count = res_index;
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int q = parse_int(*(first_multiple_input + 1));

    char** number_temp = split_string(rtrim(readline()));

    int* number = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int number_item = parse_int(*(number_temp + i));

        *(number + i) = number_item;
    }

    int result_count;
    int* result = waiter(n, number, q, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
