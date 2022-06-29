#include <SDL2/SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
#include <bits/stdc++.h>

using namespace std;

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;

const int screenWidth=1600;
const int screenHeight=900;    
const int arrSize=200;
const int rectSize=8;

int arr[arrSize];
int arrB[arrSize];

bool complete=false;

//intitializing the window and checking for some errors
bool init()
{
    bool working=true;
    window=SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if(window==NULL)
        {
        cout<<"Problem Creating window! "<<SDL_GetError();
        working=false;
        }
    else
    {
        renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer==NULL)
        {
            cout<<"Problem Creating window! "<<SDL_GetError();
            working=false;
        }
    }
    return working;
}

//closing out of the window
void close()
{
    SDL_DestroyRenderer(renderer);
    renderer=NULL;
    SDL_DestroyWindow(window);
    window=NULL;
    SDL_Quit();
}

//visualizing the data
void visualize(int x=-1, int y=-1, int z=-1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j=0;
    for(int i=0; i<=screenWidth-rectSize; i+=rectSize)
    {
        SDL_PumpEvents();
        
        SDL_Rect rect={i, screenHeight, rectSize, -(arr[j])};
        if(complete)
        {
            //when finished sorting set outline color to green
            SDL_SetRenderDrawColor(renderer, 50, 205, 50, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if(j==x || j==z)
        {
            //current value in the array is a dark green box
            SDL_SetRenderDrawColor(renderer, 50, 205, 50, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j==y)
        {
            //vakye that is going to be swapped is highlighted in purple
            SDL_SetRenderDrawColor(renderer, 180, 110, 200, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            //untouched bars stay red
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void selectionSort()
{
    int minIdx, i, j;
    //one by one move boundry of unsorted array
    for(int i=0;i<arrSize-1;i++)
    {
        minIdx=i;
        for(int j=i+1;j<arrSize;j++)
        {
            if(arr[j]<arr[minIdx])
            {
                minIdx=j;
                //visualize the change
                visualize(i, minIdx);
            }
            SDL_Delay(1);
        }
        //swap found min with first element
        swap(arr[minIdx], arr[i]);
    }
}

void insertionSort()
{
    int i, key, j;
    for (i = 1; i < arrSize; i++)
    {
        key = arr[i];
        j = i - 1;
 
        // Move elements of arr[0..i-1], that are greater than key, to oneposition ahead of their ncurrent position
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;

            visualize(i, j+1);
            SDL_Delay(1);
        }
        arr[j + 1] = key;
    }
}

void bubbleSort()
{
   int i, j;
   bool swapped;
   for (i = 0; i < arrSize-1; i++)
   {
     swapped = false;
     for (j = 0; j < arrSize-i-1; j++)
     {
        if (arr[j] > arr[j+1])
        {
           swap(arr[j], arr[j+1]);
           swapped = true;

           visualize(j+1, j, arrSize-i);
        }
        SDL_Delay(1);
     }
     // IF no two elements were swapped
     // by inner loop, then break
     if (swapped == false)
        break;
   }
}
 

void merge(int a[], int begin,int mid, int end)
{
    int size_output=(end-begin)+1;
    int* output=new int[size_output];
    int i=begin, j=mid+1, k=0;
    while(i<=mid && j<=end)
    {
        if(a[i]<=a[j])
        {
            output[k]=a[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k]=a[j];
            visualize(i, j);
            j++;
            k++;
        }
    }
    while(i<=mid)
    {
        output[k]=a[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while(j<=end)
    {
        output[k]=a[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x=0;
    for(int l=begin; l<=end; l++)
    {
        a[l]=output[x];
        visualize(l);
        SDL_Delay(5);
        x++;
    }
    delete []output;
}

void mergeSort(int array[], int begin, int end)
{
    if (begin >= end)
        return; // Returns recursively
 
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

int partition(int a[], int begin, int end)
{
    int count_small=0;

    for(int i=(begin+1);i<=end;i++)
    {
        if(a[i]<=a[begin])
        {
            count_small++;
        }
    }
    int c=begin+count_small;
    int temp=a[c];
    a[c]=a[begin];
    a[begin]=temp;
    visualize(c, begin);

    int i=begin, j=end;

    while(i<c && j>c)
    {
        if(a[i]<= a[c])
        {
            i++;
        }
        else if(a[j]>a[c])
        {
            j--;
        }
        else
        {
            int temp_1=a[j];
            a[j]=a[i];
            a[i]=temp_1;
            visualize(i, j);
            SDL_Delay(70);
            i++;
            j--;
        }
    }
    return c;
}

void quickSort(int array[], int begin, int end)
{
    if(begin>=end)
    {
        return;
    }
    int pivot=partition(array, begin, end);
    quickSort(array, begin, pivot-1);
    quickSort(array, pivot+1, end);

}

void inplaceHeapSort(int* input, int n)
{
    for(int i=1; i<n; i++)
    {
       int childIndex=i;
       int parentIndex=(childIndex-1)/2;

       while(childIndex>0)
       {
           if(input[childIndex]>input[parentIndex])
           {
               int temp=input[parentIndex];
               input[parentIndex]=input[childIndex];
               input[childIndex]=temp;

           }
           else
           {
               break;
           }

           visualize(parentIndex, childIndex);
           SDL_Delay(20);

           childIndex=parentIndex;
           parentIndex=(childIndex-1)/2;
       }
    }

    for(int heapLast=n-1; heapLast>=0 ; heapLast--)
    {
        int temp=input[0];
        input[0]=input[heapLast];
        input[heapLast]=temp;

        int parentIndex=0;
        int leftChildIndex=2*parentIndex + 1;
        int rightChildIndex=2*parentIndex + 2;

        while(leftChildIndex<heapLast)
        {
            int maxIndex=parentIndex;

            if(input[leftChildIndex]>input[maxIndex])
            {
                maxIndex=leftChildIndex;
            }
            if(rightChildIndex<heapLast && input[rightChildIndex]>input[maxIndex])
            {
                maxIndex=rightChildIndex;
            }
            if(maxIndex==parentIndex)
            {
                break;
            }

            int temp=input[parentIndex];
            input[parentIndex]=input[maxIndex];
            input[maxIndex]=temp;

            visualize(maxIndex, parentIndex, heapLast);
            SDL_Delay(20);

            parentIndex=maxIndex;
            leftChildIndex=2*parentIndex + 1;
            rightChildIndex=2*parentIndex + 2;
        }
    }
}

void CocktailSort()
{
    bool swapped = true;
    int start = 0;
    int end = arrSize - 1;
 
    while (swapped) {
        swapped = false;
 
        // loop from left to right same as a bubble sort
        for (int i = start; i < end; ++i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                visualize(arr[i],arr[i+1]);
                SDL_Delay(1);
                swapped = true;
            }
        }
 
        // if nothing moved, then array is sorted.
        if (!swapped)
            break;
 
        // otherwise, reset the swapped flag so that it
        swapped = false;
 
        // move the end point back by one
        --end;
 
        //same comparison as in the previous stage
        for (int i = end - 1; i >= start; --i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
                visualize(arr[i],arr[i+1]);
                SDL_Delay(1);
            }
        }
 
        // increase the starting point,
        ++start;
    }
}

void flip(int arr[], int i) 
{ 
    int temp, start = 0; 
    while (start < i) 
    { 
        temp = arr[start]; 
        arr[start] = arr[i]; 
        arr[i] = temp; 
        start++; 
        i--; 
    } 
} 
  
// Returns index of the maximum element 

int findMax(int arr[], int n) 
{ 
int mi, i; 
for (mi = 0, i = 0; i < n; ++i) 
    if (arr[i] > arr[mi]) 
            mi = i; 
return mi; 
} 
  
// The main function that sorts given array using flip operations 
void pancakeSort(int* arr, int n) 
{ 
    for (int curr_size = n; curr_size > 1; 
                               --curr_size) 
    { 
        // Find index of the maximum element 
        int mi = findMax(arr, curr_size); 
  
        // move max to end if not there already 
        if (mi != curr_size-1) 
        { 
            flip(arr, mi); 
            visualize(arr[0],mi);
            SDL_Delay(25);
  
            //move max to end by reversing
            flip(arr, curr_size-1); 
            visualize(arr[0],curr_size-1);
            SDL_Delay(25);
        } 
    } 
} 
 //comb sort stuff
 // To find gap between elements
int getNextGap(int gap)
{
    // Shrink gap by Shrink factor
    gap = (gap*10)/13;
  
    if (gap < 1)
        return 1;
    return gap;
}

void combSort()
{
    // Initialize gap
    int gap = arrSize;
  
    // Initialize swapped as true 
    bool swapped = true;
  
    // Keep running while gap is more than 1 
    while (gap != 1 || swapped == true)
    {
        // Find next gap
        gap = getNextGap(gap);
  
        // Initialize swapped as false
        swapped = false;
  
        // Compare all elements with current gap
        for (int i=0; i<arrSize-gap; i++)
        {
            if (arr[i] > arr[i+gap])
            {
                swap(arr[i], arr[i+gap]);
                visualize(arr[i],arr[i+gap]);
                SDL_Delay(15);
                swapped = true;
            }
        }
    }
} 



void loadArr()
{
    //memory coppying arr to arrb
    memcpy(arr, arrB, sizeof(int)*arrSize);
}

//randomizing the elements of the array
void randomizeArray()
{
    srand(time(NULL));
    for(int i=0; i<arrSize; i++)
    {
        arrB[i]=rand()%(screenHeight);
    }
}

void execute()
{
    if(!init())
    {
        cout<<"SDL Initialization Failed.\n";
    }
    else
    {
        randomizeArray();
        loadArr();

        SDL_Event e;
        bool quit=false;
        while(!quit)
        {
            while(SDL_PollEvent(&e)!=0)
            {
                if(e.type==SDL_QUIT)
                {
                    quit=true;
                    complete=false;
                }
                else if(e.type==SDL_KEYDOWN)
                {
                    switch(e.key.keysym.sym)
                    {
                        case(SDLK_q):
                            quit=true;
                            complete=false;
                            cout<<"\nEXITING VISUALIZER.\n";
                            break;
                        case(SDLK_0):
                            randomizeArray();
                            complete=false;
                            loadArr();
                            cout<<"\nRANDOM LIST GENERATED.\n";
                            break;
                        case(SDLK_1):
                            loadArr();
                            complete=false;
                            selectionSort();
                            complete=true;
                            cout<<"\nSELECTION SORT COMPLETE.\n";
                            break;
                        case(SDLK_2):
                            loadArr();
                            complete=false;
                            insertionSort();
                            complete=true;
                            cout<<"\nINSERTION SORT COMPLETE.\n";
                            break;
                        case(SDLK_3):
                            loadArr();
                            complete=false;
                            bubbleSort();
                            complete=true;
                            cout<<"\nBUBBLE SORT COMPLETE.\n";
                            break;
                        case(SDLK_4):
                            loadArr();
                            complete=false;
                            mergeSort(arr, 0, arrSize-1);
                            complete=true;
                            cout<<"\nMERGE SORT COMPLETE.\n";
                            break;
                        case(SDLK_5):
                            loadArr();
                            complete=false;
                            quickSort(arr, 0, arrSize-1);
                            complete=true;
                            cout<<"\nQUICK SORT COMPLETE.\n";
                            break;
                        case(SDLK_6):
                            loadArr();
                            complete=false;
                            inplaceHeapSort(arr, arrSize);
                            complete=true;
                            cout<<"\nHEAP SORT COMPLETE.\n";
                            break;
                        case(SDLK_7):
                            loadArr();
                            complete=false;
                            CocktailSort();
                            complete=true;
                            cout<<"\nCocktail SORT COMPLETE.\n";
                            break;
                        case(SDLK_8):
                            loadArr();
                            complete=false;
                            pancakeSort(arr,arrSize);
                            complete=true;
                            cout<<"\nPancake SORT COMPLETE.\n";
                            break;
                        case(SDLK_9):
                            loadArr();
                            complete=false;
                            combSort();
                            complete=true;
                            cout<<"\nComb SORT COMPLETE.\n";
                            break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout<<"Press enter to begin visualization\n" "Complexity is based on average.\n" << "Press [0] to Generate a diferent array.\n"
        <<"Press [1] to visualize Selection Sort Algorithm O(n^2).\n" << "Press [2] to visualize Insertion Sort Algorithm O(n^2).\n"
        <<"Press [3] to visualize Bubble Sort Algorithm O(n^2).\n" <<"Press [4] to visualize Merge Sort Algorithm O(nlog(n)).\n"
        <<"Press [5] to visualize Quick Sort Algorithm O(nlog(n)).\n" <<"Press [6] to visualize Heap Sort Algorithm O(nlog(n)).\n"
        <<"Press [7] to visualize Cocktail Sort Algorithm O(n^2).\n" <<"Press [8] to visualize Pancake Sort Algorithm O(n).\n"
        <<"Press [9] to visuzlize Comb Sort Algorithm O(n^2).\n" <<"Press [q] to exit out of Sorting Visualizer\n"
        <<"Type 'quit' or 'QUIT' to exit the program.";

    string str;
    getline(cin, str);
    if(str=="quit" or str=="QUIT")
    {
        return false;
    }
    return true;
}
int main(int argc, char* args[])
{
    while(1)
    {
        cout<<'\n';
        if(controls())
            execute();
        else
            break;
    }
    return 0;
}