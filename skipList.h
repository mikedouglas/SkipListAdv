#ifndef __SKIPLIST_H_
#define __SKIPLIST_H_

#include <set>
#include <iostream>
using namespace std;

#define MAX_HEIGHT 16

typedef struct skipNode {
        char *key;
        int instances;
        int height;
        set<int> pages;
        struct skipNode *next[MAX_HEIGHT];
} skipNode;

typedef skipNode skipList; // Always points to the head

skipList *skipCreate();
void skipFree(skipList *list);
int skipSearch(skipList *list, const char *key);
void skipInsert(skipList *list, const char *key, int page);
void skipPrint(ostream& out, skipList *list);
int skipGenHeight(int curMaxHeight);

#endif
