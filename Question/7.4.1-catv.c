/* Practice Question 7.4.1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GNU_SOURCE
#include <getopt.h>

static int visual;

static struct option longopts[] = {
    {"visual", no_argument, NULL, 'v'}
};

static void do_cat(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]){
    int i;
    int opt;
    visual = 0;

    while((opt = getopt_long(argc, argv, "v", longopts, NULL)) != -1){
        switch(opt){
        case 'v':
            visual = 1;
            break;
        case '?':
            fprintf(stderr, "Usage: %s [-v][filename]\n", argv[0]);
            exit(1);
        }
    }

    if(optind == argc){
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
    } else {
        int i;

        for(i = optind; i < argc; i++){
            do_cat(argv[i]);
        }
        exit(0);
    }
}

#define BUFFER_SIZE 2048

static void do_cat(const char *path){
    char c;
    FILE *file;

    file = fopen(path, "r");
    if(file == NULL) die("fopen");
    while((c = fgetc(file)) != EOF){
        if(visual == 1){
            switch(c){
            case '\t':
                if(fputs("\\t", stdout) < 0) die("fputs");
                break;
            case '\n':
                if(fputs("$\\n\n", stdout) < 0) die("fputs");
                break;
            default:
                if(fputc(c, stdout) < 0) die("fputc");
                break;
            }
        } else {
            if(fputc(c, stdout) < 0) die("fputc");
        }
    }
    if(fclose(file) < 0) die("fclose");
}

static void die(const char *s){
    perror(s);
    exit(1);
}
