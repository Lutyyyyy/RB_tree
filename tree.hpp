#pragma once
#include <stdexcept>
#include <type_traits>


namespace tree 
{

//template <class T> void destroy(T *p) { p->~T(); }

enum class Color_t { BLACK, RED };

template <typename T = int>
struct Node_t final {

static_assert(std::is_nothrow_move_constructible<T>::value, "RB_tree_t requires nothrow_move_constructible elements");
static_assert(std::is_nothrow_move_assignable<T>::value, "RB_tree_t requires nothrow_move_assignable elements");

public:
    Node_t* left;
    Node_t* right;
    Node_t* p;   

private:
    T key_; 
    Color_t color_;
    unsigned size_;

public:
    Node_t(T key, Node_t* left_, Node_t* right_, Node_t* p_, Color_t color, unsigned size = 1);

    Node_t(Node_t& rhs) = delete;
    Node_t& operator=(Node_t& rhs) = delete;
    Node_t(Node_t&& rhs) noexcept;
    Node_t& operator=(Node_t&& rhs) noexcept;

    ~Node_t() = default;

    T        key()   const { return key_; }
    unsigned size()  const { return size_; }
    Color_t  color() const { return color_; }

    void update_size();
    void update_color(Color_t color) { color_ = color; }
};


template <typename T = int>
class RB_tree_t final {
    
static_assert(std::is_default_constructible<T>::value, "RB_tree_t requires default_constructible elements");

private:
    Node_t<T> nil{T(), nullptr, nullptr, nullptr, Color_t::BLACK, 0};
    Node_t<T> *root = nullptr;

public:    
    RB_tree_t() : root(&nil) {}

    RB_tree_t(RB_tree_t& rhs) = delete;
    RB_tree_t& operator=(RB_tree_t& rhs) = delete; //add copy ctor + asign
    RB_tree_t (RB_tree_t&& rhs);
    RB_tree_t& operator= (RB_tree_t&& rhs);
    ~RB_tree_t();

    void insert (T key);
    void insert_fixup (Node_t<T>* node);

    void left_rotate (Node_t<T>* node);
    void right_rotate (Node_t<T>* node);

    T order_statistic (unsigned pos);
    unsigned nKeys_less_than (T key);
    void dump();
};

} //tree



template <typename T>
tree::Node_t<T>::Node_t (T key, 
                         tree::Node_t<T>* left_, 
                         tree::Node_t<T>* right_, 
                         tree::Node_t<T>* p_, 
                         tree::Color_t color, 
                         unsigned size
                        ) : key_{key}, left(left_), right(right_), p(p_), color_(color), size_(size) {}

template <typename T>
tree::Node_t<T>::Node_t (tree::Node_t<T>&& rhs) noexcept : key_{rhs.key_}, 
                                                           left(rhs.left), 
                                                           right(rhs.right), 
                                                           p(rhs.p), 
                                                           color_(rhs.color_),
                                                           size_(rhs.size_) 
{
    rhs.left_ = nullptr;
    rhs.right_ = nullptr;
    rhs.p_ = nullptr;
}

template <typename T>
tree::Node_t<T>& tree::Node_t<T>::operator= (tree::Node_t<T>&& rhs) noexcept {
    std::swap(key_, rhs.key_);
    std::swap(left, rhs.left);
    std::swap(right, rhs.right);
    std::swap(p, rhs.p);
    std::swap(color_, rhs.color_);
    std::swap(size_, rhs.size_);
    return *this;
}

template <typename T> 
void tree::Node_t<T>::update_size () {
    size_ = 1 + left->size() + right->size();
}

//==============================================================================================================================================================================================================================

template <typename T>
tree::RB_tree_t<T>::RB_tree_t (tree::RB_tree_t<T>&& rhs) : root(rhs.root) {
    rhs.root = nullptr;
}

template <typename T>
tree::RB_tree_t<T>& tree::RB_tree_t<T>::operator= (tree::RB_tree_t<T>&& rhs) {
    std::swap(root, rhs.root);
    rhs.root = nullptr;
    return *this;
}

template <typename T>
tree::RB_tree_t<T>::~RB_tree_t () {
    Node_t<T>* ptr = root;
    
    if (ptr == &nil) return;

    while (root->left != &nil || root->right != &nil) {
        while (ptr->left != &nil || ptr->right != &nil) {
            while (ptr->left != &nil) 
                ptr = ptr->left;
            if (ptr->right != &nil)
                ptr = ptr->right;
        }

        if (ptr == ptr->p->left) {
            ptr = ptr->p;
            delete ptr->left;
            ptr->left = &nil; 
        }
        else if (ptr == ptr->p->right) {
            ptr = ptr->p;
            delete ptr->right;
            ptr->right = &nil; 
        }
    }
    
    delete root;

}

template <typename T> 
void tree::RB_tree_t<T>::insert (T key) {
    Node_t<T>* parent_node = &nil;
    Node_t<T>* current_node = root;
    
    while (current_node != &nil) {
        parent_node = current_node;
        if (key < current_node->key())
            current_node = current_node->left;
        else 
            current_node = current_node->right;
    }
    
    Node_t<T>* node = new Node_t<T>{key, &nil, &nil, parent_node, Color_t::RED};

    if (parent_node == &nil)
        root = node;
    else if (node->key() < parent_node->key()) {
        parent_node->left = node;
    }
    else {
        parent_node->right = node;
    }

    while (parent_node != &nil) {
        parent_node->update_size();
        parent_node = parent_node->p;
    }

    insert_fixup(node);
}


template <typename T> 
void tree::RB_tree_t<T>::insert_fixup (tree::Node_t<T>* node) {
    while (node->p->color() == Color_t::RED) {
        if (node->p == node->p->p->left) {
            Node_t<T>* uncle = node->p->p->right;
            if (uncle->color() == Color_t::RED) {
                node->p->update_color(Color_t::BLACK);
                uncle->update_color(Color_t::BLACK);
                node->p->p->update_color(Color_t::RED);
                node = node->p->p;
            }
            else { 
                if (node == node->p->right) {
                    node = node->p;
                    left_rotate(node);
                }
                node->p->update_color (Color_t::BLACK);
                node->p->p->update_color (Color_t::RED);
                right_rotate(node->p->p);
            }
        }
        else {
            Node_t<T>* uncle = node->p->p->left;
            if (uncle->color() == Color_t::RED) {
                node->p->update_color(Color_t::BLACK);
                uncle->update_color(Color_t::BLACK);
                node->p->p->update_color(Color_t::RED);
                node = node->p->p;
            }
            else { 
                if (node == node->p->left) {
                    node = node->p;
                    right_rotate(node);
                }
                node->p->update_color (Color_t::BLACK);
                node->p->p->update_color (Color_t::RED);
                left_rotate(node->p->p);
            }
        }
    }

    root->update_color(Color_t::BLACK);
}

template <typename T> 
void tree::RB_tree_t<T>::left_rotate (tree::Node_t<T>* node) {
    Node_t<T>* temp = node->right;
    node->right = temp->left;

    if (temp->left != &nil)
        temp->left->p = node;
    temp->p = node->p;
    if (node->p == &nil)
        root = temp;
    else if (node == node->p->left)
        node->p->left = temp;
    else
        node->p->right = temp;
    
    temp->left = node;
    node->p = temp;

    node->update_size();
    temp->update_size();
}


template <typename T> 
void tree::RB_tree_t<T>::right_rotate (tree::Node_t<T>* node) {
    Node_t<T>* temp = node->left;
    node->left = temp->right;

    if (temp->right != &nil)
        temp->right->p = node;
    temp->p = node->p;
    if (node->p == &nil)
        root = temp;
    else if (node == node->p->left)
        node->p->left = temp;
    else
        node->p->right = temp;
    
    temp->right = node;
    node->p = temp;

    node->update_size();
    temp->update_size();
}

template <typename T> 
T tree::RB_tree_t<T>::order_statistic (unsigned k) {
    if (k > root->size()) {
        throw std::runtime_error("Given position is bigger than tree size");
    }

    Node_t<T>* current = root;
    unsigned left_half_size = current->left->size() + 1;

    while (k != left_half_size) {
        if (k < left_half_size) {
            current = current->left;
        }
        else {
            current = current->right;
            k -= left_half_size;
        }
        left_half_size = current->left->size() + 1;
    }

    return current->key();
}

template <typename T> 
unsigned tree::RB_tree_t<T>::nKeys_less_than (T key) {
    if (key > root->size()) {
        throw std::runtime_error("Given position is bigger than tree size");
    }

    Node_t<T>* current = root;
    unsigned left_half_size = current->left->size() + 1;

    while (k != left_half_size) {
        if (k < left_half_size) {
            current = current->left;
        }
        else {
            current = current->right;
            k -= left_half_size;
        }
        left_half_size = current->left->size() + 1;
    }

    return current->key();
}
