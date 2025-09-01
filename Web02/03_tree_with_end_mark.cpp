#include <vector>
#include <memory>
#include <iostream>
#include <limits>
using namespace std;

template <typename T>
class Tree{
    struct Node{
        T info;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> parent;
        Node(const T& value):info(value){}
    };
    
    std::shared_ptr<Node> head;
    
public:
    std::shared_ptr<Node> end;
    Tree():end(new Node(std::numeric_limits<T>::max())){}
    Tree(const std::initializer_list<T> &list):Tree(){
        for(const T& value: list){
            addNode(value);
        }
    }
    void add(std::shared_ptr<Node>& cur, const T& value){
        if (cur->info < value){
            if (cur->right){
                if (cur->right == end){
                    cur->right = std::make_shared<Node>(Node(value));
                    cur->right->parent = cur;
                    cur->right->right = end;
                }
                else add(cur->right, value);
            }
            else {
                cur->right = std::make_shared<Node>(Node(value));
                cur->right->parent = cur;
            }
        }
        else{
            if (cur->left) add(cur->left, value);
            else {
                cur->left = std::make_shared<Node>(Node(value));
                cur->left->parent = cur;
            }
        }
    }
    void addNode(const T&value){
        if(!head){
            head = std::make_shared<Node>(Node(value));
            head->right = end;
        }
        else add(head, value);
    }
    std::shared_ptr<Node> find(const std::shared_ptr<Node> & node, const T& value) const{
        if (node == end) return end;
        if (node->info == value) return node;
        if (node->left && node->info > value) return find(node->left, value);
        if (node->right && node->info < value) return find(node->right, value);
        return end;
    }


    std::shared_ptr<Node> find(int value) const{
            return find(head, value);
    }

};

int main(){    
    Tree<int> tree({3,2,1,4,5});
    if (tree.find(7) != tree.end){
        cout << "Элемент найден\n";
    }
    else{
        cout << "Элемент не найден\n";
    }
    return 0;    
}

