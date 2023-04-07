#pragma once

#include <queue>
#include <stdexcept>


namespace tree 
{

const int POIZON = 333;

enum class Color_t { BLACK, RED };


struct Node_t final {

private:
    int key_; 

public:
    Node_t* left;
    Node_t* right;
    Node_t* p;   

private:
    Color_t color_;
    unsigned size_;

public:
    Node_t(int key, Node_t* left_, Node_t* right_, Node_t* p_, Color_t color, unsigned size = 1);

    Node_t(Node_t&) = delete;
    Node_t& operator=(Node_t&) = delete;
    Node_t(Node_t&& rhs) noexcept;
    Node_t& operator=(Node_t&& rhs) noexcept;

    ~Node_t() = default;

    int      key()   const { return key_; }
    unsigned size()  const { return size_; }
    Color_t  color() const { return color_; }

    void update_size();
    void update_color (Color_t color) { color_ = color; }

}; //Node_t


class RB_tree_t final {

private:
    Node_t *root;

public:    
    RB_tree_t() : root(nullptr) {}

    RB_tree_t(const RB_tree_t& rhs);
    RB_tree_t& operator=(const RB_tree_t& rhs);
    RB_tree_t (RB_tree_t&& rhs) noexcept;
    RB_tree_t& operator= (RB_tree_t&& rhs) noexcept;
    ~RB_tree_t();

    const Node_t* get_root() const { return root; }

    void insert (int key);
    void insert_fixup (Node_t* node);
    void left_rotate (Node_t* node);
    void right_rotate (Node_t* node);

    int order_statistic (unsigned pos) const;
    unsigned nKeys_less_than (int key) const;
    
    void dump(const char* filepath) const;

}; //RB_tree_t

} //tree


tree::Node_t::Node_t (int key, 
                      tree::Node_t* left_, 
                      tree::Node_t* right_, 
                      tree::Node_t* p_, 
                      tree::Color_t color, 
                      unsigned size
                     ) : key_{key}, left(left_), right(right_), p(p_), color_(color), size_(size) {}

tree::Node_t::Node_t (tree::Node_t&& rhs) noexcept : key_(rhs.key_), 
                                                     left(rhs.left), 
                                                     right(rhs.right), 
                                                     p(rhs.p), 
                                                     color_(rhs.color_),
                                                     size_(rhs.size_) 
{
    rhs.left = nullptr;
    rhs.right = nullptr;
    rhs.p = nullptr;
}

tree::Node_t& tree::Node_t::operator= (tree::Node_t&& rhs) noexcept {
    std::swap(key_, rhs.key_);
    std::swap(left, rhs.left);
    std::swap(right, rhs.right);
    std::swap(p, rhs.p);
    std::swap(color_, rhs.color_);
    std::swap(size_, rhs.size_);
    return *this;
}

void tree::Node_t::update_size() {
    size_ = 1;
    if (left) 
        size_ += left->size();
    if (right) 
        size_ += right->size();
}

tree::RB_tree_t::RB_tree_t (const tree::RB_tree_t& rhs) {
    root = new Node_t{rhs.root->key(), nullptr, nullptr, nullptr, Color_t::BLACK, rhs.root->size()};
    
    std::queue< std::pair<Node_t*, const Node_t*> > node_queue; //queue <tmp node, rhs node>
    node_queue.push(std::make_pair(root, rhs.root));

    while (!node_queue.empty()) {
        Node_t* node = node_queue.front().first; 
        const Node_t* rhs_node = node_queue.front().second;
        node_queue.pop();

        if (rhs_node->left != nullptr) {
            node->left = new Node_t{rhs_node->left->key(), 
                                    nullptr, 
                                    nullptr, 
                                    node, 
                                    rhs_node->left->color(), 
                                    rhs_node->left->size()
                                   };
            node_queue.push(std::make_pair(node->left, rhs_node->left));
        }

        if (rhs_node->right != nullptr) {
            node->right = new Node_t{rhs_node->right->key(), 
                                     nullptr, 
                                     nullptr, 
                                     node, 
                                     rhs_node->right->color(), 
                                     rhs_node->right->size()
                                    };
            node_queue.push(std::make_pair(node->right, rhs_node->right));
        }
    }
}

tree::RB_tree_t& tree::RB_tree_t::operator= (const tree::RB_tree_t& rhs) {
    RB_tree_t temp(rhs);
    std::swap(*this, temp);
    return *this;
}

tree::RB_tree_t::RB_tree_t (tree::RB_tree_t&& rhs) noexcept : root(rhs.root) {
    rhs.root = nullptr;
}

tree::RB_tree_t& tree::RB_tree_t::operator= (tree::RB_tree_t&& rhs) noexcept {
    std::swap(root, rhs.root);
    rhs.root = nullptr;
    return *this;
}

tree::RB_tree_t::~RB_tree_t () {
    Node_t* ptr = root;
    
    if (root == nullptr) return;

    while (root->left != nullptr || root->right != nullptr) {
        while (ptr->left != nullptr || ptr->right != nullptr) {
            while (ptr->left != nullptr) 
                ptr = ptr->left;
            if (ptr->right != nullptr)
                ptr = ptr->right;
        }

        if (ptr == ptr->p->left) {
            ptr = ptr->p;
            delete ptr->left;
            ptr->left = nullptr; 
        }
        else if (ptr == ptr->p->right) {
            ptr = ptr->p;
            delete ptr->right;
            ptr->right = nullptr; 
        }
    }
    
    delete root;

}

void tree::RB_tree_t::insert (int key) {
    Node_t* parent_node = nullptr;
    Node_t* current_node = root;
    
    while (current_node != nullptr) {
        parent_node = current_node;
        if (key < current_node->key())
            current_node = current_node->left;
        else 
            current_node = current_node->right;
    }
    
    Node_t* node = new Node_t{key, nullptr, nullptr, parent_node, Color_t::RED};

    if (parent_node == nullptr)
        root = node;
    else if (node->key() < parent_node->key()) {
        parent_node->left = node;
    }
    else {
        parent_node->right = node;
    }

    while (parent_node != nullptr) {
        parent_node->update_size();
        parent_node = parent_node->p;
    }

    insert_fixup(node);
}

void tree::RB_tree_t::insert_fixup (tree::Node_t* node) {
    while (node->p != nullptr && node->p->color() == Color_t::RED) {
        if (node->p == node->p->p->left) {
            Node_t* uncle = node->p->p->right;
            if (uncle != nullptr && uncle->color() == Color_t::RED) {
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
            Node_t* uncle = node->p->p->left;
            if (uncle != nullptr && uncle->color() == Color_t::RED) {
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

void tree::RB_tree_t::left_rotate (tree::Node_t* node) {
    Node_t* temp = node->right;
    node->right = temp->left;

    if (temp->left != nullptr)
        temp->left->p = node;
    temp->p = node->p;
    if (node->p == nullptr)
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

void tree::RB_tree_t::right_rotate (tree::Node_t* node) {
    Node_t* temp = node->left;
    node->left = temp->right;

    if (temp->right != nullptr)
        temp->right->p = node;
    temp->p = node->p;
    if (node->p == nullptr)
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

int tree::RB_tree_t::order_statistic (unsigned k) const {
    if (k > root->size()) {
        throw std::runtime_error("Given position is bigger than tree size");
    }
    if (k == 0) {
        throw std::runtime_error("k must be greater than 0");
    }

    Node_t* current = root;
    unsigned left_half_size = 1;
    if (current->left != nullptr)
        left_half_size += current->left->size();

    while (k != left_half_size) {
        if (k < left_half_size) {
            current = current->left;
        }
        else {
            current = current->right;
            k -= left_half_size;
        }

        if (current->left != nullptr)
            left_half_size += current->left->size();
        else
            left_half_size = 1;
    }

    return current->key();
}

unsigned tree::RB_tree_t::nKeys_less_than (int key) const {
    unsigned result = 0;
    Node_t* current = root;
    
    while (current != nullptr) {    
        if (key == current->key()) {
            if (current->left != nullptr) 
                result += current->left->size();
            break;
        }
        else if (key < current->key()) {
            current = current->left;
        }
        else {
            result += 1;
            if (current->left != nullptr) 
                result += current->left->size();
            current = current->right;
        }
    }

    return result;
}

