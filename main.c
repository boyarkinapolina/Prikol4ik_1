#include <stdio.h>
#include <stdlib.h> //библиотека связанная с выделением памяти 
#include <string.h> //работа со строками (копирование, сравнение и т д)
#include <ctype.h> //для определения типа символов 
#include <time.h> // для подсчета времени выполнения ф-ии  
#include <conio.h> // для ввода с клавы 

#define MAX_TEXT_SIZE 1000
#define MAX_INPUT_SIZE 100
#define MAX_FILENAME_SIZE 50

typedef struct {
    int correct_ch, incorrect_ch, total_ch, total_words, time;
} GameStatistics;


//чтение файла 
int load_text(const char* filename, char* text) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return 0;
    }

    fseek(file, 0, SEEK_END); // позиция конца файла  
    long file_size = ftell(file); //определяет размер файла
    fseek(file, 0, SEEK_SET); //позиция начала файла 

    //Проверяет размер файла, чтобы он не превышал заданного размера, в противном случае блокает файл
    if (file_size > MAX_TEXT_SIZE - 1) {
        printf("Error.\n");
        fclose(file);
        return 0;
    }

    size_t bytes_read = fread(text, 1, file_size, file);
    text[bytes_read] = '\0';
    fclose(file);
    return bytes_read; //читает файл и добавляет все прочитанные элементы в bytes_read
}

//для ввода символов
char get_input_character() {
    return _getch();
}
//для подсчета времени 
int get_elapsed_time() {
    return clock() / CLOCKS_PER_SEC;
}


void run_typing_test(const char* text, int time_limit, int max_errors) {
    int text_length = strlen(text); //вычисляет длину строки текста
    int text_index = 0; //для отслеживания текущего индекса символа в тексте
    int num_errors = 0; //для отслеживания кол-ва ошибок  
    int num_characters_typed = 0; // общее кол-во набранных символов 
    int num_words_typed = 0; // кол-во слов 
    double start_time = get_elapsed_time(); //для подсчета времени 
    double current_time = start_time;
    double time_elapsed = 0.0;

    printf("%s\n", text);

    while (time_elapsed < time_limit && text_index < text_length) { //цикл до тех пор пока не будет завершено назначенное время или не закончится текст 
        char input = get_input_character();
        current_time = get_elapsed_time();
        time_elapsed = current_time - start_time; //подсчитывает время выполнения 

        if (input == '\n') { //Если нажать enter заканчивает тест 
            printf("\nTest completed.\n");
            break;
        }

        if (input == text[text_index]) {
            printf("\033[42m%c\033[0m", input); // Подсветка правильного символа голубым
            text_index++;
            num_characters_typed++;
        } else {
            printf("\033[31m%c\033[0m", text[text_index]); // Подсветка неправильного символа оранжевым 
            num_errors++;
            num_characters_typed++;
        }

        if (input == ' ') { //считает кол-во слов  
            num_words_typed++;
        }

        if (num_errors > (max_errors - 1)) {
            printf("\nGame over.\n");
            break;
        }
    }

    printf("\n\n--Typing Test Statistics--\n");
    printf("Correctly entered characters: %d\n", num_characters_typed - num_errors);
    printf("Incorrectly entered characters: %d\n", num_errors);
    printf("Number of characters entered: %d\n", num_characters_typed);
    printf("Average typing speed sieves per minute : %.2f\n", (num_characters_typed / time_elapsed) * 60);
    printf("Average typing speed words per minute: %.2f\n", (num_words_typed / time_elapsed) * 60);
}


//как вводить инфу 
void print_help() {
    printf("Usage: typing_test <text_file> <time_limit> <max_errors>\n");
}

int main(int argc, char* argv[]) {
    if (argc < 4 | argc > 4) {
        printf("Invalid arguments.\n");
        print_help();
        return 1;
    }

    char* text_file = argv[1]; //путь к файлу
    int time_limit = atoi(argv[2]); //ограничение времени в целое число 
    int max_errors = atoi(argv[3]); //макс кол-во ошибок в целое число из строки 

    char text[MAX_TEXT_SIZE];
    if (!load_text(text_file, text)) { //вызывает ф-ию для загрузки текста из файла
        return 1;
    }

    printf("Typing Test\n\n");
    printf("Press Enter to start and enter this text...\n");
    getchar();

    run_typing_test(text, time_limit, max_errors);

    printf("\n\nTest completed.\n");

    return 0;
}