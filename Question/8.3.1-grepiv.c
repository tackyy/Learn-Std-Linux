#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static int match_type; /* == 1: grep --inverse option */

static void do_grep(regex_t *pat, FILE *f);

static struct option longopts[] = {
    {"ignore", no_argument, NULL, 'i'},
    {"inverse", no_argument, NULL, 'v'}
};

int main(int argc, char *argv[]){
    int opt;
    regex_t pat;
    int err;
    int i;
    int regflag;
    
    if(argc < 2){
        fputs("no pattern\n", stderr);
        exit(1);
    }

    regflag = 0;
    match_type = 0;
    
    while((opt = getopt_long(argc, argv, "iv", longopts, NULL)) != -1){
        switch(opt){
        case 'i':
            regflag = REG_ICASE; /* ignore case; A <-> a, X <-> x etc... */
            match_type = 0;
            break;
        case 'v':
            regflag = 0;
            match_type = REG_NOMATCH;
            break;
        }
    }

    err = regcomp(&pat, argv[optind], REG_EXTENDED | REG_NOSUB | REG_NEWLINE | regflag);
    if(err != 0){
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }

    if(optind + 1 == argc){
        do_grep(&pat, stdin);
    } else {
        for(i = optind + 1; i < argc; i++){
            FILE *f;

            f = fopen(argv[i], "r");
            if(!f){
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src){
    char buf[4096];

    while(fgets(buf, sizeof buf, src)){
        if(regexec(pat, buf, 0, NULL, 0) == match_type){
            fputs(buf, stdout);
        }
    }
}
