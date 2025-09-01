#include <vector>
#include <memory>
#include <iostream>
using namespace std;

template <typename T>
class Tree{
    struct Node{
        T info;
        shared_ptr<Node> left;
        shared_ptr<Node> right;
        shared_ptr<Node> parent;
        Node(const T& value):info(value){}
    };
    
    shared_ptr<Node> head;

public:
    Tree(){}
    Tree(const initializer_list<T> &list){
        for(const T& value: list){
            addNode(value);
        }
    }
    void add(shared_ptr<Node>& cur, const T& value){
        if (cur->info < value){
            if (cur->right) add(cur->right, value);
            else {
                cur->right = make_shared<Node>(Node(value));
                cur->right->parent = cur;
            }
        }
        else{
            if (cur->left) add(cur->left, value);
            else {
                cur->left = make_shared<Node>(Node(value));
                cur->left->parent = cur;
            }
        }
    }
    void addNode(const T&value){
        if(!head){head = make_shared<Node>(Node(value));}
        else add(head, value);
    }

    void print_sorted(ostream & out, const shared_ptr<Node> & node){
        if (node->left) print_sorted(out, node->left);
        out << node->info << " ";
        if (node->right && node->right != end) print_sorted(out, node->right);        
    }

    void print_branches(ostream & out, const shared_ptr<Node> & node){
        out << node->info << " ";
        if (node->left) print_branches(out, node->left);
        if (node->right && node->right != end) print_branches(out, node->right);        
    }

    friend ostream &operator << (ostream & out, Tree &tree){
        tree.print_sorted(out, tree.head);
        out << endl;
        return out;
    }

};

int main(){    
    Tree<int> tree({3,2,1,4,5});
    return 0;    
}


