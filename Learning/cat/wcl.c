#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int do_wc(const char *path);
static int countChar(const unsigned char *buf, unsigned int num);
static void die(const char *s);

int main(int argc, char *argv[]){
    int i, count = 0;

    if(argc < 2){
        fprintf(stderr, "%s: file name not given\n", argv[0]);
        exit(1);
    }

    for(i = 1; i < argc; i++){
        count += do_wc(argv[i]);
    }

    printf("\'\\n\': %d\n", count);
    exit(0);
}

#define BUFFER_SIZE 2048

static int do_wc(const char *path){
    int fd;
    unsigned char buf[BUFFER_SIZE];
    int n, cnt = 0;

    fd = open(path, O_RDONLY);
    if(fd < 0) die(path);
    for(;;){
        n = read(fd, buf, sizeof buf);
        cnt = countChar(buf, sizeof buf);
        if(n < 0) die(path);
        if(n == 0) break;
        }
    if(close(fd) < 0) die(path);
    
    return cnt;
}

static int countChar(const unsigned char *buf, unsigned int num){
    int i, cnt = 0;
    for(i = 0; i < num; i++){
        if( *(buf + i) == '\n') cnt++;
    }

    return cnt;
}

static void die(const char *s){
    perror(s);
    exit(1);
}
