//
//  B_Tree.h
//  Assignment_03
//
//  Created by Henry Cho on 11/15/16.
//  Copyright © 2016 Henry Cho. All rights reserved.
//

#ifndef B_Tree_h
#define B_Tree_h
#include <math.h>


const int INSERT_INCOMPLETE = 1;
const int DUPLICATE = 2;
const int INSERT_SUCCESS = 3;
const int NOT_FOUND = 4;
const int DELETE_SUCCESS = 5;
const int _UNDERFLOW = 6;


template<typename T, int M>
class BNode
{
private:
    int keyCount;   //tracks how many keys are in the array
    BNode **childPtr;  //pointer to array of pointers
    T *keys;        //pointer to the array of keys
    
public:
    BNode();
    ~BNode();
    BNode(const T&);

    template<typename A, int B>friend class BTree;
};


template<typename T, int M>
class BTree
{
private:
    BNode<T, M> *root;
    int _insert(BNode<T,M> *&, T&, BNode<T,M> *&, T&);
    int _remove(BNode<T,M> *&, T&);
    void _print(BNode<T,M> *node, int);    //Level-order Traversal
    
public:
    BTree();
    ~BTree();
    void insert (T);
    void remove(T);
    void print();
    bool find(T);
    

};

//constructor for BNode
template<typename T, int M>
BNode<T, M>::BNode()
{
    keyCount = 0;
    keys = new int[M];
    childPtr = new BNode *[M];
    
    //set all the childPtr to NULL
    for (int i = 0; i < M+1; i++)
    {
        childPtr[i] = NULL;
    }
}

template<typename T, int M>
BNode<T, M>::BNode(const T& key)
{
    keyCount = 0;
    keys = new int[M];
    childPtr = new BNode *[M];
    
    //set all the childPtr to NULL
    for (int i = 0; i < M+1; i++)
    {
        childPtr[i] = NULL;
    }
    
    keys[0] = key;
}

template<typename T, int M>
BNode<T, M>::~BNode()
{
    delete[] childPtr;
    childPtr = NULL;
    delete[] keys;
    keys = NULL;
}

template<typename T, int M>
BTree<T, M>::BTree()
{
    root = NULL;
}

template<typename T, int M>
BTree<T, M>::~BTree()
{
    while (root != NULL)
    {
        remove(root->keys[0]);
    }
    delete root;
}



template<typename T, int M>
void BTree<T, M>:: insert(T elem)
{
    BNode<T, M> *outPtr;
    T outElem;
    bool fullRoot = false;          //check if the root is full
    if (root != NULL)
    {
        if (root->keyCount > M-2)
        {
            fullRoot = true;
        }
    }
    int return_code = _insert(root, elem, outPtr, outElem);
    if (return_code == INSERT_INCOMPLETE)
    {
        if (root == NULL) //if the root is NULL create a new BNode
        {
            root = new BNode<T, M>;
            root->keys[0] = outElem;
            root->keyCount++;
        }
        
        else
        {
            //if the root is full, split the old root and create a new root
            if (fullRoot == true)
            {
                BNode<T, M> *newRoot = new BNode<T, M>;
                newRoot->keys[0] = outElem;
                newRoot->childPtr[0] = root;
                newRoot->childPtr[1] = outPtr;
                newRoot->keyCount++;
                root = newRoot;
                newRoot = NULL;
            }
            
            else
            {
                int insertIndex = 0;
                //Place index to where it needs to be inserted
                while ((insertIndex < root->keyCount) && (outElem > root->keys[insertIndex]))
                {
                    insertIndex++;
                }
                
                //If there already key in the index where the insertion
                //needs to be made, shift the array down by one to make room
                if (insertIndex < root->keyCount)
                {
                    for (int i = root->keyCount; i > insertIndex; i--)
                    {
                        root->keys[i] = root->keys[i-1];
                        root->childPtr[i+1] = root->childPtr[i];
                    }
                }
                //make the insertion
                root->keys[insertIndex] = outElem;
                root->childPtr[insertIndex+1] = outPtr;
                
                //increase the keyCount
                root->keyCount++;
                
                //check if the array has exceeded the maximum # of keys
                if (root->keyCount > M-1)
                {
                    //split node
                    int median = M/2;
                    BNode<T, M> *newNode = new BNode<T, M>;
                    int i;
                    for (i = median + 1; i < M; i++)
                    {
                        //redistribute the full array
                        //copy both keys and child pointers
                        newNode->keys[i-median-1] = root->keys[i];
                        newNode->childPtr[i-median-1] = root->childPtr[i];
                        root->keyCount--;
                        newNode->keyCount++;
                    }
                    newNode->childPtr[i-median-1] = root->childPtr[i];
                    
                    BNode<T, M> *newRoot = new BNode<T, M>;
                    newRoot->keys[0] = root->keys[median];
                    newRoot->childPtr[0] = root;
                    newRoot->childPtr[1] = newNode;
                    root->keyCount--;     //decrease keyCount one more time to account for the median
                    root = newRoot;
                    newNode = NULL;
                    newRoot = NULL;
                }
            }

        }
    }
    else if (return_code == DUPLICATE)
    {
        std::cout << elem << "<----- This is a duplicate" << std::endl;
        
    }
    
    else
    {
        //insertion successful
        //Do nothing
    }


}

template<typename T, int M>
int BTree<T, M>:: _insert(BNode<T,M> *&node, T& key, BNode<T,M> *& outP, T&outKey)
{
    BNode<T, M> *t_outptr;
    T t_outKey;
    if (node == NULL)
    {
        outP = NULL;
        outKey = key;
        return INSERT_INCOMPLETE; //
    }
    
    int index = 0;
    //searching for the child pointer to go into
    for (int i = 0; i < node->keyCount; i++)
    {
        //places the index to the pointer that needs to go into
        if (key > node->keys[i])
        {
            index++;
        }
        
        else if (key == node->keys[i])
        {
            return DUPLICATE;   //duplicate found
        }
        
        else
        {
            //if the inserting key is less than the comparing key, then do nothing
        }
    }
    
    int results =_insert(node->childPtr[index], key, t_outptr, t_outKey);
    
    if (results == INSERT_INCOMPLETE)
    {
        
        int insertIndex = 0;
        //Place index to where it needs to be inserted
        while ((insertIndex < node->keyCount) && (t_outKey > node->keys[insertIndex]))
        {
            insertIndex++;
        }
        
        //If there already key in the index where the insertion
        //needs to be made, shift the array down by one to make room
        if (insertIndex < node->keyCount)
        {
            for (int i = node->keyCount; i > insertIndex; i--)
            {
                node->keys[i] = node->keys[i-1];
                node->childPtr[i+1] = node->childPtr[i];
            }
        }
        //make the insertion
        node->keys[insertIndex] = t_outKey;
        node->childPtr[insertIndex+1] = t_outptr;
        
        //increase the keyCount
        node->keyCount++;
        
        //check if the array has exceeded the maximum # of keys
        if (node->keyCount > M-1)
        {
            //split node
            int median = M/2;
            BNode<T, M> *newNode = new BNode<T, M>;
            int i;
            for (i = median + 1; i < M; i++)
            {
                //redistribute the full array
                //copy both keys and child pointers
                newNode->keys[i-median-1] = node->keys[i];
                newNode->childPtr[i-median-1] = node->childPtr[i];
                node->keyCount--;
                newNode->keyCount++;
            }
            newNode->childPtr[i-median-1] = node->childPtr[i];
            //set output parameter
            outP = newNode;
            outKey = node->keys[median];
            node->keyCount--;     //decrease keyCount one more time to account for the median
            
            newNode = NULL;
            //insert the median into the parent node
            return INSERT_INCOMPLETE;
        }
        
        else
        {
            //insertion successful
            return INSERT_SUCCESS;
        }
        
    }
    
    else if (results == DUPLICATE)
    {
        //found duplicate
        return DUPLICATE;
    }
    
    else
    {
        //insertion successful;
        return INSERT_SUCCESS;
    }
    
}



template<typename T, int M>
void BTree<T, M>::remove(T elem)
{
    if (root == NULL)
    {
        std::cout << std::endl;
        std::cout << "CANNOT DELETE: TREE IS EMPTY" << std::endl;
        return;
    }
    int return_code = _remove(root, elem);
    if (return_code == _UNDERFLOW)
    {
        BNode<T, M> *leftPtr = root->childPtr[0];
        BNode<T, M> *rightPtr = root->childPtr[1];
        double doubleM(M);
        double minimumKeys = ceil(doubleM/2)-1;

        if (root->keyCount > 1)
        {
            

        }
        else if ((leftPtr->keyCount >= minimumKeys) && (rightPtr->keyCount >= minimumKeys))
        {
            
        }
        else
        {
            
            if (rightPtr == NULL)
            {
                root = root->childPtr[0];
            }
            else
            {
                //merge leftPtr, root, and rightPtr
                if (leftPtr->keyCount + rightPtr->keyCount < M-1)
                {
                    //move parent key to leftPtr
                    leftPtr->keys[leftPtr->keyCount] = root->keys[0];
                    leftPtr->keyCount++;
                    
                    
                    //move everything on the rightPtr to leftPtr
                    int rightCount = rightPtr->keyCount;
                    int leftCount = leftPtr->keyCount;
                    for (int i = 0; i < rightCount; i++)
                    {
                        leftPtr->keys[leftCount + i] = rightPtr->keys[i];
                        leftPtr->childPtr[leftCount+i] = rightPtr->childPtr[i];
                        leftPtr->keyCount++;
                        rightPtr->keyCount--;
                    }
                    leftPtr->childPtr[leftPtr->keyCount] = rightPtr->childPtr[rightCount];
                
                    
                    root = root->childPtr[0];
                }
                
                else if (rightPtr->keyCount > minimumKeys)
                {
                    int tempNode[10];
                    int tempCount = 0;
                    
                    //move all the keys from leftPtr to tempNode
                    int count = leftPtr->keyCount;
                    for (int i = 0; i < count; i++)
                    {
                        tempNode[i] = leftPtr->keys[i];
                        tempCount++;
                        leftPtr->keyCount--;
                    }
                    //move the parent key to tempNode
                    tempNode[tempCount] = root->keys[0];
                    tempCount++;
                    
                    //move all the keys from rightPtr to tempNode
                    count = rightPtr->keyCount;
                    int temp = tempCount;
                    for (int i = 0; i < count; i++)
                    {
                        tempNode[temp + i] = rightPtr->keys[i];
                        tempCount++;
                        rightPtr->keyCount--;
                    }

                    //re-distribute
                    int median = M/2;
                    //put the first half in leftPtr
                    for (int i = 0; i < median; i++)
                    {
                        leftPtr->keys[i] = tempNode[i];
                        leftPtr->keyCount++;
                    }
                    //put the median in the parent key
                    root->keys[0] = tempNode[median];
                    
                    //put the rest in rightPtr
                    for (int i = median +1; i < tempCount; i++)
                    {
                        rightPtr->keys[i-median-1] = tempNode[i];
                        rightPtr->keyCount++;
                    }
                

                }
            }
        }
        
    }
    else if (return_code == NOT_FOUND)
    {
            std::cout << std::endl;
            std::cout << elem << "<-------- This key does not exist in the tree" << std::endl;
    }
    else
    {
        //DELETE_SUCCESS
    
    }
    
    if (root->keyCount == 0)
    {
        root = NULL;
    }
    
}


template<typename T, int M>
int BTree<T, M>::_remove(BNode<T, M> *&node, T &key)
{
    int result;
    double doubleM(M);
    double minimumKeys = ceil(doubleM/2)-1;
    
    //if the node is empty, return
    if (node == NULL)
    {
        return NOT_FOUND;
    }
    
    //traverse the node and find the key to delete
    int index = 0;
    int ptrIndex = 0;
    bool keyFound = false;
    for (int i = 0; i < node->keyCount; i++)
    {
        //places the index to the pointer that needs to go into
        if (key > node->keys[i])
        {
            index++;
        }
        
        else if (key == node->keys[i])
        {
            keyFound = true;
        }
        
        else
        {
            //if the inserting key is less than the comparing key, then do nothing
        }
    }
    
    //Key Found
    if (keyFound)
    {
        //if the key is in the leaf node
        if (node->childPtr[index] == NULL)
        {
            //delete the key
            for (int i = index+1; i < node->keyCount; i++)
            {
                node->keys[i-1] = node->keys[i];
            }
            node->keyCount--;
            
            if ((node->keyCount < 2) && (node == root))
            {
                return DELETE_SUCCESS;
            }
            
            //check if the node has minimum # of keys
            if (node->keyCount < minimumKeys)
            {
                return _UNDERFLOW;
            }
            else
            {
                return DELETE_SUCCESS;
            }
        }
        
        //if the key is not in the leaf node
        else
        {
            //find the immediate predecessor
            BNode<T, M> *ptr = node;
            ptr = node->childPtr[index];
            while (ptr->childPtr[ptr->keyCount] != NULL)
            {
                ptr = ptr->childPtr[ptr->keyCount];
            }
            
            //(swap) replace key with its predecessor
            node->keys[index] = ptr->keys[ptr->keyCount-1];
            
            //delete the predecessor recursively
            result = _remove(node->childPtr[index], ptr->keys[ptr->keyCount-1]);
        }
    }
    
    //Key has not been found in the node
    //Enter Child Pointer
    else
    {
        ptrIndex = index;
        //decrement index because it had
        //been incremented one more than it
        //needs to be
        index--;
        result = _remove(node->childPtr[ptrIndex], key);
    }
    
    //Use the "result" to decide what instructions
    //to carry out
    if (result == _UNDERFLOW)
    {
        //borrow from left sibling
        if ((ptrIndex != 0) && (node->childPtr[ptrIndex-1]->keyCount > minimumKeys))
        {
            BNode<T, M> *leftPtr = node->childPtr[ptrIndex-1];
            BNode<T, M> *rightPtr = node->childPtr[ptrIndex];
            int tempNode[10];
            int tempCount = 0;
            
            //move all the keys from leftPtr to tempNode
            int count = leftPtr->keyCount;
            for (int i = 0; i < count; i++)
            {
                tempNode[i] = leftPtr->keys[i];
                tempCount++;
                leftPtr->keyCount--;
            }
            
            //move the parent key to tempNode
            tempNode[tempCount] = node->keys[index];
            tempCount++;
            
            count = rightPtr->keyCount;
            //move all the keys from rightPtr to tempNode
            for (int i = 0; i < count; i++)
            {
                tempNode[tempCount + i] = rightPtr->keys[i];
                tempCount++;
                rightPtr->keyCount--;
            }
            
            //re-distribute
            int median = M/2;
            //put the first half in leftPtr
            for (int i = 0; i < median; i++)
            {
                leftPtr->keys[i] = tempNode[i];
                leftPtr->keyCount++;
            }
            //put the median in the parent key
            node->keys[index] = tempNode[median];
            
            //put the rest in rightPtr
            for (int i = median +1; i < tempCount; i++)
            {
                rightPtr->keys[i-median-1] = tempNode[i];
                rightPtr->keyCount++;
            }
            
            
            
        }
        
        //borrow from right sibling
        else if ((ptrIndex != node->keyCount) && (node->childPtr[ptrIndex+1]->keyCount > minimumKeys))
        {
            BNode<T, M> *leftPtr = node->childPtr[ptrIndex];
            BNode<T, M> *rightPtr = node->childPtr[ptrIndex+1];
            int tempNode[10];
            int tempCount = 0;
            
            //move all the keys from leftPtr to tempNode
            int count = leftPtr->keyCount;
            for (int i = 0; i < count; i++)
            {
                tempNode[i] = leftPtr->keys[i];
                tempCount++;
                leftPtr->keyCount--;
            }
            
            //move the parent key to tempNode
            tempNode[tempCount] = node->keys[index];
            tempCount++;
            
            //move all the keys from rightPtr to tempNode
            count = rightPtr->keyCount;
            int temp = tempCount;
            for (int i = 0; i < count; i++)
            {
                tempNode[temp + i] = rightPtr->keys[i];
                tempCount++;
                rightPtr->keyCount--;
            }
            
            //re-distribute
            int median = M/2;
            //put the first half in leftPtr
            for (int i = 0; i < median; i++)
            {
                leftPtr->keys[i] = tempNode[i];
                leftPtr->keyCount++;
            }
            //put the median in the parent key
            node->keys[index] = tempNode[median];
            
            //put the rest in rightPtr
            for (int i = median +1; i < tempCount; i++)
            {
                rightPtr->keys[i-median-1] = tempNode[i];
                rightPtr->keyCount++;
            }

            
        }
        
        //neither siblings have more than minimal # of key
        //merge with parent and left sibling
        else
        {
            if (node == root && root->keyCount == 1)
            {
                return _UNDERFLOW;
            }
            
            BNode<T, M> *leftPtr = node->childPtr[ptrIndex-1];
            BNode<T, M> *rightPtr = node->childPtr[ptrIndex];
            
            if (ptrIndex == 0)
            {
                leftPtr = node->childPtr[ptrIndex];
                rightPtr = node->childPtr[ptrIndex+1];
            }
            
            //add the parent key to leftPtr
            leftPtr->keys[leftPtr->keyCount] = node->keys[index];
            leftPtr->keyCount++;
            
            
            //add all the keys of rightPtr to leftPtr
            int rightCount = rightPtr->keyCount;
            int leftCount = leftPtr->keyCount;
            for (int i = 0; i < rightCount; i++)
            {
                leftPtr->keys[leftCount + i] = rightPtr->keys[i];
                leftPtr->childPtr[leftCount+i] = rightPtr->childPtr[i];
                leftPtr->keyCount++;
                rightPtr->keyCount--;
            }
            leftPtr->childPtr[leftPtr->keyCount] = rightPtr->childPtr[rightCount];
            
            //shift the parent key array
            if (ptrIndex != node->keyCount)
            {
                for (int i = ptrIndex+1; i < node->keyCount; i++)
                {
                    node->keys[i-1] = node->keys[i];
                    node->childPtr[i] = node->childPtr[i+1];
                }
            }
            
            node->keyCount--;
            delete rightPtr;
            rightPtr = NULL;
            
            //check if parent node has minimum # of keys
            if (node->keyCount < minimumKeys)
            {
                return _UNDERFLOW;
            }
            else
            {
                return DELETE_SUCCESS;
            }
        }
        
        
        
    }
    else if (result == NOT_FOUND)
    {
        return NOT_FOUND;
    }
    else
    {
        return DELETE_SUCCESS;
    }
    return DELETE_SUCCESS;
}



//prints level order
template<typename T, int M>
void BTree<T, M>::print()
{
    //empty tree
    if (root == NULL)
    {
        std::cout << "CANNOT PRINT: TREE IS EMPTY"  << std::endl;
        return;
    }
    
    //find the height of the b-tree
    int height = 0;
    BNode<T, M> *ptr;
    ptr = root;
    while (ptr->childPtr[0] != NULL)
    {
        ptr = ptr->childPtr[0];
        height++;
    }
    
    //print out the elements recursively for each level/height
    std::cout << "Printing in level-order" << std::endl;
    for (int i = 0; i <= height; i++)
    {
        std::cout << "Depth " << i << "---->";
        _print(root, i);
        std::cout << std::endl;
    }
}


template<typename T, int M>
void BTree<T, M>::_print(BNode<T, M> *node, int level)
{
    if (node == NULL)
    {
        return;
    }
    

    if (level == 0)
    {
        for (int i = 0; i < node->keyCount; i++)
        {
            std::cout << node->keys[i] << "  ";
        }
    }
    
    else if (level > 0)
    {
        for (int i = 0; i <= node->keyCount; i++)
        {
            _print(node->childPtr[i], level-1);
        }
    }
    
}


template<typename T, int M>
bool BTree<T, M>::find(T key)
{
    std::cout << "Searching for key " << key << std::endl;
    BNode<T, M> *search = root;
    while (search != NULL)
    {
        int index = 0;
        bool keyFound = false;
        for (int i = 0; i < search->keyCount; i++)
        {
            //places the index to the pointer that needs to go into
            if (key > search->keys[i])
            {
                index++;
            }
            
            else if (key == search->keys[i])
            {
                keyFound = true;
            }
            
        }
        //compare the key
        if (keyFound)
        {
            std::cout << "The node has been found!" << std::endl;
            return true;
        }
        else
        {
            search = search->childPtr[index];
        }

    }
    std::cout << "The node has not been found!" << std::endl;
    return false;
}


#endif /* B_Tree_h */
