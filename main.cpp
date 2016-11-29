#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "B_Tree.h"
using namespace std;


int main(int argc, char * argv[])
{
    ifstream inputFile;
    ifstream inputFile2;
    string readLine;
    string command;
    string delimiter;
    string filename;

    inputFile.open( argv[1] );
    
    if  (!inputFile)
    {
        cout << "ERROR: file did not open" << endl;
    }
    
    else
    {
        inputFile >> command;
        if (command == "L:")
        {
            inputFile >> filename;
            //open data file
            inputFile2.open(filename);
            
            if  (!inputFile2)
            {
                cout << "ERROR: data file did not open" << endl;
            }
            else
            {
                //data opened successfully
                BTree<int, 5> testTree;
                int key;
                //insert all the data into the tree
                while (inputFile2 >> key)
                {
                    testTree.insert(key);
                }
                while (getline (inputFile, readLine))
                {
                    string newCommand;
                    stringstream ss(readLine);
                    ss >> newCommand;
                    char command1 = newCommand[0];
                    switch (command1)
                    {
                        case 'A':
                            ss >> key;
                            testTree.insert(key);
                            break;

                        case 'D':
                            ss >> key;
                            testTree.remove(key);
                            break;

                        case 'S':
                            ss >> key;
                            testTree.find(key);
                            break;

                        case 'P':
                            testTree.print();
                            break;
                            
                        case 'T':
                            break;
                            
                        default:
                            break;
                    }

                    
                    cout << endl;
                }
            }
            
        }
        else
        {
            cout << "Load the file first!" << endl;
        }
        
    }
    
    return 0;
}
