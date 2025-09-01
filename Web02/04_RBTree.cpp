#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

enum Color { RED, BLACK };

map<string, string> nodeStyle({{"shape", "ellipse"},
     {"style", "\"filled, bold\""}, {"fontcolor", "white"},
     {"fontsize", "12"},{"width", "0.4"}, {"height", "0.2"},
     {"margin", "0.01"}});

map<string, string> edgeStyle({{"color", "gray"},
     {"style", "\"bold, filled\""}});


struct Node {
    int data;
    int ID;
    Color color;
    Node *left, *right, *parent;
    int childrenIDs[2] = {-1, -1};

    Node(int data) : data(data), ID(-1){
        parent = left = right = nullptr;
        color = RED;
        
    }
    void nodeToGraphVis(ofstream &out, int num){
        ID = num;
        string colorName = color == RED? "red": "black";
        out << "I" << num << "[";              
        out << " color = " << colorName << ", ";
        out << " fillcolor =" << colorName << ", ";        
        out << " label = \"" << data << "\"";
        out << "];\n";
    }
    static void nullToGraphVis(ofstream &out, int num){
        out << "I" << num << "[shape=box, color=black, height=0.1, width=0.1, fontsize=9, label=\"NULL\"];\n";
    }
    void edgeToGraphVis(ofstream &out){
        out << "I" << ID << " -- I" << childrenIDs[0]  <<" [";
        
        out << "];\n";
        out << "I" << ID << " -- I" << childrenIDs[1] <<"  [";
        for (auto &item :edgeStyle){
            out << item.first << " = " << item.second << ", ";
        }
        out << "];\n";
    }
};

class RBTree {
private:
    Node *root;
    
    // Вспомогательные функции
    void rotateLeft(Node *&);
    void rotateRight(Node *&);
    void fixViolation(Node *&);
    void inorderHelper(Node *);
    Node* BSTInsert(Node*, Node*);
    
public:
    RBTree() { root = nullptr; }
    void insert(const int &n);
    void inorder();
    void toGraphVis(string filename);
};


void outputNodes(Node * node, ofstream &out, int &num){
    if (node) {
        node->nodeToGraphVis(out, num++);
        node->childrenIDs[0] = num;
        if (node->left) outputNodes(node->left, out, num);
        else Node::nullToGraphVis(out, num++);   
        node->childrenIDs[1] = num;
        if (node->right) outputNodes(node->right, out, num);
        else Node::nullToGraphVis(out, num++);    
    }
}

void outputEdges(Node * node, ofstream &out){
    if (node){
        node->edgeToGraphVis(out);
        outputEdges(node->left, out);
        outputEdges(node->right, out);
    }
}

void RBTree::toGraphVis(string filename){
    ofstream out(filename);
    int num = 1;
    out << "graph G {\n";
    out << "size=\"4,9\"\n";
    out <<"fontname = \"Arial\"";
    out << "    graph [nodesep=0.1, ranksep=0.05]\n";
    out << "    node [";
    for (auto &item :nodeStyle){
        out << item.first << " = " << item.second << ", ";
    }  
    out << "]\n";
    out << "    edge [";
    for (auto &item :edgeStyle){
        out << item.first << " = " << item.second << ", ";
    }
    out << "]";
    outputNodes(this->root, out, num);
    outputEdges(this->root, out);
    out << "}";

}

// Вставка в бинарное дерево поиска
Node* RBTree::BSTInsert(Node* root, Node* pt) {
    if (root == nullptr) return pt;
    
    if (pt->data < root->data) {
        root->left = BSTInsert(root->left, pt);
        root->left->parent = root;
    } else if (pt->data > root->data) {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    
    return root;
}

// Вставка в красно-черное дерево
void RBTree::insert(const int &data) {
    static int n = 0;
    Node *pt = new Node(data);
    
    root = BSTInsert(root, pt);
    fixViolation(pt);
}

// Восстановление свойств после вставки
void RBTree::fixViolation(Node *&pt) {
    Node *parent_pt = nullptr;
    Node *grand_parent_pt = nullptr;
    
    while ((pt != root) && (pt->color != BLACK) && 
           (pt->parent->color == RED)) {
        
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
        
        // Случай A: отец - левый потомок деда
        if (parent_pt == grand_parent_pt->left) {
            Node *uncle_pt = grand_parent_pt->right;
            
            // Случай 1: дядя красный
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                // Случай 2: узел - правый потомок
                if (pt == parent_pt->right) {
                    rotateLeft(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                
                // Случай 3: узел - левый потомок
                rotateRight(grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        } 
        // Случай B: отец - правый потомок деда
        else {
            Node *uncle_pt = grand_parent_pt->left;
            
            // Случай 1: дядя красный
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                // Случай 2: узел - левый потомок
                if (pt == parent_pt->left) {
                    rotateRight(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                
                // Случай 3: узел - правый потомок
                rotateLeft(grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    
    root->color = BLACK;
}

// Левый поворот
void RBTree::rotateLeft(Node *&pt) {
    Node *pt_right = pt->right;
    
    pt->right = pt_right->left;
    
    if (pt->right != nullptr)
        pt->right->parent = pt;
    
    pt_right->parent = pt->parent;
    
    if (pt->parent == nullptr)
        root = pt_right;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
    else
        pt->parent->right = pt_right;
    
    pt_right->left = pt;
    pt->parent = pt_right;
}

// Правый поворот
void RBTree::rotateRight(Node *&pt) {
    Node *pt_left = pt->left;
    
    pt->left = pt_left->right;
    
    if (pt->left != nullptr)
        pt->left->parent = pt;
    
    pt_left->parent = pt->parent;
    
    if (pt->parent == nullptr)
        root = pt_left;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
    else
        pt->parent->right = pt_left;
    
    pt_left->right = pt;
    pt->parent = pt_left;
}

// Обход дерева (инфиксный)
void RBTree::inorderHelper(Node *root) {
    if (root == nullptr) return;
    
    inorderHelper(root->left);
    cout << root->data << " ";
    inorderHelper(root->right);
}

void RBTree::inorder() { inorderHelper(root); }

// Пример использования
int main() {
    RBTree tree;
    
    tree.insert(7);
    tree.insert(4);
    tree.insert(11);
    tree.insert(1);
    tree.insert(3);
    tree.insert(10);
    tree.insert(12);
    
    cout << "Inorder Traversal: ";
    tree.inorder();
    tree.toGraphVis("tree_1.dot");
    cout << endl;

    tree.insert(5);
    tree.toGraphVis("tree_2.dot");

    tree.insert(6);
    tree.toGraphVis("tree_3.dot");
    tree.insert(9);
    tree.toGraphVis("tree_4.dot");
    tree.insert(8);
    tree.toGraphVis("tree_5.dot");
    
    return 0;
}