#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node Node;

static const BUFFER_LEN = 256; // its actually allocated as 256 for null termination

typedef struct Node
{
    Node *left;
    Node *right;
    float value;
} Node;

Node root;

// Manipulates the actual string
char *trimString(char *str, size_t len){
    char *start;
    char *end;

    for (size_t i = 0; i < len; i++)
    {
        if ()
        {
            /* code */
        }
        
    }
}

int insert(float value){

};

int delete(float value){

};

int find(float value){

};

int print(){

};


int main(int argc, char const *argv[])
{

    char buffer[BUFFER_LEN];
    char *strippedBuffer;    
    
    for(;;)
    {
        printf("Action: ");
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            fprintf(stderr, "Failed to read input\n");
            return 1;
        }

        char actionChar;


    }
    


    return 0;
};