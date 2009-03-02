#include <iostream>
#include <cstdlib>
#include <cstring>

#include "skipList.h"
#include "randBit.h"

skipList *skipCreate()
{
        skipList *head = (skipList*)malloc(sizeof(skipNode));
        head->key = NULL;
        head->height = 0;

        return head;
}

skipNode *skipNodeCreate(int height, const char *key)
{
        skipNode *node = (skipNode*)malloc(sizeof(skipNode));
        node->height = height;
        node->key = (char*)malloc(sizeof(char)*strlen(key));
        strcpy(node->key, key);
        node->instances = 1;

        return node;
}

void skipFree(skipList *list)
{
        for (skipNode *node = list->next[0]; node != NULL; node = node->next[0]) {
                free(node->key);
                skipNode *nextNode = node->next[0];
                free(node);
                node = nextNode;
        }
}

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

void skipInsert(skipList *list, const char *key, int page)
{
        skipNode *node = (skipNode*)list;
        skipNode *update[list->height+1];

        for (int i = node->height; (i >= 0) && (node != NULL); i--) {
                while ((node->next[i] != NULL)
                       && (strcmp(node->next[i]->key, key) < 0))
                        node = node->next[i];
                update[i] = node;
        }

        if ((node->next[0] != NULL) && (strcmp(node->next[0]->key, key) == 0))
                node->next[0]->instances++;
        else {
                int height = skipGenHeight(list->height);
                if (height > list->height) {
                        update[list->height] = (skipNode*)list;
                        list->height = height;
                }

                node = skipNodeCreate(height, key);
                for (int i = 0; i < height; i++) {
                        node->next[i] = update[i]->next[i];
                        update[i]->next[i] = node;
                }
        }
}

/* 
 * return a random height between (1..curMaxLevel+1) or
 * (1..MAX_LEVEL), whichever is smaller.
 * implements the `fix the dice` recommendation of Pugh1990.
 */
int skipGenHeight(int curMaxLevel)
{
        int i;
        for (i = 1; (i <= curMaxLevel) && (i <= MAX_HEIGHT); i++)
                if (randBit() == 1)
                        break;
        return i;
}

void skipPrint(skipList *list)
{
        for (skipNode *node = list->next[0]; node != NULL; node = node->next[0]) {
                std::cout << node->key << " (" << node->instances
                          << ")" << std::endl;
        }
}
