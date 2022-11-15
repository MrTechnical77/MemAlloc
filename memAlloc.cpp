#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <stdlib.h>

// Global Variables
bool bestFit = false;
bool implicit = false;

// Implicit free list
int* impList = new int[1000];

// takes int value to indicate size of bytes to malloc
// Returns index(?) of start of payload
int myalloc(int size){
    int pointer = 0;

    return pointer;
}

// Takes pointer to allocated block and size of new block
// Returns index(?) of new block
// Copies payload from old block to new block
// Frees old block
 // If called with size 0 = free
int myrealloc(int pointer, int size){


    return pointer;
}

// Frees allocated block
void myfree(int pointer){

}

// Grows size of simulated heap
// End simulation of size passes 100,000 words
void mysbrk(int size){
    if(implicit){
        
    }
    
}




void printUseage(){
    std::cout << "Useage $ ./memAlloc [-h] -f <first/best> -l <implicit/explicit> -i <input file>" << std::endl;
}

int main(int argc, char* argv[]){

    std::string input;

    // Print useage
    if(strcmp(argv[1], "-h") == 0){
        printUseage();
        return 0;
    }

    // Set first or best fit
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-f")== 0){
            if(strcmp(argv[i+1], "first") == 0){
                bestFit = false;
                break;
            }
            if(strcmp(argv[i+1], "best") == 0){
                bestFit = true;
                break;
            }
        }
    }

    // Set implicit or explicit
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-l")== 0){
            if(strcmp(argv[i+1], "explicit") == 0){
                implicit = false;
                break;
            }
            if(strcmp(argv[i+1], "implicit") == 0){
                implicit = true;
                break;
            }
        }
    }    

    // Set input file
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-i") == 0){
            input = argv[i + 1];
            break;
        }
    }

    FILE* inputFile = fopen(input.c_str(), "r");
    std::ofstream outputFile;
    outputFile.open("output.txt");

    // Check to make sure file was opened
    if(inputFile == NULL){
        std::cout << "Error opening file, check your syntax" << std::endl;
        printUseage();
        return -1;
    }

    char* lineptr = NULL;
    size_t n = 0;

    while(getline(&lineptr, &n, inputFile) != -1){
        outputFile << lineptr;
    }    

    // Cleanup
    fclose(inputFile);
    outputFile.close();

    return 0;
}