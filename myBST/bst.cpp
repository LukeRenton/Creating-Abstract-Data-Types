#include <iostream>

using namespace std;

class TreeNode{
public:
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    int value;

    // Constructor, sets the value
    TreeNode(int v) : value(v) {}

    ~TreeNode() {
        delete left;
        delete right;
    }
};

class Tree{
private:
    TreeNode* root = nullptr;

public:
    TreeNode * minValueLeaf(TreeNode * node){
        TreeNode * curr = node;
        while (curr && curr->left != nullptr){
            curr = curr->left;
        }
        return curr;
    }

    void insert(int v, TreeNode* &subtree){
        if(subtree == nullptr){
           subtree = new TreeNode(v);
        }else if(v < subtree->value){
            insert(v, subtree->left);
        }else{
            insert(v, subtree->right);
        }
    }

    void preOrderTraversal(TreeNode* subtree) const{
        if (subtree == nullptr) return;
        cout << subtree->value << " ";
        preOrderTraversal(subtree->left);
        preOrderTraversal(subtree->right);
    }

    void inOrderTraversal(TreeNode* subtree) const{
        if (subtree == nullptr) return;
        inOrderTraversal(subtree->left);
        cout << subtree->value << " ";
        inOrderTraversal(subtree->right);

    }

    void postOrderTraversal(TreeNode* subtree) const{
        if (subtree == nullptr) return;
        postOrderTraversal(subtree->left);
        postOrderTraversal(subtree->right);
        cout << subtree->value << " ";
    }

    int min(TreeNode* subtree) const{
        while (subtree->left != nullptr){
            subtree = subtree->left;
        }
        return subtree->value;

    }
    int max(TreeNode* subtree) const{
        while (subtree->right != nullptr){
            subtree = subtree->right;
        }
        return subtree->value;
    }
    bool contains(int value, TreeNode* subtree) const{
        while(subtree != nullptr){
            if (value == subtree->value){
                return true;
            }

            if (value < subtree->value){
                subtree = subtree->left;
            }
            else{
                subtree = subtree->right;
            }
        }
        return false;
    }
    TreeNode* remove(int value, TreeNode * root){
        //case 0: just delete pointer
        //case 1: replace with that pointer
        //case 2: find minimum in right subtree

        if (root == nullptr) return root;
              if (value < root->value)
                 root->left = remove(value, root->left);
              else if (value> root->value)
                 root->right = remove(value, root->right);
           else{
              if (root->left == nullptr){
                 TreeNode *temp = root->right;
                 free(root);
                 return temp;
              }
              else if (root->right == nullptr){
                 TreeNode *temp = root->left;
                 free(root);
                 return temp;
              }
              TreeNode* temp = minValueLeaf(root->right);
              root->value = temp->value;
              root->right = remove(temp->value, root->right);
           }
           return root;


    }

    void insert(int value){
        insert(value, root);

    }

    void preOrderTraversal(){
        preOrderTraversal(root);
        cout << endl;
    }
    void inOrderTraversal(){
        inOrderTraversal(root);
        cout << endl;
    }
    void postOrderTraversal(){
        postOrderTraversal(root);
        cout << endl;
    }
    int min(){
        return min(root);
    }
    int max(){
        return max(root);
    }
    bool contains(int value){
        return contains(value, root);
    }
    void remove(int value){
        remove(value, root);

    }
    ~Tree(){
        delete root;
    }
};

int main(){
    Tree t;
    int value;

    while(cin >> value && value != -1){
        t.insert(value);
    }


    t.preOrderTraversal();
    t.inOrderTraversal();
    t.postOrderTraversal();

    cout << t.min() << endl;
    cout << t.max() << endl;

    while (cin >> value && value != -1){
        if (t.contains(value)){
            cout << "true" << endl;
        }
        else{
            cout << "false" << endl;
        }
    }

    cout << endl;


    while(cin >> value && value != -1){
        t.remove(value);
        t.preOrderTraversal();
        t.inOrderTraversal();
        t.postOrderTraversal();

        cout << endl;
    }
}
