#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INIT_CAPACITY 16
#define MAX_STR_SIZE 1000
#define MAX_INPUT_STRINGS_NUM 100

#define EXIT_ERROR_CODE 1

#define error(...) (fprintf(stderr, __VA_ARGS__))

static const char PUNCTUATION_CHARS[] = ",.;:!?\n";

int comparator(const void *a, const void *b) {
    return -strcmp(*(char **) a, *(char **) b);
}

typedef struct {
    char **items;
    int capacity;
    int size;
} my_vector;

int my_vector_constructor(my_vector *v) {
    v->items = malloc(sizeof(char *) * VECTOR_INIT_CAPACITY);
    if (v->items == NULL) {
        return EXIT_ERROR_CODE;
    }
    v->capacity = VECTOR_INIT_CAPACITY;
    v->size = 0;
    return 0;
}

int my_vector_push(my_vector *v, char *item) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        char **tmp = realloc(v->items, sizeof(char *) * v->capacity);
        if (tmp == NULL) {
            return EXIT_ERROR_CODE;
        }
        v->items = tmp;
    }
    v->items[v->size++] = item;
    return 0;
}

void my_vector_destructor(my_vector *v) {
    free(v->items);
    v->items = NULL;
}

int is_not_punctuation_char(char c) {
    if (strchr(PUNCTUATION_CHARS, c) == NULL) {
        return 1;
    }
    return 0;
}

int read_file(const char *filename, my_vector *v) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        error("Cannot open file %s\n", filename);
        return EXIT_ERROR_CODE;
    }
    char buf[MAX_STR_SIZE];
    while (fgets(buf, MAX_STR_SIZE, file)) {
        size_t buf_len = 0;
        size_t str_len = 0;
        for (buf_len = 0; buf[buf_len] != '\0'; buf_len++) {
            char c = buf[buf_len];
            if (is_not_punctuation_char(c)) {
                str_len++;
            }
        }
        char *str = malloc(str_len + 1);
        if (str == NULL) {
            error("Cannot allocate memory\n");
            my_vector_destructor(v);
            fclose(file);
            return EXIT_ERROR_CODE;
        }
        str[str_len] = '\0';
        for (int i = (int)buf_len - 1; i >= 0; i--) {
            char c = buf[i];
            if (is_not_punctuation_char(c)) {
                str[--str_len] = c;
            }
        }
        if (my_vector_push(v, str) != 0) {
            error("Cannot allocate memory\n");
            my_vector_destructor(v);
            fclose(file);
        }
    }
    fclose(file);
    return 0;
}

void print_first_str(my_vector *str) {
    size_t size = MAX_INPUT_STRINGS_NUM < str->size ? MAX_INPUT_STRINGS_NUM : str->size;
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", str->items[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error("Wrong number of parameters\n");
        return EXIT_ERROR_CODE;
    }
    my_vector str_arr;
    if (my_vector_constructor(&str_arr) != 0) {
        error("Cannot allocate memory\n");
        return EXIT_ERROR_CODE;
    }
    if (!read_file(argv[1], &str_arr)) {
        qsort(str_arr.items, str_arr.size, sizeof(char *), comparator);
        print_first_str(&str_arr);
        my_vector_destructor(&str_arr);
        return 0;
    }
    return 1;
}