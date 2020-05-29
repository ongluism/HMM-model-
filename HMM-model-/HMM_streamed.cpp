#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <algorithm>    // std::find
using namespace std;

const int MAX_LINK_LENGTH = 16;
const int NUM_NODES = 66;
const int GOAL_NODE = 65;
int removeDuplicates(int arr[], int n);
void print_1D_array(int data[], int size = MAX_LINK_LENGTH);

struct node {
    int data;
    node * next;
};

class Link_List {
public:
	double weight_transition;
    node* head;
    
    Link_List()
    {
        head = NULL;
    }
    Link_List(int cluster) // Not currently used
    {
        head = new node();
        head->data = cluster;
        head->next = NULL;
    }
    ~Link_List()
    {
        node* current = head;
        while(current != NULL){
            node* next = current->next;
            delete current;
            current = next;
        }
        head = NULL;
    }
    void insert_array_of_data(int one_trip[], int size)
    {
        for(int i = 0; i < size; i++){
            
            insert_node(&head, one_trip[i]);
        }
    }
    void print_link_list()
    {
        node* curr = head->next;
        cout << "The head's data member is " << head->data << " --> This is the cluster type \n" << endl;
        cout << "The following output are the road links: " << endl;
        while(curr != NULL)
        {
            cout << curr->data << ", ";
            curr = curr->next;
        }
    }
private:
    void insert_node(node ** head_ref, int link_id){
        // allocate new link, add data, & set new_link's next node to NULL
        node* new_node = new node();
        new_node->data = link_id;
        new_node->next = NULL;
        node* last = *head_ref;
  
        // assign head if list is empty
        if (*head_ref == NULL)
        {
            *head_ref = new_node;
            // delete new_node;
            return;
        }
       
        // insert last node to then end of the list
        while (last->next != NULL)
            last = last->next;
   
        // re-assign last link
        last->next = new_node;
        return;
    }
};

void training_data()
{
    /*
     1 0 1 3 7 12 17 25 34 42 49 54 57 60 61 65 -1
     1 0 1 4 8 12 17 25 34 43 50 54 58 65 -1
     2 0 2 9 14 21 29 38 46 52 56 59 65 -1
     2 0 2 9 13 19 27 36 45 52 56 59 65 -1

     */
   int traindata[6][MAX_LINK_LENGTH] =
    {
        {0, 0, 1, 3, 6, 10, 15, 22, 30, 39, 47, 53, 58, 65},
        {0, 0, 1, 3, 6, 10, 15, 23, 32, 41, 49, 55, 59, 65},
        {1, 0, 1, 3, 7, 12, 17, 25, 34, 42, 49, 54, 57, 60, 61, 65},
        {1, 0, 1, 4, 8, 12, 17, 25, 34, 43, 50, 54, 58, 65},
        {2, 0, 2, 9, 14, 21, 29, 38, 46, 52, 56, 59, 65},
        {2, 0, 2, 9, 13, 19, 27, 36, 45, 52, 56, 59, 65}
    };
}
void learningHMM_Parameters(int data[], int a[][NUM_NODES], int c[]){
    c[data[0]]++; // index 0 holds the cluster number type, it is used to
                  //    index c (tot num of clusters seen) & increment size
    
    // {null, 0, 1, 3, 6, 10, 15, 22, 30, 39, 47, 53, 58, 65},
    int idx = 1;
    while(data[idx] != GOAL_NODE){
        a[data[idx]][data[idx+1]]++;
        idx++;
    }
    
}

int main(){
    // all training routes
    int traindata[6][MAX_LINK_LENGTH] =
    {
        {0, 0, 1, 3, 6, 10, 15, 22, 30, 39, 47, 53, 58, 65},
        {0, 0, 1, 3, 6, 10, 15, 23, 32, 41, 49, 55, 59, 65},
        {1, 0, 1, 3, 7, 12, 17, 25, 34, 42, 49, 54, 57, 60, 61, 65},
        {1, 0, 1, 4, 8, 12, 17, 25, 34, 43, 50, 54, 58, 65},
        {2, 0, 2, 9, 14, 21, 29, 38, 46, 52, 56, 59, 65},
        {2, 0, 2, 9, 13, 19, 27, 36, 45, 52, 56, 59, 65}
    };
//    int arr[] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 17, 19, 21, 22, 23, 25, 27, 28, 29, 30, 32, 34, 35, 36, 38, 39, 40, 41, 42, 43, 45, 46, 47, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 65};
    
    //
    int a[NUM_NODES][NUM_NODES];
    for(int r = 0; r < NUM_NODES; r++){
        for(int c = 0; c < NUM_NODES; c++){
            a[r][c] = 0;
        }
    }
    int clusterDistribution[] = {0, 0, 0};
    learningHMM_Parameters(traindata[0], a, clusterDistribution); // passing transition
    
    return 0;
}


void print_1D_array(int data[], int size){
    int i = 0;
    while(i < size-1){
        cout << data [i] << ", ";
        i++;
    }
    
}

// *** used later to find all links seen by trips **
int removeDuplicates(int arr[], int n)
{
    //
    // Function to remove duplicate elements
    // This function returns new size of modified
    // array.
    
    // Return, if array is empty
    // or contains a single element
    if (n==0 || n==1)
        return n;
  
    int temp[n];
  
    // Start traversing elements
    int j = 0;
    for (int i=0; i<n-1; i++)
  
        // If current element is not equal
        // to next element then store that
        // current element
        if (arr[i] != arr[i+1])
            temp[j++] = arr[i];
  
    // Store the last element as whether
    // it is unique or repeated, it hasn't
    // stored previously
    temp[j++] = arr[n-1];
  
    // Modify original array
    for (int i=0; i<j; i++)
        arr[i] = temp[i];
  
    return j;
}

///
// FUNCTION: get_training_data(int traindata[][MAX_LINK_LENGTH])
// GOAL:     To obtain data from a text file where individual trips
//              are separated by a -1 integer and new line
// INPUT:    Empty 2D array 6x20:
//            - Each row represents a single trip (r),
//            - columns hold the links seen during trip r.
// OUTPUT:   Training data stored in 2D array
//
void get_training_data(int traindata[][MAX_LINK_LENGTH])
{
    //nt test[6][MAX_LINK_LENGTH];
    int training[MAX_LINK_LENGTH];
    int num_links = 0;
    int size_training = 0;
    
    // Load data
    ifstream inData("training_data.txt");
    if (!inData.is_open()) std::cout << "Error: file open" << endl;
    int x; // used to read in data
   
    for (int i = 0; i < 6; i++){
        while(inData >> x && x != -1){
            traindata[i][num_links] = x;
            if (i==0){
                training[size_training] = x;
                size_training++;
            }
            num_links++;
        }
    }
    inData.close();
    
    // Store data from file in an array
//    while(inData >> x){
//        training[num_links++] = x;
//    }
}


/*

/// The following function was obtained from // https://www.tutorialspoint.com/cplusplus/cpp_return_pointer_from_functions.htm
/// on 05/27/2020. Reference:
/// can be used to generate clustered data sets
int * getRandom( ) { // function to generate and retrun random numbers.
   static int  r[10];
 
   // set the seed
   srand( (unsigned)time( NULL ) );
   
   for (int i = 0; i < 10; ++i) {
      r[i] = rand();
      cout << r[i] << endl;
   }
 
   return r;
}
*/
