#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;
// exploring dynamic memory and pointers.
class Node{
    public:
        Node();
        Node(int data, Node *next, Node *previous);
        ~Node();
    public:
        int data;
        Node *next;
        Node *previous;
};

Node::Node(){
    this->data = 0;//I have decided to initialize the value of each node to 0 for the default constructor
    this->next = NULL;
    this->previous = NULL;
}

Node::Node(int data, Node *next, Node *previous){
    this->data = data;
    this->next = next;
    this->previous = previous;
}
Node::~Node(){
}

class DLLStructure{
    public:
        DLLStructure();
        DLLStructure(int array[], int size);
        ~DLLStructure();
        void printDLL();
        void InsertAfter( int valueToInsertAfter, int valueToBeInserted);
        void InsertBefore( int valueToInsertBefore, int valueToBeInserted);
        void Delete(int value);
        void Sort();
        bool IsEmpty();
        int GetHead();
        int GetTail();
        int GetSize();
        int GetMax();
        int GetMin();
        DLLStructure( DLLStructure& dlls);
    private:
        Node* first;
        Node* last;
};

DLLStructure::DLLStructure(){
    first = NULL;
    last = NULL;
}

DLLStructure::~DLLStructure(){
    if (first!=NULL){//making sure that the DLLStructure is not empty.
        while(first->next!=NULL){//deleting every node
            Node *temp= first;
            first=first->next;
            delete temp;
        }
        delete first;
    }
    //cout <<"Destructor was called" <<"\n";//test to see if the destructor is being called
}

DLLStructure::DLLStructure(int array[], int size){
    if (size==0){//case where the array is empty. Then first and last will be pointing to nothing
        first=NULL;
        last=NULL;
    }else{
        Node *curr=new Node(array[0], NULL, NULL);//create the first node
        first=curr;
        for (int i=1; i<size; i++){//link the nodes following nodes afterwards.
            Node *el= new Node(array[i], NULL, curr);// the next node will be empty. We will update at the next iteration.
            this->last=el;//we will always update last everytime we add a node to the list
            curr->next = el;//updating the next node pointer of (el->previous)->next since it isn't the last element of the DLL anymore
            curr = el;//curr will become the node we just created
        }
    }
}
void DLLStructure ::printDLL(){
    Node *temp=first;
    if (temp==NULL){//if first is NULL the we have an empty DLL
        cout<< "The list is empty";
    }
    while (temp!=NULL){//otherwise print each element of the DLL by incrementing the pointer.
        cout << temp->data;
        temp=temp->next;
        if(temp!=NULL){//print a comma everytime after printing an element unless that it is the last element of the DLL
            cout << ", ";
        }
    }
    cout << "\n";
}

void DLLStructure ::InsertAfter( int valueToInsertAfter, int valueToBeInserted){
    Node *temp=first;
    bool found=false;//verify if valueToInsertAfter exists.
    while (temp!=NULL){
        if(temp->data == valueToInsertAfter){//valueToInsert was found.
            if (temp->next==NULL){//Case where valueToBeInserted will become the last element of the DLL
                Node *n = new Node(valueToBeInserted, NULL, temp);//create the new node that we will append
                temp->next=n;//update the next node pointer of temp.
                found=true;
                this->last = n;//update last as the new added node is the last element of the DLL now
            }else{//adding valueToBeInserted in the middle of the linked list otherwise.
                Node *n = new Node(valueToBeInserted, temp->next, temp);
                (temp->next)->previous=n;//update the previous node of n->next
                temp->next = n;//update the next node of valueToInsertAfter
                found=true;
            }
            break;
        }
        temp=temp->next;
    }
    //cout << found;
    temp=last;
    //cout << temp << " ";
    if (found == false){//if valueToInsertAfter not found, then add valueToBeInserted at the end of the DLL
        Node *n = new Node (valueToBeInserted, NULL, temp);
        temp->next = n;
        this->last = n;//update last as valueToBeInserted is now the last element of the DLL
    }
}

void DLLStructure ::InsertBefore( int valueToInsertBefore, int valueToBeInserted){
    Node *temp=first;
    Node *realLast=last;//the last pointer will never change as we will always be appending somewhere before the last element. 
                        //When we use InsertAfter we might modify the the last pointer so we should keep track of it.
    bool found=false;//keep track to see of valueToInsertBefore exists in the DLL
    InsertAfter(valueToInsertBefore, valueToBeInserted);
    //now that we inserted valueToBeInserted after valueToInsertBefore, we have to swap them to create the InsertBefore.
    while(temp!=NULL){
        if(temp->data==valueToInsertBefore){//if valueToInsertBefore is found, then we have appeneded valueToBeInserted somewhere in the middle of the list
                                            //if valueToBeInserted was the last element of the DLL before calling InsertAfter then it would be an edge case.
            found=true;
            if(temp->next != NULL){//case where  valueToInsertBefore is not the last element of the DLL.
                //here we have a swap. we just swap the data as the node pointers are already pointing the right addresses.
                int tempData= temp->next->data;
                temp->next->data=temp->data;
                temp->data=tempData;
            }else{
                Node*tempFirst = first;
                Node* tempLast= last;
                this -> last = last ->previous;
                last->next= NULL;
                this->first=tempLast;
                first->next= tempFirst;
                first -> previous = NULL;
            }

            break;
        }
        temp=temp->next;
    }
    if (found==false){//if valueToInsertBefore doesn't exist, then valueToBeInserted will be at the end of the DLL, but we want it at the beginning of the DLL.
                      //Hence, we need to update first and last and also change the node Pointers of the first node, the last node and the node of valueToBeInserted.
        (last->previous)->next=NULL;
        last->previous=NULL;
        last->next=first;
        first->previous=last;
        first=last;
    }
    last=realLast;//making sure that last has the same value as it did before calling InsertAfter.
}

void DLLStructure ::Delete(int value){
    Node *temp= first;
    while (temp != NULL){//searching if value exists.
        if (temp->data==value){//value exists
            if (temp==first){// case where we delete the first node of the DLL
                if (temp->next==NULL){//case where there is only one element in the DLL. We must update first and last.
                    this->first=NULL;
                    this->last=NULL;
                    delete temp;//freeing the space that node we deleted was taking.
                    break;
                }else{//there are more than 1 element so update first only with the previous node pointer of the new first node of the DLL
                    this->first=temp->next;
                    temp->next->previous=NULL;
                    delete temp;
                    break;
                }

            }else if(temp->next==NULL){// case where we are deleting the last element of the DLL. Then update last and the next node pointer of the new last node of the DLL
                this->last=temp->previous;
                temp->previous->next=NULL;
                delete temp;
                break;
            }else{// case where we are deleting a node in the middle of the DLL. temp->previous->next and temp->next->previous
                Node *pre = temp->previous;
                Node *nxt = temp->next;
                temp->previous->next=nxt;
                temp->next->previous=pre;
                delete temp;
                break;
            }
        }
        temp = temp->next;
    }
}

void DLLStructure ::Sort(){// I will be implementing a bubble sort. I will be sorting the largest values first. The largest values will be at the end of the list.
    Node *temp = first;
    bool notDone= true;//verify if we are done sorting
    Node *firstElement;
    Node *newFirstElement = NULL;//we need to verify all elements in the first iteration. So the first iteration will go on till firstElement->next=NULL.
    if (temp == NULL){//if list is empty then we don't sort anything
        return;
    }
    while(notDone){
        notDone = false;// we will set not done to true in the next while loop. If notDone does not get updated to true, then it means that we are done sorting.
        firstElement = temp;
        while (firstElement->next != newFirstElement){//We don't need to verify the last element we checked at the last iteration since the last element checked is sorted.
            if (firstElement->data > firstElement->next->data){ //if the firstElement is bigger than the next element, then swap.
                swap(firstElement->data, firstElement->next->data);
                notDone = true;//since we entered the while loop, it means that we are still not done sorting
            }
            firstElement = firstElement->next;
        }
        newFirstElement = firstElement;//update newFirstElement as we don't need to check the last element checked at the last iteration since the last element is sorted.
    }
}

bool DLLStructure ::IsEmpty(){
    if (first==NULL){//if the pointer first points to NULL then it means that the DLL is empty.
        return true;
    }else{
        return false;
    }
}

int DLLStructure ::GetHead(){
    return first->data;
}

int DLLStructure ::GetTail(){
    return last->data;
}

int DLLStructure ::GetSize(){
    int size=0;
    Node *temp=first;
    while(temp!=NULL){//loop through the DLL to increment the counter size to obtain the size of the DLL.
        size+=1;
        temp=temp->next;
    }
    return size;
}
int DLLStructure ::GetMax(){
    int alpha=0;
    Node *temp= first;
    if (temp==NULL){//if the list is empty then
        cout <<"The list is empty";
        return 0;//return 0 as the default constructor initializes the value of a node to 0.
    }while (temp!=NULL){
        if (temp->data > alpha){//loop through the DLL to find the biggest value.
            alpha=temp->data;
        }
        temp=temp->next;
    }
    return alpha;
}

int DLLStructure ::GetMin(){
    Node *temp= first;

    if (temp==NULL){
        cout <<"The list is empty";
        return 0;
    }
    int beta = (first->data);//we can't set beta to 0 as 0 will be smaller than any value of a given node. So initialize beta with first->data.
    while (temp!=NULL){//loop through the DLL to find the smallest value.
        if (temp->data < beta){
            beta=temp->data;
        }
        temp=temp->next;
    }
    return beta;
}
DLLStructure::DLLStructure( DLLStructure& dlls){
    int size = 1;//assume that dlls is a DLL structure with at least one node.
    Node *temp= dlls.first;
    while(temp->next!=NULL){//loop through dlls to obtain the size.
        temp = temp->next;
        size+=1;
    }
    int *array=new int[size];//create a pointer array
    temp= dlls.first;
    for(int i=0;i<size;i++){//ass the values in the pointer array.
        array[i] = temp->data;
        temp = temp->next;
    }
    //line 301 to line 312 is the constructor of DLLStructure(int array[], size). We need to implement it here, otherwise "DLLStructure newDLL(array,size)" will be in a local scope
    //so we won't be able to print out the desired expected outcome.
    if (size==0){
        first=NULL;
        last=NULL;
    }else{
        Node *curr=new Node(array[0], NULL, NULL);
        first=curr;
        for (int i=1; i<size; i++){
            Node *el= new Node(array[i], NULL, curr);
            this->last=el;
            curr->next = el;
            curr = el;
        }
    }
    DLLStructure newDLL(array,size);
}


int main(){
    /*
    Node *a=new Node();
    Node *b=new Node();
    Node *c=new Node();
    a->data=1;
    b->data=2;
    c->data=3;
    a->previous=NULL;
    b->previous=a;
    c->previous=b;
    a->next=b;
    b->next=c;
    c->next=NULL;

    cout << a->data << " ";
    cout << (a->next)->data << " ";
    cout << (a->previous) << "\n";

    cout << b->data << " ";
    cout << (b->next)->data << " ";
    cout << (b->previous)->data << "\n";


    cout << c->data << " ";
    cout << (c->next) << " ";
    cout << (c->previous)->data << "\n";
    */
    // Q 1, 2, 3 should obviously be implemented successfully
    // in order to run the following code
    int array[5] = {11, 2, 7, 22, 4};
    DLLStructure dll(array, 5); // note that 5 is the size of the array
    dll.printDLL(); // the output should be: 11, 2, 7, 22, 4
    // Q 4
    dll.InsertAfter(7, 13); // To insert 13 after the first occurence of 7
    dll.printDLL(); // the output should be: 11, 2, 7, 13, 22, 4
    dll.InsertAfter(25, 7); // To insert 7 after the first occurence of 25
    dll.printDLL(); // the output should be: 11, 2, 7, 13, 22, 4, 7
    
    // Q 5
    dll.InsertBefore(7, 26); // To insert 26 before the first occurence of 7
    dll.printDLL(); // the output should be: 11, 2, 26, 7, 13, 22, 4, 7
    dll.InsertBefore(19, 12); // To insert 12 before the first occurence of 19
    dll.printDLL(); // the output should be: 12, 11, 2, 26, 7, 13, 22, 4, 7
    // Q 6
    dll.Delete(22);
    dll.printDLL(); // the output should be: 12, 11, 2, 26, 7, 13, 4, 7
    // Q 7
    dll.Sort();
    dll.printDLL(); // the output should be: 2, 4, 7, 7, 11, 12, 13, 26
    // Q 8
    if (dll.IsEmpty()){
        cout << "The list is empty" << endl;
    }
    
    // Q 9
    cout << "Head element is: " << dll.GetHead() << endl;
    cout << "Tail element is: " << dll.GetTail() << endl;
    // Q 10
    cout << "Number of elements in the list is: " << dll.GetSize() <<endl;

    cout << "Q10)   We can avoid looping everytime we call GetSize() by adding a new attribute \"int size\" in the constructor of the DLLStructure class. For the default constructor, "
    <<"\n       we can initialize the size to 0, and the size of the personalized constructor would be the size of the array that the personalized constructor takes as an input."
    <<"\n       We can increment the variable \"size\" everytime we add an element to the list, and decrement it everytime we delete an element from the list."
    <<"\n       Now, instead of looping throughthe list everytime we call GetSize(), we can simply return the variable \"size\" to obtain the size of the list. "
    <<"\n       This method is more efficient as it has a constant time since we are not looping everytime we call GetSize()." << "\n\n";
    // Q 11
    cout << "Max element is: " << dll.GetMax() << endl;
    cout << "Min element is: " << dll.GetMin() << endl;
    cout << "\nQ11)   We can avoid looping everytime we call GetMax() and GetMin() by adding the attributes \"int max, min\" in the constructor of the DLLStructure class."
    <<"\n       For the default constructor, we can set the max and min values to 0(default value). The max value of the personalized constructor would be the"
    <<"\n       largest value we would have for the data attribute while we initialize the personalized constructor and the min value would be the lowest value we would"
    <<"\n       have for the data attribute while we initialize the personalized constructor. When we add a new element to the list, we can directly verify if the new element"
    <<"\n       is bigger than the current max value or smaller than the current min value. Hence we can directly update the max and min value. When we delete an element in the list,"
    <<"\n       the max value and min value won't change unless we delete the element with the min value or the max value. In the case where we delete the min or max value, we would have to loop back"
    <<"\n       through the list to find the new max or min value. Thus, we might need to loop through the list in the case we delete the max or min element, but it is still more "
    <<"\n       efficient than to loop through the list evertime that we call GetMax() or GetMin().\n";
    // Q 12
    cout <<"\nQ12   The default copy constructor does a shallow copy and not a deep copy. Hence, any modification done on the original DLL or the newDLL can be seen through"
    << "\n        the old and the new DLL which makes it unreliable as we might not want the changes on one DLL to be reflected on another DLL. \n";
    DLLStructure dll2 (dll);
    dll2.printDLL(); // the output should be: 2, 4, 7, 7, 11, 12, 13, 26
    return 0;
}
