#include "str.h"

int main() {

    char *str = read_until(stdin, ','); 
    if (str == NULL) {
        printf("Verifiquei\n");
        exit(1);
    }

    int n = read_int_until(stdin, ',');
    if (n == INVALID_NUM) {
        printf("Deu ruim\n");
        exit(1);
    }


    printf("Nome: %s\n", str);
    int len = str_len(str);
    printf("\tTamanho do nome: %d\n", len);
    printf("\tRetirando espaços: %d\n", valid_chars(str, ' '));
    printf("\tQuantidade de nomes: %d\n", word_counter(str, ' '));
    printf("Idade: %d\n", n);
    upper(str);
    printf("Nome em maiusculas: %s\n", str);
    lower(str);
    printf("Nome em minusculas: %s\n", str);

    //int list_size;
    //scanf(" %d", &list_size);

    //int amt_of_words = word_counter(str);
    //int *words_frequency = malloc(amt_of_words * sizeof(int));

    //free(words_frequency);
    free(str);

    return 0;
}