#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define EMPTY_FRAME -1

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, size_t page_max);

int _contains(int* arr, size_t sz, int target)
{
    int i = 0;

    for (i = 0; i < sz; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }

    return -1;
}

int _min(int* arr, size_t sz) 
{
    int i;
    int min = INT_MAX;

    for (i = 0; i < sz; i++) 
    {
        if (arr[i] < min) 
        {
            min = arr[i];
        }
    }

    return min;
}

// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
    if (argc < 4) return -1;
    
    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

    int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int page_faults = lru(ref, ref_arr_sz, frame_sz, page_max);
    printf("\nPage Fault : %d\n", page_faults);
    free(ref);

    // int ref_S[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    // int sz = sizeof(ref_S) / sizeof(int);
    // lru(ref_S, sz, 4, page_max);

    return 0;
}

int* generate_ref_arr(size_t sz, size_t page_max) {
    int* ref_arr = (int*) malloc(sizeof(int) * sz);
    
    for (int i = 0; i < sz; i++)
    {
        ref_arr[i] = rand()%(page_max);
    }

    return ref_arr;
}

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, size_t page_max) {
    int i, j;
    int page_faults = 0;

    int k, l;
    int is_fault;
    int target;
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) frames[i] = -1;

    int* refBit = (int*) malloc(sizeof(int) * (page_max+1));
    for (i=0; i<(page_max+1); i++) refBit[i] = 0;

    for (i = 0; i < ref_arr_sz; i++)
    {
        is_fault = _contains(frames, frame_sz, ref_arr[i]);

        if (is_fault != -1)
        {
            for (j = 0; j < (page_max+1); j++)
            {
                refBit[j] = refBit[j]/2;
            }

            refBit[ref_arr[i]] += 128;
        }

        else
        {
            int empty_idx = _contains(frames, frame_sz, EMPTY_FRAME);

            if (empty_idx == EMPTY_FRAME)
            {
                int* temp = (int*) malloc(sizeof(int) * frame_sz);

                for (k = 0; k < frame_sz; k++) 
                {
                    temp[k] = refBit[frames[k]];
                }

                int min = _min(temp, frame_sz);
                target = _contains(refBit, (page_max+1), min);
                target = _contains(frames, frame_sz, target);
            }

            else
            {
                target = empty_idx;
            }

            frames[target] = ref_arr[i];
            page_faults++;

            for (j = 0; j < (page_max+1); j++)
            {
                refBit[j] = refBit[j]/2;
            }

            refBit[ref_arr[i]] += 128;
        }


        printf("%d | ", ref_arr[i]);

        for (l = 0; l < frame_sz; l++)
        {
            if (frames[l] == -1)
            {
                printf(". ");
            }

            else
            {
                printf("%d ", frames[l]);
            }
        }
        
        if (is_fault == -1)
        {
            printf("(fault)");
        }

        printf("\n");
    }

    return page_faults;
}

