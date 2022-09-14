#include "psort.h"
#include <bits/stdc++.h>
void swap(uint32_t *xp, uint32_t *yp) 
{ 
    uint32_t temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// A function to implement bubble sort 
void bubbleSort(uint32_t *arr, uint32_t n)
{ 
    uint32_t i, j; 
    for(i = 0; i < n-1; i++){   
        for (j = 0; j < n-i-1; j++) 
            if (arr[j] > arr[j+1]) 
                swap(&arr[j], &arr[j+1]); 
    }
}

void split(uint32_t *B, int i, uint32_t *count, uint32_t *S, uint32_t *d,int p, uint32_t n){
    if(i !=0 && i != p-1){
        for(uint32_t j=0; j < n; j++){
            if((d[j] > S[i-1]) && (d[j] <= S[i])){
                B[count[i]] = d[j];
                count[i] +=1;
            }
        }
    }
    else if(i ==0){
        for(uint32_t j=0; j < n; j++){
            if(d[j] <= S[i]){
                B[count[i]] = d[j];
                count[i] +=1;
            }
        }
    }
    else if(i == p-1){
        for(uint32_t j=0; j < n; j++){
            if(d[j] > S[i-1]){
                B[count[i]] = d[j];
                count[i] +=1;
            }
        }
    }
}

void propersort(uint32_t *B, int i, uint32_t *count, uint32_t threshold,int p){
    if(count[i] > threshold){
        ParallelSort(B,count[i],p);
    }
    else{
        bubbleSort(B,count[i]);
    }
}
void ParallelSort(uint32_t *data, uint32_t n, int p)
{
    // Entry point to your sorting implementation.
    // Sorted array should be present at location pointed to by data
    int req = n/p;
    uint32_t threshold = 2*(n/p);
    uint32_t *R = new uint32_t[p*p];
    for(uint32_t i = 0; i < n; i+= req){
        for(int j= 0 ; j < p; j++){
            if(((i/req)*p)+j >= p*p){
                break;
            }
            R[((i/req)*p)+j] = data[i+j];
        }
    }
    //std::cout << "Hello1" << std::endl;
    bubbleSort(R,p*p);
    //std::cout <<"afterbubble" << std::endl;
    //std::cout << p<< std::endl;
    //for(int i= 0; i< p*p;i++){
        //std::cout << R[i] << " ";
    //}
    uint32_t *S = new uint32_t[p-1];
    //std::cout << "1.1" << std::endl;
    for(int j =0;j<p-1; j++){S[j] = R[(j+1)*p];}
    //std::cout << "1.2" << std::endl;
    uint32_t *B[p];
    //std::cout << "1.3" << std::endl;
    uint32_t *count = new uint32_t[p];
    //std::cout << "1.4" << std::endl;
    for(int i= 0; i< p; i++){
        count[i]= 0;
        B[i] = new uint32_t[n];
    }
    //std::cout << "Hello2" << std::endl;
    for(int i = 0; i<p;i++){
        #pragma omp task
        {
            split(B[i],i,count,S,data,p,n);
        }
    }
    #pragma omp taskwait

    //std::cout << "Hello3"<< std::endl;

    for(int i =0; i<p;i++){
        #pragma omp task
        {
            propersort(B[i],i,count,threshold,p);
        }
    }
    #pragma omp taskwait

    
    uint32_t temp= 0;
    for(int i= 0; i< p; i++){
        for(uint32_t j= 0; j< count[i];j++){
            data[temp+j] = B[i][j];
        }
        temp+= count[i];
    }
}