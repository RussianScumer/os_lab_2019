#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int pid = fork();

    if (argc != 3) {
        fprintf(stderr, "Usage: %s seed arraysize\n", argv[0]);
        return 1;
    }

    
    char *args[] = {"./sequential_min_max", argv[1], argv[2], NULL};

    if(pid == 0){
    if (execvp(args[0], args) == -1) {
        perror("execvp");
        return 1;
    }
    }
    wait(3);
    return 0;
}
