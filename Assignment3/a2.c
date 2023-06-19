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

int victim(int* frames, size_t frame_sz, int* refBit)
{
    int victim;
    int temp = 256;
    
    for (int i = 0; i < frame_sz; i++)
    {
        if (refBit[frames[i]] < temp)
        {
            temp = refBit[frames[i]];
            victim = i;
        }
    }

    return victim;
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

    int k;
    int is_fault;
    int target = 0;
    int first = 0;
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) frames[i] = -1;

    int* refBit = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) refBit[i] = 0;

    for (i = 0; i < ref_arr_sz; i++)
    {
        is_fault = _contains(frames, frame_sz, ref_arr[i]);

        if (is_fault != -1)
        {
            for (j = 0; j < frame_sz; j++)
            {
                if (j == frames[is_fault])
                {
                    refBit[j] = (refBit[j] >> 1) +128;
                }

                else
                {
                    refBit[j] = refBit[j] >> 1;
                }
            }
        }

        else
        {
            if (first >= frame_sz)
            {
                target = victim(frames, frame_sz, refBit);
                frames[target] = ref_arr[i];

                for (j = 0; j < frame_sz; j++)
                {
                    if (j == frames[target])
                    {
                        refBit[j] = (refBit[j] >> 1) + 128;
                    }

                    else
                    {
                        refBit[j] = refBit[j] >> 1;
                    }
                }
            }

            else
            {
                frames[target] = ref_arr[i];

                for (j = 0; j < frame_sz; j++)
                {
                    if (j == frames[target])
                    {
                        refBit[j] = (refBit[j] >> 1) + 128;
                    }

                    else
                    {
                        refBit[j] = refBit[j] >> 1;
                    }
                }

                target = (target+1)%frame_sz;
                first++;
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

