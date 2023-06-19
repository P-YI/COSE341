#include <stdio.h>
#include <stdlib.h>

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz);

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

// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
    if (argc < 4) return -1;
    
    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

    int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int page_faults = lru(ref, ref_arr_sz, frame_sz);
    printf("\nPage Fault : %d\n", page_faults);
    free(ref);

    // int ref_S[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    // int sz = sizeof(ref_S) / sizeof(int);
    // lru(ref_S, sz, 4);

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

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz) {
    int i, j;
    int page_faults = 0;

    int k, cur;
    int is_fault;
    int target = 0;
    int topStack = 0;
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) frames[i] = -1;

    for (i = 0; i < ref_arr_sz; i++)
    {
        is_fault = _contains(frames, frame_sz, ref_arr[i]);

        if (is_fault != -1)
        {
            for(j = is_fault; j <= topStack-1; j++)
            {
                if (is_fault == topStack)
                    break;

                else if (is_fault < topStack)
                {
                    frames[j] = frames[j+1];
                }
            }

            frames[topStack] = ref_arr[i];
        }

        else
        {
            if (topStack < frame_sz && frames[frame_sz-1] == -1)
            {
                if (frames[topStack] != -1 && topStack < frame_sz-1)
                {
                    topStack++;
                }

                frames[target] = ref_arr[i];
                target = (target+1)%(frame_sz);
            }

            else
            {
                for (j = 0; j <= topStack-1; j++)
                {
                    frames[j] = frames[j+1];
                    cur = j+1;
                }

                frames[cur] = ref_arr[i];
            }

            page_faults++;
        }

        printf("%d | ", ref_arr[i]);

        for (k = 0; k < frame_sz; k++)
        {
            if (frames[k] == -1)
            {
                printf(". ");
            }

            else
            {
                printf("%d ", frames[k]);
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
