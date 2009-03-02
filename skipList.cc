#include <iostream>
#include <cstdlib>
#include <cstring>
#include <set>

#include "skipList.h"
#include "randBit.h"

/*
 * returns a new skipList
 */
skipList *skipCreate()
{
        skipList *head = new skipList;
        head->key = NULL;
        head->height = 0;

        return head;
}

/*
 * returns a new node
 */
skipNode *skipNodeCreate(int height, const char *key)
{
        skipNode *node = new skipNode;
        node->height = height;
        node->key = (char*)malloc(sizeof(char)*strlen(key));
        strcpy(node->key, key);
        node->instances = 1;

        return node;
}

void skipFree(skipList *list)
{
        for (skipNode *node = list->next[0]; node != NULL; node = node->next[0]) {
                delete node->key;
                skipNode *nextNode = node->next[0];
                delete node;
                node = nextNode;
        }
}

/*
 * searches list for key, and returns either the number of instances
 * or -1.
 */
int skipSearch(skipList *list, const char *key)
{
        skipNode *node = (skipNode*)list;

        for (int i = node->height; (i > 0) && node->next[i] != NULL; i--)
                while (strcmp(node->next[i]->key, key) > 0)
                        node = node->next[i];

        node = node->next[1];
        if ((node != NULL) && (strcmp(node->key, key) == 0))
                return node->instances;
        else
                return -1;
}

/*
 * adds 'key' at 'page' to list.
 * based off the algorithm described in Pugh90.
 */
void skipInsert(skipList *list, const char *key, int page)
{
        skipNode *node = (skipNode*)list;
        skipNode *update[list->height+1];

        /* from the top, traverse downwards until proper place for 'key'
           found. keep track of the last visible node for each height */
        for (int i = node->height; (i >= 0) && (node != NULL); i--) {
                while ((node->next[i] != NULL)
                       && (strcmp(node->next[i]->key, key) < 0))
                        node = node->next[i];
                update[i] = node;
        }

        // if 'key' isn't a unique node
        if ((node->next[0] != NULL) && (strcmp(node->next[0]->key, key) == 0)) {
                node->next[0]->pages.insert(page);
                node->next[0]->instances++;
        }
        else { // if it is
                int height = skipGenHeight(list->height);
                if (height > list->height) {
                        // simple, thanks to opt. in skipGenHeight()
                        update[list->height] = (skipNode*)list;
                        list->height = height;
                }

                node = skipNodeCreate(height, key);
                node->pages.insert(page);
                for (int i = 0; i < height; i++) {
                        /* place new node inbetween update[i] and
                           update[i]->next[i] */
                        node->next[i] = update[i]->next[i];
                        update[i]->next[i] = node;
                }
        }
}

/* 
 * return a random height between (1..curMaxLevel+1) or
 * (1..MAX_LEVEL), whichever is smaller.
 * implements the `fix the dice` recommendation of Pugh90.
 */
int skipGenHeight(int curMaxLevel)
{
        int i;
        for (i = 1; (i <= curMaxLevel) && (i <= MAX_HEIGHT); i++)
                if (randBit() == 1)
                        break;
        return i;
}

/*
 * prints nodes in sorted order, collapsing consecutive page ranges.
 */
void skipPrint(std::ostream& out, skipList *list)
{
        for (skipNode *node = list->next[0]; node != NULL; node = node->next[0]) {
                out << node->key << " (" << node->instances
                          << ") ";
                set<int>::iterator it, it2;
                for (it = node->pages.begin(); it != node->pages.end();) {
                        it2 = it;
                        int i = 0;

                        /* while (it..it2) contains a consecutive range,
                           increase it2. */
                        while((it2 != node->pages.end())
                              && ((*it2) - (*it)) == i) {
                                i++;
                                it2++;
                        }
                        it2--; // decrease to capture valid range
                        
                        if (i > 1)
                                out << (*it) << "-" << (*it2);
                        else
                                out << (*it);
                        it = it2;

                        /* increment moved here to ensure proper placement
                           of the comma */
                        if (++it != node->pages.end())
                                out << ",";
                }
                out << std::endl;
        }
}
