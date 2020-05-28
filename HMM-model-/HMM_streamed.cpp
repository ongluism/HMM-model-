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
class directed_graph
{
public:

	Node * *head;

	directed_graph(int number_Nodes, int number_edges, directed_edge array_of_edges[])
	{
		head = new Node*[number_Nodes]();
		this->number_Nodes = number_Nodes;

		for (int count = 0; count < number_Nodes; count++)
			head[count] = nullptr;

		// add edge
		for (int count = 0; count < number_edges; count++)
		{
			int begin_Road = array_of_edges[count].begin_Road;
			int end_Road = array_of_edges[count].end_Road;
			double weight_transition = array_of_edges[count].weight_transition;

			Node* newNode = adjacency_list(head[begin_Road], end_Road, weight_transition);

			head[begin_Road] = newNode;
		}
	}

	~directed_graph()
	{
		for (int count = 0; count < number_Nodes; count++)
			delete[] head[count];

		delete[] head;
	}

private:
	int number_Nodes;

	//	Node * adjacency_list(Node* head, int end_road, double weight_transition)
	Node * adjacency_list(Node* head, int end_road, double weight_transition)
	{
		Node* new_Node = new Node;

		new_Node->weight_transition = weight_transition;
		new_Node->nxt = head;
		new_Node->intersect_num = end_road;

		return new_Node;
	}
};
 class directed_edge {

 public:
     
     int begin_Road;
     int end_Road;    //I want to turn these variables into private variables
     double weight_transition;        // and create a class like the one below, but not sure how.
 };
 OG main
 int main()
{

	//array of objects
	cout << "poop" << endl;
	cout << "piss" << endl;
	
	int train_routes = 22; //22 routes as training data for HMM model

	int train_routes_1 = 9;		//1st level: 9 routes of 22 go left
	int train_routes_2 = 13;	//1st level: 13 routes of 22 go down
	int train_routes_3 = 8;		//2nd level: 8 routes of 9 go left
	int train_routes_4 = 1;		//2nd level: 1 routes of 9 go down
	int train_routes_5 = 2;		//2nd level: 2 routes of 13 go left
	int train_routes_6 = 11;	//2nd level: 11 routes of 13 go down

	//probabilities (these probabilities are constants right now, but should be dynamic)
	double x = (train_routes_1 + 1) / double(train_routes + 2);		// '+1' and '+2' are laplac smoothing constants
	double y = (train_routes_3 + 1) / double(train_routes_1 + 2);	
	double z = (train_routes_5 + 1) / double(train_routes_2 + 2);

	//{a,b,c}
	//a is the starting Node
	//b is ending Node
	//c is the probabilities computed above (note, this should not be a constant)
	directed_edge array_of_roads[] = {
		{ 0,1,x},{ 0,2,abs(x-1)},{ 1,3,y},{1,4,abs(y-1)},{2,5,z},{2,6, abs(z-1)} };		//this is only the first few Nodes
		

	///ignore
	///{ 0,1 },{ 1,2 },{ 1,5 },{ 2,3 },{ 2,6 },{ 3,4 },{ 3,7 },
	///{ 4,8 },{ 5,6 },{ 5,12 } ,{ 6,7 } ,{ 7,8 } ,{ 7,9 },{ 8,10 },{ 9,10 },{ 9,14 },{ 10,11 },{ 10,15 },{ 11,16 },{ 12,13 } ,{ 12,17 } ,{ 13,14 } ,{ 13,18 },{ 14,15 } ,{ 14,19 },
	///{ 15,16 } ,{ 15,20 } ,{ 16,21 },{ 17,18 },{ 17,22 },{ 18,19 } ,{ 18,23 },{ 19,20 },{ 19,24 },{ 20,21 },{ 20,25 },{ 21,26 },{ 22,23 },{ 22,27 },{ 23,24 },{ 23,28 },{ 24,25 },{ 24,29 },
	///{ 25,26 },{ 25,30 } ,{ 26,31 } ,{ 27,28 } ,{ 27,33 } ,{ 28,29 } ,{ 28,34 },{ 29,30 },{ 29,35 },{ 30,31 },{ 30,36 } ,{ 31,32 },{ 31,37 } ,{ 32,38 },{ 33,34 },{ 34,35 },{ 35,36 },{ 36,37 },
	///{ 37,40 },{ 38,37 } ,{ 38,39 },{ 39,41 },{ 40,40 } ,{ 41,40 } };	//NOTE: {40,40} is end of route! It does not count as a link/state
	///ignore

	int number_Nodes = 4;

	number_edges = sizeof(array_of_roads) / sizeof(array_of_roads[0]); //gives you number of elements in the array, in this case: number of edges

																	   //cout << "Number of states/links is " << number_edges -1 << endl;
	directed_graph graph_1(number_Nodes, number_edges, array_of_roads); //graph needs number of edges, Nodes, and adjacency list above
																		//cout << number_Nodes << endl;
																		//print the adjacency list 
	for (int count = 0; count < number_Nodes; count++)
	{
		//cout << count;
		print_Node_list(graph_1.head[count], count);
	}


	///////////////////////////////////////////////
	int testing;
	cin >> testing;
	return 0;
}*/

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
