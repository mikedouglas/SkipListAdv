#include <sstream>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cmath>

#include "Driver.h"
#include "skipList.h"

#define LINES_PER_PAGE 40.0

using namespace std;

int main(int argc, char *argv[])
{
        skipList *index = skipCreate();
        int lines = 0;
        string word, line;
        
        ifstream inFile;
        inFile.open(argv[1]);

        if (inFile.is_open()) {
                while (!inFile.eof()) {
                        getline(inFile, line);
                        lines++;

                        istringstream iss(line, istringstream::in);
                        while(iss >> word) {
                                trim(&word);
                                
                                string::iterator it;
                                for (it = word.begin();
                                     it != word.end(); it++) {
                                        if(isalpha(*it) && isupper(*it))
                                                   (*it) = tolower((*it));
                                }

                                if (word.begin() != word.end())
                                        skipInsert(index, word.c_str(), getpage(lines));
                        }
                }
                inFile.close();
        }
        std::ofstream out(argv[2]);
        skipPrint(out, index);
}

void trim(string *word) 
{
        string::iterator it;
        for (it = (*word).begin(); it != (*word).end(); it++) {
                if (isalpha(*it)) {
                        (*word).erase((*word).begin(), it);
                        break;
                }
                if (it == (*word).end()) {
                        (*word).clear();
                        break;
                }
        }
        
        for (it = (*word).end() - 1;; it--) {
                if (isalpha(*it)) {
                        (*word).erase(it+1, (*word).end());
                        break;
                }
                if (it == (*word).begin()) {
                        (*word).clear();
                        break;
                }
        }
}


int getpage(int lines)
{
        return (ceil(lines/LINES_PER_PAGE));
}
