#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <cstdint>
#include <iomanip>
#include <math.h>
#include <limits.h>

// Global Variables
bool bestFit = false;
bool implicit = false;

// Implicit free list
int impSize = 1000;
int* impList = new int[impSize];

// Array to store pointers
int ptrsArr[1000];
int blockAmt = 0;

// takes int value to indicate size of bytes to malloc
// Returns index(?) of start of payload
int myalloc(int size){
    int pointer = 0;

    // Calculate size of payload
    int words = std::round(ceil(size * 0.25f) * 0.5f) * 2.0f;

    // Implicit free list malloc
    if(implicit){   

        // Implicit Best Fit
        if(bestFit){

            int lowest = INT_MAX;
            int lowestIndex = 0;

            // Create list of possible locions to store malloc
            int j = 1;
            while(j < impSize - 1){
                if(impList[j] % 2 == 1){
                    j += ((impList[j] - 1) / 4);
                    std::cout << "index: " << j << std::endl;
                }
                else if (impList[j] % 2 == 0){

                    if(impList[j] < lowest){
                        if(impList[j] >= size){
                            lowest = impList[j];
                            lowestIndex = j;
                        }
                    }


                    j += (impList[j] / 4);
                    std::cout << "Index: " << j << std::endl;
                }
            }

            // Store old free size
            int temp = impList[lowestIndex];

            // Write header
            impList[lowestIndex] = 1 + (4 * (words + 2));

            // Write footer
            impList[lowestIndex + words + 1] = 1 + (4 * (words + 2));

            // Write new free size header
            int freedHeadIndex = lowestIndex + words + 2;

            int newFreeSize = temp - (4 * (words + 2));

            impList[freedHeadIndex] = newFreeSize;
            //std::cout << freedHeadIndex << std::endl;

            // Write new freed footer
            int freedFootIndex = freedHeadIndex + (newFreeSize / 4) - 1;
            //std::cout << "Footer: " << freedFootIndex << std::endl;
            impList[freedFootIndex] = newFreeSize;
            
            
            return lowestIndex;


        } // End Implicit Best Fit

        // Implicit First Fit
        else {
            int i = 1;
            int temp = 0;
            
            // Find first open spot
            for(;;){
                // Check to see if already allocated
                if(impList[i] & 1){
                    i +=((impList[i] - 1) / 4);
                }
                // If not allocated check if size is large enough
                else if((impList[i] / 4) < words + 2){
                    i +=((impList[i]) / 4);
                }
                else
                    break;
            }

            // Store old free size
            temp = impList[i];

            // Write header
            impList[i] = 1 + (4 * (words + 2));

            // Write footer
            impList[i + words + 1] = 1 + (4 * (words + 2));

            // Write new free size header
            int freedHeadIndex = i + words + 2;

            int newFreeSize = temp - (4 * (words + 2));

            impList[freedHeadIndex] = newFreeSize;
            //std::cout << freedHeadIndex << std::endl;

            // Write new freed footer
            int freedFootIndex = freedHeadIndex + (newFreeSize / 4) - 1;
            //std::cout << "Footer: " << freedFootIndex << std::endl;
            impList[freedFootIndex] = newFreeSize;
            
            return i;


        } // End implicit First Fit

    }

    // Explicit free list malloc
    else {

        // Best Fit Explicit
        if(bestFit){

        }

        // First Fit Explicit
        else {

            int i = 1;
            int temp = 0;
            
            // Find first open spot
            for(;;){
                // Check to see if already allocated
                if(impList[i] & 1){
                    i +=((impList[i] - 1) / 4);
                }
                // If not allocated check if size is large enough
                else if((impList[i] / 4) < words + 2){
                    i +=((impList[i]) / 4);
                }
                else
                    break;
            }

            // Store old free size
            temp = impList[i];

            // Write header
            impList[i] = 1 + (4 * (words + 2));

            // Only write new pointer if there isn't one there
            if(impList[i + 1] == 0)
                impList[i + 1] = -1;
            else if(impList[i + 1] != -1){
                impList[impList[i + 1] + 2] = -1;
            }

            if(impList[i + 2] == 0)
                impList[i + 2] = -1;
            else if(impList[i + 2] != -1){
                impList[impList[i + 2] + 1] = -1;
            }

            // If pointer exists update the blocks it is pointing to


            // Write footer
            impList[i + words + 1] = 1 + (4 * (words + 2));

            // Write new free size header
            int freedHeadIndex = i + words + 2;

            int newFreeSize = temp - (4 * (words + 2));

            impList[freedHeadIndex] = newFreeSize;
            impList[freedHeadIndex + 1] = -1;
            impList[freedHeadIndex + 2] = -1;

            // Write new freed footer
            int freedFootIndex = freedHeadIndex + (newFreeSize / 4) - 1;

            impList[freedFootIndex] = newFreeSize;
            
            return i;

        }

    }

    return pointer;
}

// Frees allocated block
void myfree(int pointer){

    // Implicit free
    if(implicit){

        if(true) {

            int headerIndex = ptrsArr[pointer];
            int size = (impList[headerIndex] - 1) / 4;
            int footerIndex = ptrsArr[pointer] + size - 1;
            bool cAbove = false;
            bool cBelow = false;

            // Check if already freed
            
            if(impList[headerIndex] % 2 == 0){
                return;
            }   

            // If coalesce above and below
            if(impList[headerIndex - 1] % 2 == 0 && impList[footerIndex + 1] % 2 == 0){
                int newSize = (impList[headerIndex] - 1) + impList[headerIndex - 1] + impList[footerIndex + 1];

                int newHeaderIndex = headerIndex - (impList[headerIndex - 1] / 4);
                int newFooterindex = footerIndex + (impList[footerIndex + 1] / 4);

                impList[newHeaderIndex] = newSize;
                impList[newFooterindex] = newSize;

                return;
            }

            // Check for lower address coalesce
            if(impList[headerIndex - 1] % 2 == 0){
                cAbove = true;
                int newSize = (impList[headerIndex] - 1) + impList[headerIndex - 1];
                if(newSize % 2 != 0){
                    newSize++;
                }

                int newHeaderIndex = headerIndex - (impList[headerIndex - 1] / 4);
                
                impList[newHeaderIndex] = newSize;
                headerIndex = newHeaderIndex;
                impList[footerIndex] = newSize;
            }

            // Check for higher address coalesce
            if(impList[footerIndex + 1] % 2 == 0){
                cBelow = true;
                int newSize = (impList[footerIndex] - 1) + impList[footerIndex + 1];
                if(newSize % 2 != 0){
                    newSize++;
                }

                int newFooterIndex = footerIndex + (impList[footerIndex + 1] / 4);

                impList[newFooterIndex] = newSize;
                footerIndex = newFooterIndex;
                impList[headerIndex] = newSize;
            }

            // Update header and footer
            if(cAbove == false && cBelow == false){
                impList[headerIndex]--;
                impList[footerIndex]--;
            }
            
        } // End implicit free

    }

    // Explicit free
    else {
            int headerIndex = ptrsArr[pointer];
            int size = (impList[headerIndex] - 1) / 4;
            int footerIndex = ptrsArr[pointer] + size - 1;
            bool cAbove = false;
            bool cBelow = false;
            bool both = false;

            // Check if already freed
            if(impList[headerIndex] % 2 == 0){
                return;
            }

            // If coalesce above and below
            if(impList[headerIndex - 1] % 2 == 0 && impList[footerIndex + 1] % 2 == 0){
                int newSize = (impList[headerIndex] - 1) + impList[headerIndex - 1] + impList[footerIndex + 1];

                int newHeaderIndex = headerIndex - (impList[headerIndex - 1] / 4);
                int newFooterindex = footerIndex + (impList[footerIndex + 1] / 4);

                impList[newHeaderIndex] = newSize;
                impList[newFooterindex] = newSize;         

                int index = newFooterindex + 1;

                impList[newHeaderIndex + 1] = impList[footerIndex + 1];
                impList[newHeaderIndex + 2] = impList[headerIndex + 2];

                std::cout << "Above Below\n";


                /*
                for(;;){
                    if(index > impSize - 2)
                        break;

                    if(impList[index] % 2 == 0){
                        impList[index + 1] = newHeaderIndex;
                        break;
                    }
                    else{
                        index += (impList[index] - 1) / 4;
                    }
                }
                */

                return;
            }

            // Check for lower address coalesce
            if(impList[headerIndex - 1] % 2 == 0){
                cAbove = true;
                int newSize = (impList[headerIndex] - 1) + impList[headerIndex - 1];
                if(newSize % 2 != 0){
                    newSize++;
                }

                int newHeaderIndex = headerIndex - (impList[headerIndex - 1] / 4);
                
                impList[newHeaderIndex] = newSize;
                headerIndex = newHeaderIndex;
                impList[footerIndex] = newSize;
            }

            // Check for higher address coalesce
            if(impList[footerIndex + 1] % 2 == 0){
                cBelow = true;
                int newSize = (impList[footerIndex] - 1) + impList[footerIndex + 1];
                if(newSize % 2 != 0){
                    newSize++;
                }

                int newFooterIndex = footerIndex + (impList[footerIndex + 1] / 4);

                impList[newFooterIndex] = newSize;
                footerIndex = newFooterIndex;
                impList[headerIndex] = newSize;

                int newNextIndex = headerIndex + 2;
                int index = newFooterIndex + 1;

                for(;;){
                    if(index > impSize - 2)
                        break;
                    
                    if(impList[index] % 2 == 0){
                        impList[newNextIndex] = index;
                        impList[index + 1] = headerIndex;
                        break;
                    }
                    else if(impList[index] % 2 != 0){
                        index += (impList[index] - 1) / 4;
                    } 
                }

            }

            // Update header and footer
            if(cAbove == false && cBelow == false){
                impList[headerIndex]--;
                impList[footerIndex]--;

                int index = headerIndex;

                for(;;){
                    if(index > impSize)
                        break;

                    if(impList[index] % 2 == 0 && index != headerIndex){
                        impList[headerIndex + 2] = index;
                        impList[index + 1] = headerIndex;
                        break;
                    }
                    else if(impList[index] % 2 != 0){
                        index += (impList[index] - 1) / 4;
                    }
                    else{
                        index += impList[index] / 4;
                    }
                }

            }        




            

    } // End Explicit Free

}

// Takes pointer to allocated block and size of new block
// Returns index(?) of new block
// Copies payload from old block to new block
// Frees old block
 // If called with size 0 = free
int myrealloc(int pointer, int size){

    // Calculate size of payload
    int words = std::round(ceil(size * 0.25f) * 0.5f) * 2.0f;

    // Implicit free list realloc
    if(implicit){

        int newHeader = myalloc(size);
        int j = 0;

        for(int i = pointer; i < ((impList[pointer] - 1) / 4) - 1 + pointer; i++){
            if(j != 0){
                impList[newHeader + j] = impList[i];
            }
            j++;
        }

        myfree(pointer);
        return newHeader;

    }

    // Explicit free list realloc
    else {

        int newHeader = myalloc(size);
        int j = 0;

        for(int i = pointer; i < ((impList[pointer] - 1) / 4) - 1 + pointer; i++){
            if(j != 0){
                impList[newHeader + j] = impList[i];
            }
            j++;
        }

        myfree(pointer);
        return newHeader;

    }


    return pointer;
}

// Grows size of simulated heap
// End simulation of size passes 100,000 words
int mysbrk(int size){
    if(implicit){
        
        // Check to see if new size will be larger than 100,000
        if((impSize + size) > 100000){
            std::cout << "Size of list has exceeded 100,000.\nSimulation will now halt." << std::endl;
            exit(1);
        }

        impList[impSize - 1] = 0;
        int *temp = new int[size];

        for(int i = 0; i < impSize; i++){
            temp[i] = impList[i];
        }

        delete impList;
        impList = temp;
        impSize = size;
        impList[impSize - 1] = 1;
        return size;
    }

    if(!implicit){


        
    }
    
    return -1;
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


    impList[1] = 998 * 4;
    impList[impSize - 2] = 998 * 4;
    impList[0] = 1;
    impList[impSize - 1] = 1;
    if(!implicit){
        impList[2] = -1;
        impList[3] = -1;
    }

    // Check to make sure file was opened
    if(inputFile == NULL){
        std::cout << "Error opening file, check your syntax" << std::endl;
        printUseage();
        return -1;
    }

    char* lineptr = NULL;
    size_t n = 0;

    while(getline(&lineptr, &n, inputFile) != -1){

        char* dup = strdup(lineptr);

        // malloc case
        if(dup[0] == 'a'){
            int size = 0;
            int pointer = 0;
            sscanf(dup, "a, %d, %d\n", &size, &pointer);
            if(size > 4000){
                std::cout << "total heap capacity reached! (100000 words)" << std::endl;
                return -1;
            }
            ptrsArr[pointer] = myalloc(size);
        }

        // free case
        if(dup[0] == 'f'){
            int pointer = 0;
            sscanf(dup, "f, %d\n", &pointer);
            myfree(pointer);
        }

        // realloc case
        if(dup[0] == 'r'){
            int size = 0;
            int block = 0;
            int newBlock = 0;
            int pointer = 0;
            sscanf(dup, "r, %d, %d, %d\n", &size, &block, &newBlock);

            // Look up block to cross refrence
            ptrsArr[newBlock] = myrealloc(block, size);
        }

        free(dup);
        
    }    



    // Print data structure
    for(int i = 0; i < impSize; i++){
        outputFile << std::dec << i << ", ";
        if(impList[i] == -1){
            outputFile << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << static_cast<int>(0) << std::endl;
        }
        else if(impList[i] != 0) 
            outputFile << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << static_cast<int>(impList[i]) << std::endl;

        else
            outputFile << std::endl;
    }

    // Cleanup
    fclose(inputFile);
    outputFile.close();
    free(lineptr);
    free(impList);

    return 0;
}