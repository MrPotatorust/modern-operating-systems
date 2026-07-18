#include "../shared/includes.h"

/*
    This implementation is not really efficient.
*/

static const int AGING_INTERVAL = 100;

size_t pageReferenceCount = 0;
int pageFrameCount = -1;

pageFrame *pageFrames = NULL;
virtualPage *virtualPages = NULL;

int populateVirtualPages()
{
    if (virtualPages == NULL)
    {
        return 1;
    }

    for (size_t i = 0; i < virtualPageCount; ++i)
    {
        virtualPages[i].isPresent = false;
    }

    return 0;
}

int populatePageFrames()
{
    if (pageFrames == NULL)
    {
        return 1;
    }

    for (size_t i = 0; i < pageFrameCount; ++i)
    {
        pageFrames[i].age = 0;
        pageFrames[i].referenced = false;
        pageFrames[i].virtualPage = -1;
    }
    return 0;
}

void executeTick()
{
    for (size_t i = 0; i < pageFrameCount; ++i)
    {
        pageFrame *curPageFrame = &pageFrames[i];

        curPageFrame->age >>= 1;

        if (curPageFrame->referenced)
        {
            curPageFrame->age += 128;
            curPageFrame->referenced = false;
        }
    }
    pageReferenceCount = 0;
}

int main(int argc, char const *argv[])
{

    unsigned long long int totalPageFaults = 0; // Overkill is fun

    if (argc != 2)
    {
        printf("Incorrect usage, the correct usage is ./main {page_frame_count} \n");
        return 1;
    }

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

    pageFrames = malloc(pageFrameCount * sizeof(pageFrame));
    virtualPages = malloc(virtualPageCount * sizeof(virtualPage));

    //? Populate virtual pages
    if (populateVirtualPages() > 0)
    {
        printf("Could not allocate virtual pages \n");
        return 1;
    }

    //? Populate pageFrames
    if (populatePageFrames(pageFrameCount) > 0)
    {
        printf("Could not allocate page frames \n");
        return 1;
    }

    //? Parse references from file and instantly run them
    FILE *fptr;
    fptr = fopen(PAGE_REFERENCES_PATH, "r");

    if (fptr == NULL)
    {
        printf("Could not open page references txt file");
        return 1;
    }

    fptr = fopen(PAGE_REFERENCES_PATH, "r");

    while (!feof(fptr))
    {
        int curVirtualPageOffset = 0; // Stores the current virtualPage read in from the file
        virtualPage *curVirtualPage;
        size_t oldestPageOffset = 0;
        __uint8_t oldestAge = 255;
        bool wasAssigned = false;

        //? Executes a tick every ~20ms (simulated) and runs aging
        if (pageReferenceCount == 100)
        {
            executeTick();
        }
        if (fscanf(fptr, "%d\n", &curVirtualPageOffset) != 1)
        {
            printf("Something went wrong when reading file, count not read value \n");
            return 1;
        };

        ++pageReferenceCount;

        curVirtualPage = &virtualPages[curVirtualPageOffset];

        if (curVirtualPage->isPresent)
        {
            pageFrames[curVirtualPage->assignedFrame].referenced = true;
            continue;
        }

        //? Try to assign the page to a free page frame if possible
        for (size_t i = 0; i < pageFrameCount; ++i)
        {
            pageFrame *curPageFrame = &pageFrames[i];

            //? Searches for a free page
            if (curPageFrame->virtualPage == -1)
            {
                curPageFrame->virtualPage = curVirtualPageOffset;
                curVirtualPage->assignedFrame = i;
                curVirtualPage->isPresent = true;
                curPageFrame->referenced = true;
                wasAssigned = true;
                ++totalPageFaults;
                break;
            }

            //? Runs aging
            else if (curPageFrame->age < oldestAge)
            {
                oldestPageOffset = i;
                oldestAge = curPageFrame->age;
            }
        }

        //? Resolves the page fault and replaces a page
        if (!wasAssigned)
        {
            printf("Page %d\n", curVirtualPageOffset);
            pageFrame *oldestPageFrame = &pageFrames[oldestPageOffset];

            //? Remove old page
            virtualPages[oldestPageFrame->virtualPage].isPresent = false;

            //? Assign new page
            oldestPageFrame->age = 0;
            oldestPageFrame->referenced = true;
            oldestPageFrame->virtualPage = curVirtualPageOffset;
            curVirtualPage->assignedFrame = oldestPageOffset;
            curVirtualPage->isPresent = true;
            ++totalPageFaults;
        }
    }

    printf("Total page faults: %llu\n", totalPageFaults);

    return 0;
}
