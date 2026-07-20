#include "../shared/includes.h"

typedef struct pageFrame
{
    int virtualPage;
    bool referenced;
    size_t age;
} pageFrame;

//? We dont have interrupts so we are implementing it by conting references
static const reclamationAgeThreshold = 50;
size_t currentAge = 0;

int pageFrameCount = -1;

int main(int argc, char const *argv[])
{

    //? Parse page frames and allocate them
    errno = 0;
    char *end = NULL;
    long parsedPageFrameCount = strtol(argv[1], &end, 10);

    if (end == argv[1])
    {
        printf("Could not parse page_frame_count \n");
        return 1;
    }
    if (*end != '\0')
    {
        printf("Could not parse page_frame_count \n");
        return 1;
    }
    if ((errno == ERANGE && parsedPageFrameCount == 0))
    {
        printf("Could not parse page_frame_count (too small) \n");
        return 1;
    }
    if ((errno == ERANGE && parsedPageFrameCount == INT_MAX))
    {
        printf("Could not parse page_frame_count (too big) \n");
        return 1;
    }
    if (parsedPageFrameCount < 1 || parsedPageFrameCount > INT_MAX)
    {
        printf("Could not parse page_frame_count (must be at least 1) \n");
        return 1;
    }

    pageFrameCount = (int)parsedPageFrameCount;

    FILE *fptr;
    fptr = fopen(PAGE_REFERENCES_PATH, "r");

    if (fptr == NULL)
    {
        printf("Could not open page references txt file");
        return 1;
    }

    while (!feof(fptr))
    {

    }

    fclose(fptr);
    return 0;
}
