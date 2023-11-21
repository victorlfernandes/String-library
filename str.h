#ifndef _STR_H_
#define _STR_H_

#include <stdio.h>
#include <stdbool.h>

typedef char * string_t;

#define INITIAL_BYTES 64
#define NONE_CHAR     -2
#define INVALID_NUM   -1902901
#define DIFF_UPP_LWR  32

//reading functions
bool end_of_line(FILE *stream, char c);
string_t read_until(FILE *stream, char end);
string_t read_str(FILE *stream);
bool ignore_until(FILE *stream, char start);
string_t read_from(FILE *stream, char start);
string_t read_from_until(FILE *stream, char start, char end);
int read_int_until(FILE *stream, char end);
float read_float_until(FILE *stream, char end);
char **read_matrix(FILE *stream, int *n_lines, int *n_columns);

//printing functions
void print_str(FILE *stream, string_t str);
void print_matrix(FILE *stream, char **matrix, int n_lines, int n_columns);

//counting functions
int valid_chars(string_t str, char invalid);
int str_len(string_t str);
int word_counter(string_t str, char delimiter);

//matrix functions
char **str_split(string_t str, char delimiter);
int diff_counter(char **list, int n_strings);
int freq_with_rep(string_t str, char **list, int n_strings, int start);
int freq_counter(string_t str, char **list, int n_strings);
char **matrix_without_rep(char **mat_with_rep, int n_strings);

//characters type functions
bool is_upper(char c);
bool is_lower(char c);
bool is_alpha(char c);
void upper(string_t str);
void lower(string_t str);

//other functions
string_t str_copy(string_t source);
bool str_cmp(string_t str1, string_t str2);
int *find_in_str(string_t string, string_t search);
string_t str_sub(string_t string, string_t error, string_t correction);

#endif
