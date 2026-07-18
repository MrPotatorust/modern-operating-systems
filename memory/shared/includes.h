#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

typedef struct pageFrame
{
    int virtualPage;
    bool referenced;
    __uint8_t age;
} pageFrame;

typedef struct virtualPage
{
    bool isPresent;
    size_t assignedFrame;
} virtualPage;

static const size_t virtualPageCount = 256; // 0-255;
static const char PAGE_REFERENCES_PATH[] = "../shared/page_references.txt";