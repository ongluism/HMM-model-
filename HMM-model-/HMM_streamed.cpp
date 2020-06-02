#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <algorithm>    // std::find
using namespace std;

const int MAX_LINK_LENGTH = 16;
const int NUM_NODES = 66; // Nodes No. 0-65
const int GOAL_NODE = 65;
const int CLUSTER_0 = 0;
const int CLUSTER_1 = 1;
const int CLUSTER_2 = 2;
const int TOTAL_CLUSTERS = 3;
int removeDuplicates(int arr[], int n);
void print_1D_array(int data[], int size = MAX_LINK_LENGTH);
void print_2D_array(int a[][NUM_NODES], int r_size, int c_size);
float prob_ljGivenlxC(int a_C[][NUM_NODES], float prob_c, int lj, int lx);
float sum( int arr[], int size);


void calculateHmmParameters(int data[], int a[][NUM_NODES]){
    
    // {null, 0, 1, 3, 6, 10, 15, 22, 30, 39, 47, 53, 58, 65},
    int idx = 1;
    while(data[idx] != GOAL_NODE){
        a[data[idx]][data[idx+1]]++;
        idx++;
    }
    
}
void prediction(int testData[], int predicted_links[], int a0[][NUM_NODES], int a1[][NUM_NODES], int a2[][NUM_NODES], float                clusterProbabilityFloat[]){
    int linksPredicted[MAX_LINK_LENGTH]; // stores the predicted links
    linksPredicted[0] = 0;// starting point is always the same
    int lx = 0; // current link (lx) is initialized to 0, i.e the starting node.
    
    
    // For each link in testData predict the next link
    // currentLink index starts at one since testData[0] has cluster type
    for(int currentLinkIndex = 1; currentLinkIndex < MAX_LINK_LENGTH; ){
        // scope of ix (current node) iteration
        int max_sum_link = 0;
        float max_sum = 0.0; // initialize sum to zero
        for (int lj = lx + 1; lj < NUM_NODES; lj++) {
            // scope of lj iteration
            float prob_sum_lj = 0.0;
            
            // for each cluster compute sum( P(lj | lx, C )
            for( int clust = 0; clust < TOTAL_CLUSTERS; clust++) // 3 clusters in total
            {
                switch(clust){
                    case CLUSTER_0:
                        prob_sum_lj += prob_ljGivenlxC(a0, clusterProbabilityFloat[CLUSTER_0], lj, lx);
                        break;
                    case CLUSTER_1:
                        prob_sum_lj += prob_ljGivenlxC(a1, clusterProbabilityFloat[CLUSTER_1], lj, lx);
                        break;
                    case CLUSTER_2:
                        prob_sum_lj += prob_ljGivenlxC(a2, clusterProbabilityFloat[CLUSTER_2], lj, lx);
                        break;
                    default:
                        cout << "ERROR: switch(clust)" << endl;
                        break;
                }
            }
            
            // Update max_sum & link index if needed
            if ( prob_sum_lj > max_sum){
                max_sum = prob_sum_lj; // update the maximum sum probability
                max_sum_link = lj; // store the link with the highest prob of being next
            }
            
        }
        // before incrementing to next_link we must store the link predicted
        linksPredicted[currentLinkIndex] = max_sum_link;
        
        // Increment to the next link i.e. actual next link becomes current link
        currentLinkIndex++; // increment because testData[1] = 0; i.e. starting node
        // testData= {0, 0, 1, 3, 6, 10, 15, 23, 32, 40, 47, 53, 58, 65},
        lx = testData[currentLinkIndex];
    }
    
    // testing output
    for (int i = 0; i < MAX_LINK_LENGTH; i++){
        predicted_links[i] = linksPredicted[i];
    }
//    cout << "predicted link:" << endl;
//    print_1D_array(linksPredicted, MAX_LINK_LENGTH);
}

// compute P(lj | lx, C)
float prob_ljGivenlxC(int a_C[][NUM_NODES], float prob_c, int lj, int lx){
    float prob_sum = 0.0;
    prob_sum += a_C[lx][lj];// number of times lx->lj when cluster = C
    float probabilityDistributionClusterC = prob_c / TOTAL_CLUSTERS;
    float sum_madelx = sum(a_C[lx], NUM_NODES);
    if (sum_madelx == 0){
        return 0.0; // dont divide by zero
    }
    prob_sum /= sum_madelx; // divided by total number of transitions made from lx when cluster = c
    prob_sum *= probabilityDistributionClusterC; // *probability of seeing cluster C
    return prob_sum;
}

int main(){
    // all training routes
    const int TRAIN_DATA_SIZE = 6;
    int trainData[TRAIN_DATA_SIZE][MAX_LINK_LENGTH] =
    {
        {0, 0, 1, 3, 6, 10, 15, 22, 30, 39, 47, 53, 58, 65},
        {0, 0, 1, 3, 6, 10, 15, 23, 32, 41, 49, 55, 59, 65},
        {1, 0, 1, 3, 7, 12, 17, 25, 34, 42, 49, 54, 57, 60, 61, 65},
        {1, 0, 1, 4, 8, 12, 17, 25, 34, 43, 50, 54, 58, 65},
        {2, 0, 2, 9, 14, 21, 29, 38, 46, 52, 56, 59, 65},
        {2, 0, 2, 9, 13, 19, 27, 36, 45, 52, 56, 59, 65}
    };

    // initialize all transition probabilities to zero
    int a0[NUM_NODES][NUM_NODES], a1[NUM_NODES][NUM_NODES], a2[NUM_NODES][NUM_NODES];
    for(int r = 0; r < NUM_NODES; r++){
        for(int c = 0; c < NUM_NODES; c++){
            a0[r][c] = 0;
            a1[r][c] = 0;
            a2[r][c] = 0;
        }
    }
    float clusterProbabilityFloat[] = {0, 0, 0};

    // LEARNING BLOCK
    // void calc_transition_prob(trainData, a0, a1, a2, clusterProbabilityFloat);
    //      Calculate A matrix for each cluster seen
    //      returns a0, a1, a2: transition probability matrix for each cluster
    //              P(C = i), i = 0, 1, 2: cluster distribution
    for(int i = 0 ; i < TRAIN_DATA_SIZE; i++){
        if (trainData[i][0] == CLUSTER_0){
            clusterProbabilityFloat[CLUSTER_0]++;         // count # of times clusters are seen in the training data
            calculateHmmParameters(trainData[i], a0); // passing transition
        }
        else if(trainData[i][0] == CLUSTER_1){
            clusterProbabilityFloat[CLUSTER_1]++;
            calculateHmmParameters(trainData[i], a1);
        }
        else if (trainData[i][0] == CLUSTER_2){
            clusterProbabilityFloat[CLUSTER_2]++;
            calculateHmmParameters(trainData[i], a2);
        }
    }
    // PREDICTION BLOCK
    //
    const int TEST_DATA_SIZE = 3;
    int predicted_links[TEST_DATA_SIZE][MAX_LINK_LENGTH];
    int testData[TEST_DATA_SIZE][MAX_LINK_LENGTH] =
    {
        {1, 0, 1, 4, 8, 12, 17, 25, 34, 43, 51, 56, 59, 65},
        {0, 0, 1, 3, 6, 10, 15, 23, 32, 40, 47, 53, 58, 65},
        {2, 0, 2, 9, 14, 21, 28, 35, 43, 50, 55, 59, 65}
    };
   
    for(int i = 0; i < TEST_DATA_SIZE; i++ ){
        prediction(testData[i], predicted_links[i], a0, a1, a2, clusterProbabilityFloat);
        // accuracy
        int j = 1;
        float acc = 0.0;
        for(; j < MAX_LINK_LENGTH; j++){
            if(testData[i][j] > GOAL_NODE)
                break;
           if(testData[i][j] == predicted_links[i][j-1]){
               acc = acc + 1.0;
           }
        }
        acc = acc/j; // j = 13
        cout << "accuracy =" << acc << "%" << endl;
    }
    
    
    
    return 0;
}


// HELPER FUNCTIONS
//
void print_2D_array(int a[][NUM_NODES], int r_size, int c_size){
    for (int i=0; i < r_size; i++){
        //cout << "ROW_" << i << "----->";
        print_1D_array(a[i], c_size);
        cout << endl; // new line to start next row
    }
}
void print_1D_array(int data[], int size){
    int i = 0;
    while(i < size){
        cout << data [i] << " ";
        i++;
    }
    
}
float sum( int arr[], int size){
    float sum = 0.0;
    for(int i = 0; i<size ; i++){
      sum+=arr[i];
   }
    return sum;
}












// *** used later to find all links seen by trips **

struct Node {
    int data;
    Node * next;
};

class Link_List {
public:
    double weight_transition;
    Node* head;
    
    Link_List()
    {
        head = NULL;
    }
    Link_List(int cluster) // Not currently used
    {
        head = new Node();
        head->data = cluster;
        head->next = NULL;
    }
    ~Link_List()
    {
        Node* current = head;
        while(current != NULL){
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = NULL;
    }
    void insert_array_of_data(int one_trip[], int size)
    {
        for(int i = 0; i < size; i++){
            
            insert_Node(&head, one_trip[i]);
        }
    }
    void print_link_list()
    {
        Node* curr = head->next;
        cout << "The head's data member is " << head->data << " --> This is the cluster type \n" << endl;
        cout << "The following output are the road links: " << endl;
        while(curr != NULL)
        {
            cout << curr->data << ", ";
            curr = curr->next;
        }
    }
private:
    void insert_Node(Node ** head_ref, int link_id){
        // allocate new link, add data, & set new_link's next Node to NULL
        Node* new_Node = new Node();
        new_Node->data = link_id;
        new_Node->next = NULL;
        Node* last = *head_ref;
  
        // assign head if list is empty
        if (*head_ref == NULL)
        {
            *head_ref = new_Node;
            // delete new_Node;
            return;
        }
       
        // insert last Node to then end of the list
        while (last->next != NULL)
            last = last->next;
   
        // re-assign last link
        last->next = new_Node;
        return;
    }
};



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
// FUNCTION: get_training_data(int trainData[][MAX_LINK_LENGTH])
// GOAL:     To obtain data from a text file where individual trips
//              are separated by a -1 integer and new line
// INPUT:    Empty 2D array 6x20:
//            - Each row represents a single trip (r),
//            - columns hold the links seen during trip r.
// OUTPUT:   Training data stored in 2D array
//
void get_training_data(int trainData[][MAX_LINK_LENGTH])
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
            trainData[i][num_links] = x;
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
void training_data()
{
    /*
     1 0 1 3 7 12 17 25 34 42 49 54 57 60 61 65 -1
     1 0 1 4 8 12 17 25 34 43 50 54 58 65 -1
     2 0 2 9 14 21 29 38 46 52 56 59 65 -1
     2 0 2 9 13 19 27 36 45 52 56 59 65 -1

     */
   int trainData[6][MAX_LINK_LENGTH] =
    {
        {0, 0, 1, 3, 6, 10, 15, 22, 30, 39, 47, 53, 58, 65},
        {0, 0, 1, 3, 6, 10, 15, 23, 32, 41, 49, 55, 59, 65},
        {1, 0, 1, 3, 7, 12, 17, 25, 34, 42, 49, 54, 57, 60, 61, 65},
        {1, 0, 1, 4, 8, 12, 17, 25, 34, 43, 50, 54, 58, 65},
        {2, 0, 2, 9, 14, 21, 29, 38, 46, 52, 56, 59, 65},
        {2, 0, 2, 9, 13, 19, 27, 36, 45, 52, 56, 59, 65}
    };
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
