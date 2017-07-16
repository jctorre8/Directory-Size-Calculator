////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS
#define MAX_FILENAME_LENGTH 264
#define MAX_ELEMENTS 264
#define MAX_PATH_LENGTH 1024

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef struct file {
	char name[MAX_FILENAME_LENGTH];
	int size;
	struct stat fileStat;
} file;

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);


//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      
int main() {
	DIR *dir;
    struct dirent *dp;
    char * file_name;
    dir = opendir(".");
    while ((dp=readdir(dir)) != NULL) {
        //printf("debug: %s\n", dp->d_name);
        if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
        {
            // do nothing (straight logic)
        } else {
            file_name = dp->d_name; // use it
            printf("file_name: \"%s\"\n",file_name);
        }
    }
    closedir(dir);
    return 0;
}



// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
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
        if (L[i] <= R[j])
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
void mergeSort(int arr[], int l, int r){
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