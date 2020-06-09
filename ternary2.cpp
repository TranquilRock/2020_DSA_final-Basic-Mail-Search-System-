#include <cstdlib> 
#include <vector>
#include <iostream>
using namespace std;
// A node of ternary search tree
struct mail{
    string from,to,subject;
    int id,length;
    unsigned long long date;
    bool currently_valid;
    //ex:   19 May 2011 at 16:50
    //   -> 201105191650
    //set<string> content;
    mail(){};
};
struct id_compare {
    bool operator() (const mail* a, const mail* b) const {
        return a->id < b->id;
    }
};
struct Node 
{ 
    char data; 
  
    // True if this character is last character of one of the words 
    set<mail*,id_compare> TST_set;

    struct Node *left, *eq, *right;
    Node(char x):data(x){
        left=eq=right=NULL;
    };
}; 
  
// A utility function to create a new ternary search tree node 
// struct Node* newNode(char data) 
// { 
//     struct Node* temp = new node()//(struct Node*) malloc(sizeof( struct Node )); 
//     temp->data = data; 
//     temp->left = temp->eq = temp->right = NULL; 
//     return temp; 
// } 
extern int sizeabc=0;
// Function to insert a new word in a Ternary Search Tree 
void TST_insert(struct Node** root,const char *word ,mail * M) 
{   
    while(*word){

        if ((*root)==NULL) {(*root) = new Node(*word);sizeabc++;}
        if ((*word) < (*root)->data) 
            root=&(*root)->left; 

        else if ((*word) > (*root)->data) 
            root=&(*root)->right; 
      
        else
        { 
            if (*(word+1)) 
                (root)=&(*root)->eq;
            else{   
                //cout <<"fuckme\n";
                (*root)->TST_set.insert(M); 
            }
            word++;
        } 
    }
    
} 
Node* searchTST(struct Node *root, const char *word) 
{ 
    if(*word==0)return NULL;
    while(*word){
        if (!root) 
            return NULL;
        if (*word < (root)->data) 
            root=root->left;

        else if (*word > (root)->data) 
            root=root->right;   

        else
        { 
            if (*(word+1) == '\0') 
                return root; 
            else word++,root=root->eq; 
        } 
    }
    return root;
} 
// // A recursive function to traverse Ternary Search Tree 
void traverseTSTUtil(struct Node* root, char* buffer, int depth) 
{ 
    if (root) 
    { 
        // First traverse the left subtree 
        traverseTSTUtil(root->left, buffer, depth); 
  
        // Store the character of this node 
        buffer[depth] = root->data; 
        if (root->TST_set.size()!=0) 
        { 
            buffer[depth+1] = '\0'; 
            printf( "%s\n", buffer); 
        } 
  
        // Traverse the subtree using equal pointer (middle subtree) 
        traverseTSTUtil(root->eq, buffer, depth + 1); 
  
        // Finally Traverse the right subtree 
        traverseTSTUtil(root->right, buffer, depth); 
    } 
} 
  
// // The main function to traverse a Ternary Search Tree. 
// // It mainly uses traverseTSTUtil() 
void traverseTST(struct Node* root) 
{ 
    char buffer[10000]; 
    cout <<"\nTTTTT\n";
    traverseTSTUtil(root, buffer, 0); 
} 
  
// // Function to search a given word in TST 
