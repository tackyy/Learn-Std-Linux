#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]){
    int i;
    if(argc < 2){
        fprintf(stderr, "%s: file name not given\n", argv[0]);
        exit(1);
    }

    for(i = 1; i < argc; i++){
        do_cat(argv[i]);
    }
    exit(0);
}

#define BUFFER_SIZE 2048

static void do_cat(const char *path){
    char c;
    FILE *file;

    file = fopen(path, "r");
    if(file == NULL) die("fopen");
    while((c = fgetc(file)) != EOF){
        switch(c){
        case '\t':
            if(fputs("\\t", stdout) < 0) die("fputs");
            break;
        case '\n':
            if(fputs("$\n", stdout) < 0) die("fputs");
            break;
        default:
            if(fputc(c, stdout) < 0) die("fputc");
            break;
        }
    }
    if(fclose(file) < 0) die("fclose");
}

static void die(const char *s){
    perror(s);
    exit(1);
}
