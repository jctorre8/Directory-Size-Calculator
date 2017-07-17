////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h>
#include <dirent.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS
#define MAX_FILENAME_LENGTH 264
#define MAX_ELEMENTS 264
#define MAX_PATH_LENGTH 1024

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef struct File {
	char name[MAX_FILENAME_LENGTH];
	struct stat fileStat;
} File;

File *files[MAX_ELEMENTS];

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

char slash[] = "/";

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void merge(File * arr[], int l, int m, int r);
void mergeSort(File * arr[], int l, int r);
int listDirectory(char *path, int *index);
void cleanUp(int index);
void printSpace(long space);
void printFiles(int index);

//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      
int main() {
    char initialPath[MAX_PATH_LENGTH] = ".";
    int index = 0;
    int totalSize = 0;

    //size calculation
    totalSize = listDirectory(initialPath, &index);

    //Array so it is in order
    mergeSort(files,0,index-1);

    //print files in nice table
    printFiles(index);
    printSpace(totalSize);
    printf("\t\t.\n");

    //Clean everything up
	cleanUp(index);
    return 0;
}

//Recursive function that does through directories looking
//for the files to be added to the size calculation
int listDirectory(char *path, int *index){
    DIR *dir;
    struct dirent *dp;
    dir = opendir(path);

    //return 0 if it fails to open a directory
    if(dir == NULL){
        printf("Failed to open directory: %s\n",path);
        return 0;
    }

    int runningTotal = 0;

    while ((dp=readdir(dir)) != NULL) {
        //check for current directory and previous directory
        if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
            continue;
        }

        //Alllocate new file in array and get info
        files[*index] = (File *) malloc(sizeof(File));        
        char newPath[MAX_PATH_LENGTH] = "";
        strcat(newPath, path);
        strcat(newPath, slash);
        strcat(newPath, dp->d_name);
        strcpy(files[*index]->name, newPath);
        stat(files[*index]->name, &files[*index]->fileStat);

        if (dp->d_type == DT_DIR){
            *index += 1;
            runningTotal += listDirectory(newPath, index);
        } else  {
            runningTotal += files[*index]->fileStat.st_size;
            *index += 1;
        }
        
    }
    closedir(dir);

    return runningTotal;
}

//Function to free the arrays.
void cleanUp(int index){
    int i;

    for(i = 0; i < index; i++){
        //printf("Current Cleanup Index: %d\n", i);
        free(files[i]);
    }

}

//Helper function to determine the magnitude of the space
// that the file is taking at the moment.
void printSpace(long space){
    if(space < 1024){
        printf("%ldB", space);
    }
    else if (space >= 1024 && space < 1048576){
        printf("%.1fK", space/1024.0);
    }
    else{
        printf("%.1fMB", space/1048576.0);   
    }

}

//Helper function to print files and not directories
void printFiles(int index){
    int i;
    for (i = 0; i < index; ++i){
        if(S_ISDIR(files[i]->fileStat.st_mode)){
            continue;
        }
        printSpace(files[i]->fileStat.st_size);
        printf("\t\t%s\n", files[i]->name);
    }
}


// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(File * arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    File * L[n1], * R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (strcmp(L[i]->name,R[j]->name) <= 0)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(File * arr[], int l, int r){
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
 
        merge(arr, l, m, r);
    }
}