#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

const int MAX_LINK_LENGTH = 30;

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

//
Link_List get_training_data()
{
    
    int training[MAX_LINK_LENGTH];
    int num_links = 0;
    
    // Load data
    ifstream inData("training_data.txt");
    if (!inData.is_open()) std::cout << "Error: file open" << endl;
    int x; // used to read in data
   
    // Store data from file in an array
    while(inData >> x){
        training[num_links++] = x;
    }
    
    // Transform array of links into a linked list
    Link_List list;
    list.insert_array_of_data(training, num_links);
    
    inData.close();
    return list;
}

int main(){
   // const int num_route_training = 6; // training on 6 routes
    
   //int training[num_route_training][MAX_LINK_LENGTH]; // training is a 6x20 matrix: each row represents a single trip (r),
                                                        // and columns hold the links seen during trip r.
    
    Link_List train_links = get_training_data();
    train_links.print_link_list();
    
    
    return 0;
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
