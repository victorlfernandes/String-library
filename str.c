#include "str.h"
#include <stdlib.h>

bool end_of_line(FILE *stream, char c) {

    if (c == EOF || c == '\n') return true;
    if (c == '\r') {
        char sec_c = fgetc(stream);
        if (sec_c != '\n') ungetc(sec_c, stream);
        return true;
    }
    return false;

}

string_t read_until(FILE *stream, char end) {

    int bytes = INITIAL_BYTES;
    string_t str = malloc(bytes * sizeof(char));
    int char_counter = 0;

    bool finished = false;
    do {
        
        if (char_counter == bytes) {
            bytes *= 2;
            str = realloc(str, bytes * sizeof(char));
        }

        char c = fgetc(stream);
        finished = (end_of_line(stream, c) || c == end);

        str[char_counter] = (finished ? '\0' : c);
        
        char_counter++;
                
    } while (!finished);

    if (str[0] == '\0') {
        free(str);
        return NULL;
    }

    if (bytes > char_counter)
        str = realloc(str, char_counter * sizeof(char));

    return str;
}

string_t read_str(FILE *stream) {
    return read_until(stream, NONE_CHAR);
}

//returns true in success search of start, false if reaches end of line 
bool ignore_until(FILE *stream, char start) {

    bool reading = true;
    bool missing_start_char = false;
    do {

        char c = fgetc(stream);
        missing_start_char = end_of_line(stream, c);
        if (c == start || missing_start_char) 
            reading = false;

    } while (reading);

    return !missing_start_char;
}

string_t read_from(FILE *stream, char start) {

    bool exists_start_char = ignore_until(stream, start);
    return (exists_start_char) ? read_str(stream) : NULL;

}

string_t read_from_until(FILE *stream, char start, char end) {

    bool exists_start_char = ignore_until(stream, start);
    return (exists_start_char) ? read_until(stream, end) : NULL;

}

int read_int_until(FILE *stream, char end) {

    string_t str = read_until(stream, end);
    if (str == NULL) return INVALID_NUM;

    int num = atoi(str);
    free(str);

    return num;
}

float read_float_until(FILE *stream, char end) {

    string_t str = read_until(stream, end);
    if (str == NULL) return INVALID_NUM;

    float num = atof(str);
    free(str);

    return num;
}

char **read_matrix(FILE *stream, int *n_lines, int *n_columns) {

    //allocating initial bytes
    int init_bytes = 32;
    int l_bytes    = init_bytes;
    int c_bytes    = init_bytes;
    char **matrix  = malloc(l_bytes * sizeof(char *));
    for (int i = 0; i < l_bytes; i++)
        matrix[i]  = malloc(c_bytes * sizeof(char));

    int cur_line   = 0;
    int cur_column = 0;
    bool reading   = true;
    do { 

        //checking if it needs reallocation 
        if (cur_line == l_bytes) {
            l_bytes *= 2;
            matrix = realloc(matrix, l_bytes * sizeof(char *));
            for (int i = l_bytes / 2; i < l_bytes; i++)
                matrix[i] = malloc(c_bytes * sizeof(char));
        }
        if (cur_column == c_bytes) {
            c_bytes *= 2;
            matrix[cur_line] = realloc(matrix[cur_line], c_bytes * sizeof(char *));
        }

        //reading the new character
        char c = fgetc(stream);
        switch (c) {

            case EOF:
                matrix[cur_line][cur_column] = '\n';
                reading = false;
                break;

            case '\n':
                //assigning the line break 
                matrix[cur_line][cur_column] = c;
                *n_columns = cur_column + 1;
                //reallocating the line to its size if necessary
                if (c_bytes > cur_column)
                    matrix[cur_line] = realloc(matrix[cur_line], cur_column + 1);
                cur_column = 0;
                c_bytes = init_bytes;
                cur_line++;
                break;

            default:
                //assigning the read character
                matrix[cur_line][cur_column] = c;
                cur_column++;
                break;
        }

    } while (reading);

    //reallocating the matrix to its size and deallocating the empty lines
    if (l_bytes > cur_line) {
        for (int i = cur_line; i < l_bytes; i++) free(matrix[i]);
        matrix = realloc(matrix, cur_line * sizeof(char *));
    }

    *n_lines = cur_line;
    return matrix;
}

void print_str(FILE *stream, string_t str) {
    fprintf(stream, "%s", str);
}

void print_matrix(FILE *stream, char **matrix, int n_lines, int n_columns) {
    for (int i = 0; i < n_lines; i++) {
        for (int j = 0; j < n_columns; j++)
            fprintf(stream, "%c", matrix[i][j]);
    }
}

int valid_chars(string_t str, char invalid) {

    int char_counter  = 0;
    int valid_counter = 0;

    while (str[char_counter] != '\0') {

        if (str[char_counter] != invalid)
            valid_counter++;

        char_counter++;

    }

    return valid_counter + 1;
}

int str_len(string_t str) {
    return valid_chars(str, NONE_CHAR);
}

int word_counter(string_t str, char delimiter) {

    int delimiter_counter = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == delimiter) 
            delimiter_counter++;  
        i++; 
    }
    return (i == 0) ? 0 : (delimiter_counter + 1);

}

char **str_split(string_t str, char delimiter) {

    int n_words = word_counter(str, delimiter);

    char **words = malloc(n_words * sizeof(char *));
    for (int i = 0; i < n_words; i++) 
        words[i] = malloc(sizeof(char));

    int cur_word = 0;
    int cur_char = 0;
    int i        = 0;

    bool working = true;
    do {
        
        if (str[i] == '\0') {
            working = false;
            words[cur_word][cur_char] = '\0';
        }
        else if (str[i] == delimiter) {
            words[cur_word][cur_char] = '\0';
            cur_word++;
            i++;
            cur_char = 0;
        }
        else {
            words[cur_word][cur_char] = str[i];
            cur_char++;
            i++;
            words[cur_word] = realloc(words[cur_word], (cur_char + 1) * sizeof(char));
        }

    } while (working);

    return words;
}

int diff_counter(char **list, int n_strings) {

    int equal_counter = 1;
    int diff = 0;

    for (int i = 0; i < n_strings; i++) {
        for (int j = i + 1; j < n_strings; j++) {

            bool cmp = str_cmp(list[i], list[j]);
            if (cmp) equal_counter++;

        } 

        if (equal_counter == 1) diff++;
        equal_counter = 1;

    }

    return diff;
}

int freq_with_rep(string_t str, char **list, int n_strings, int start) {

    int equal_counter = 0;

        for (int j = start + 1; j < n_strings; j++) {
            
            bool cmp = str_cmp(str, list[j]);
            if  (cmp)  equal_counter++;

        }

    return equal_counter;
}

int freq_counter(string_t str, char **list, int n_strings) {

    return freq_with_rep(str, list, n_strings, -1);

}

char **matrix_without_rep(char **mat_with_rep, int n_strings) {

    int diff = diff_counter(mat_with_rep, n_strings);

    char **diff_words = malloc(diff * sizeof(char *));
    int cur_word = 0;

    for (int i = 0; i < n_strings; i++) {

        if ((freq_with_rep(mat_with_rep[i], mat_with_rep, n_strings, i)) == 0) {

            diff_words[cur_word] = str_copy(mat_with_rep[i]);
            cur_word++;

        }

    }

   return diff_words;
}

bool is_upper(char c) {
    return (c >= 'A' && c <= 'Z') ? true : false;
}


bool is_lower(char c) {
    return (c >= 'a' && c <= 'z') ? true : false;
}

bool is_alpha(char c) {
    return (is_lower(c) || is_upper(c)) ? true : false;
}

void upper(string_t str) {

    int len = str_len(str);
    for (int i = 0; i < len; i++) {
       if (is_lower(str[i])) 
            str[i] -= DIFF_UPP_LWR;
    }

}

void lower(string_t str) {

    int len = str_len(str);
    for (int i = 0; i < len; i++) {
       if (is_upper(str[i])) 
            str[i] += DIFF_UPP_LWR;
    }

}

string_t str_copy(string_t source) {

    int len_source = str_len(source);
    string_t dest = malloc(len_source);

    for (int i = 0; i < len_source; i++) 
        dest[i] = source[i];

    return dest;
}

bool str_cmp(string_t str1, string_t str2) {

    int str1_len = str_len(str1);
    int str2_len = str_len(str2);
    if (str1_len != str2_len) return false;

    int commom_char_counter = 0;
    for (int i = 0; i < str1_len; i++) {
        if (str1[i] == str2[i])
            commom_char_counter++;
    }

    return (commom_char_counter == str1_len) ? true : false;
}

int *find_in_str(string_t string, string_t search) {

    int *src_pos        = malloc(sizeof(int)); 
    int  string_len     = str_len(string);
    int  src_len        = str_len(search);
    int  src_counter    = 0;

    for (int i = 0; i < string_len; i++) {
        if (string[i] == search[0]) {
            int str_index = i;
            int common_counter = 0;
            for (int j = 0; j < src_len - 1; j++, str_index++) {
                if (string[str_index] == search[j])
                    common_counter++;
            }
            if (common_counter == src_len - 1) {
                src_pos[src_counter] = i;
                src_counter++;
                src_pos = realloc(src_pos, (src_counter + 1) * sizeof(int)); 
            }
        }
    }

    src_pos[src_counter] = INVALID_NUM;
    return src_pos;
} 

string_t str_sub(string_t string, string_t error, string_t correction) {

    int *err_pos = find_in_str(string, error);
    
    int amt_of_err = 0;
    while (err_pos[amt_of_err] != INVALID_NUM) amt_of_err++;

    if (amt_of_err == 0)  {
        free(err_pos);
        return string;
    }

    int err_len = str_len(error);
    int cor_len = str_len(correction);
    int old_len = str_len(string);

    int new_str_size = (cor_len - err_len) * amt_of_err + old_len;
    string_t new_str = malloc(new_str_size);

    int init_pos = 0;
    for (int j = 0; j < amt_of_err; j++) {

        for (int i = init_pos; i < err_pos[j]; i++)
            new_str[i] = string[i];

        for (int i = err_pos[j], j = 0; j < cor_len - 1; i++, j++) 
            new_str[i] = correction[j];

        int chars_left   = old_len - err_pos[j] - err_len + 1; 
        int str_index    = err_pos[j] + err_len - 1;
        int n_str_index  = err_pos[j] + cor_len - 1;
        for (int i = 0; i < chars_left; i++, str_index++, n_str_index++)
            new_str[n_str_index] = string[str_index];

        init_pos = chars_left;
    }

    new_str[new_str_size - 1] = '\0';
    free(err_pos);
    return new_str;
}
