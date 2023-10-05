#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Убедитесь, что у нас есть два аргумента: seed и arraysize
    if (argc != 3) {
        fprintf(stderr, "Usage: %s seed arraysize\n", argv[0]);
        return 1;
    }

    // Используйте аргументы для передачи в sequential_min_max
    char *args[] = {"./sequential_min_max", argv[1], argv[2], NULL};

    // Запустите sequential_min_max с переданными аргументами
    if (execvp(args[0], args) == -1) {
        perror("execvp");
        return 1;
    }

    return 0;
}
