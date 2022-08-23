#define _GNU_SOURCE
#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"

#define sign(a) ( ( (a) < 0 )  ?  -1 : ( (a) > 0 ? 1 : 0) )

struct file_node {
    char             name[FILENAME_MAX];
    struct stat      stat;
    struct file_node *next;
    struct file_node *prev;
};

struct dirlist {
    struct file_node *head;
    struct file_node *tail;
};

void add_node(struct file_node *node, struct dirlist *list){
    if(node == NULL)
        return;
    node->prev = list->tail;
    node->next = NULL;

    if(list->tail == NULL){
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
}

void del_node(struct file_node *node, struct dirlist *list){
    if(node == NULL)
        return;
    if(node->prev == NULL){
        list->head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if(node->next == NULL){
        list->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    free(node);
}

// Maximum length of arrays
int maxl_L = 0; //number of hardlinks
int maxl_U = 0; //username
int maxl_G = 0; //groupname
int maxl_S = 0; //size
int maxl_M = 0; //last modification time
int maxl_N = 0; //file name
int maxl_I = 0; //I-node

//find out maximum length of arrays
void set_max_size(struct file_node *node){
    int length = 0;
    char buffer[40];

    struct group *grp;
    struct passwd *passwd;

    length = strlen(node->name);
    if(length > maxl_N)
        maxl_N = length;
    sprintf(buffer,"%lu",node->stat.st_size);
    length = strlen(buffer);
    if(length > maxl_S)
        maxl_S = length;
    sprintf(buffer,"%lu",node->stat.st_ino);
    length = strlen(buffer);
    if(length > maxl_I)
        maxl_I = length;
    sprintf(buffer,"%lu",node->stat.st_nlink);
    length = strlen(buffer);
    if(length > maxl_L)
        maxl_L = length;
    grp = getgrgid(node->stat.st_gid);
    if(grp == NULL) {
        sprintf(buffer,"%u",node->stat.st_gid);
        length = strlen(buffer);
    } else {
        length = strlen(grp->gr_name);
    }
    if(length > maxl_G)
        maxl_G = length;
    passwd = getpwuid(node->stat.st_uid);
    if(passwd == NULL) {
        sprintf(buffer,"%u",node->stat.st_uid);
        length = strlen(buffer);
    } else {
        length = strlen(passwd->pw_name);
    }
    if(length > maxl_U)
        maxl_U = length;
    length = strftime(buffer,40,"%b %e %Y %H:%M", localtime(&(node->stat.st_mtime)));
    if(length > maxl_M)
        maxl_M = length;
}

//compare orders of two nodes by parameter sort
int nodecmp(struct file_node *node1, struct file_node *node2, char *sort){
    int i,length;
    long int res;
    char buffer1[20], buffer2[20];
    struct group *grp;
    struct passwd *passwd;

    length = strlen(sort);
    for(i=0;i<length;i++){
        switch(sort[i]){
        case 'R':
            mode_to_string(node1->stat.st_mode, buffer1);
            mode_to_string(node2->stat.st_mode, buffer2);
            res = strcasecmp(buffer1,buffer2);
            if( res == 0)
                break;
            return(res);
        case 'L':
            res = node1->stat.st_nlink - node2->stat.st_nlink;
            if(res == 0)
                break;
            return sign(res);
        case 'U':
            passwd = getpwuid(node1->stat.st_uid);
            if(passwd == NULL) {
                sprintf(buffer1,"%u",node1->stat.st_uid);
            } else {
                strcpy(buffer1,passwd->pw_name);
            }
            passwd = getpwuid(node2->stat.st_uid);
            if(passwd == NULL) {
                sprintf(buffer2,"%u",node2->stat.st_uid);
            } else {
                strcpy(buffer2,passwd->pw_name);
            }
            res = strcasecmp(buffer1,buffer2);
            if( res == 0)
                break;
            return(res);
        case 'G':
            grp = getgrgid(node1->stat.st_gid);
            if(grp == NULL) {
                sprintf(buffer1,"%u",node1->stat.st_gid);
            } else {
                strcpy(buffer1,grp->gr_name);
            }
            grp = getgrgid(node2->stat.st_gid);
            if(grp == NULL) {
                sprintf(buffer2,"%u",node2->stat.st_gid);
            } else {
                strcpy(buffer2,grp->gr_name);
            }
            res = strcasecmp(buffer1,buffer2);
            if( res == 0)
                break;
            return(res);
        case 'S':
            res = node1->stat.st_size - node2->stat.st_size;
            if(res == 0)
                break;
            return sign(res);
        case 'I':
            res = node1->stat.st_ino - node2->stat.st_ino;
            if(res == 0)
                break;
            return sign(res);
        case 'M':
            res = node1->stat.st_mtime - node2->stat.st_mtime;
            if(res == 0)
                break;
            return sign(res);
        case 'N':
            // compare name without main '.'
            res = strcasecmp(node1->name[0] == '.'? &(node1->name[1]) : node1->name,
                             node2->name[0] == '.' ? &(node2->name[1]): node2->name);
            if(res == 0)
                break;
            return sign(res);
        }

    }
    // compare name without main '.'
    res = strcasecmp(node1->name[0] == '.'? &(node1->name[1]) : node1->name,
                     node2->name[0] == '.' ? &(node2->name[1]): node2->name);
    return sign(res);
}

//print line by parameter show
void print_ls(struct file_node *node, char *show){
    int i,length;
    char buffer[20], format[10];
    struct group *grp;
    struct passwd *passwd;


    length = strlen(show);
    for(i=0;i<length;i++){
        switch(show[i]){
        case 'R':
            mode_to_string(node->stat.st_mode, buffer);
            printf("%10s", buffer );
            break;
        case 'L':
            sprintf(format,"%%%dhu",maxl_L);
            printf(format,node->stat.st_nlink);
            break;
        case 'U':
            passwd = getpwuid(node->stat.st_uid);
            if(passwd == NULL) {
                sprintf(format,"%%%dd",maxl_U);
                printf(format,node->stat.st_uid);
            } else {
                sprintf(format,"%%-%ds",maxl_U);
                printf(format,passwd->pw_name);
            }
            break;
        case 'G':
            grp = getgrgid(node->stat.st_gid);
            if(grp == NULL) {
                sprintf(format,"%%%dd",maxl_G);
                printf(format, node->stat.st_gid);
            } else {
                sprintf(format,"%%-%ds",maxl_G);
                printf(format,grp->gr_name);
            }
            break;
        case 'S':
            sprintf(format,"%%%dlu",maxl_S);
            printf(format,node->stat.st_size);
            break;
        case 'M':
            sprintf(format,"%%-%ds",maxl_M);
            strftime(buffer,40,"%b %e %Y %H:%M", localtime(&(node->stat.st_mtime)));
            printf(format,buffer);
            break;
        case 'N':
            sprintf(format,"%%-%ds",maxl_N);
            printf(format,node->name);
            break;
        case 'I':
            sprintf(format,"%%%dlu",maxl_I);
            printf(format,node->stat.st_ino);
            break;
        }
        if(i < length-1){
            printf(" ");
        } else {
            printf("\n");
        }
    }

}

int main(int argc, char **argv)
{
    int c;
    bool f_dot = false; // print dot and double dot
    bool f_all = false; // print hidden files
    DIR *dir;
    struct dirent *direntry;
    struct stat filestat;
    struct dirlist list = {NULL,NULL};
    struct file_node *node, *maxnode;

    char show[8] = "RLUGSMN";
    char sort[8] = "";

    //parse arguments
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
        {"all",         no_argument, 0,  'a' },
        {"almost-all",  no_argument, 0,  'A' },
        {"show",  required_argument, 0,  's' },
        {"sort-by",  required_argument, 0,  't' },
        {0,      0,                 0,  0 }
    };

        c = getopt_long(argc, argv, "aA",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'a':
            f_dot = true;
            f_all = true;
            break;

        case 'A':
            f_all = true;
            break;
        case 's':
            for(unsigned int i = 0; i < strlen(optarg); i++){
                if(strchr("RLUGSMNI",optarg[i]) == NULL){
                    printf("incorrect character when defining format of print out.\nCorrect options are RLUGSMNI\n");
                    exit(1);
                }
            }
            strcpy(show, optarg);
            break;
        case 't':
            for(unsigned int i = 0; i < strlen(optarg); i++){
                if(strchr("RLUGSMNI",optarg[i]) == NULL){
                    printf("incorrect character when defining format of sorting\nCorrect options aress RLUGSMNI\n");
                    exit(1);
                }
            }
            strcpy(sort, optarg);
            break;
        }
    }

    //open working directory
    dir=opendir(".");
    if(dir == NULL){
        exit(1);
    }

    //create list with dir entry
    while((direntry = readdir(dir)) != NULL){
        if(!f_dot){
            // if dot or double dot skip entry
            if(strcmp(direntry->d_name,".") == 0 || strcmp(direntry->d_name,"..") == 0){
                continue;
            }
        }
        if(!f_all){
            // if is hiden file skip entry
            if(direntry->d_name[0] == '.')
                continue;
        }

        stat(direntry->d_name, &filestat);

        node = malloc(sizeof(struct file_node));
        if (node == NULL){
            closedir(dir);
            while(list.head != NULL)
                 del_node(list.head,&list);
            exit(1);
        }
        strcpy(node->name,direntry->d_name);
        memcpy(&(node->stat),&filestat,sizeof(struct stat));
        add_node(node,&list);
        set_max_size(node);
    }
    closedir(dir);

    // entry dot and double dot print always first if sort by name
    if(f_dot && strcmp(sort,"") == 0){
        print_ls(list.head, show);
        del_node(list.head, &list);
        print_ls(list.head, show);
        del_node(list.head, &list);
    }

    while(list.head != NULL){
        node = list.head;
        maxnode = list.head;
        while(node != NULL){
            if(nodecmp(node,maxnode,sort) < 0){
                maxnode = node;
            }
            node = node->next;
        }
        print_ls(maxnode,show);
        del_node(maxnode,&list);
    }

    //Clear list if not empty
    while(list.head != NULL)
         del_node(list.head,&list);

    exit(EXIT_SUCCESS);
}
