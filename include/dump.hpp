#include <fstream>
#include "tree.hpp"


void enum_link_nodes (std::ofstream& file, tree::Node_t* node, const tree::Node_t* nil)
{
    if (node->left == nil && node->right == nil)
    {
        if (node->color() == tree::Color_t::BLACK)
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#aaaaaa\"];\n";
        else
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#ff7777\"];\n";
        //file << "elem" << node << ":<l>->NIL;\n";
        //file << "elem" << node << ":<r>->NIL;\n";
    }

    else if (node->left != nil && node->right == nil)
    {
        if (node->color() == tree::Color_t::BLACK)
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#aaaaaa\"];\n";
        else
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#ff7777\"];\n";
        file << "elem" << node << ":<l>->elem" << node->left << ":<k>;\n";
        //file << "elem" << node << ":<r>->NIL;\n";
        enum_link_nodes (file, node->left, nil);
    }

    else if (node->left == nil && node->right != nil)
    {
        if (node->color() == tree::Color_t::BLACK)
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#aaaaaa\"];\n";
        else
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#ff7777\"];\n";
        file << "elem" << node << ":<r>->elem" << node->right << ":<k>;\n";
        //file << "elem" << node << ":<l>->NIL;\n";
        enum_link_nodes (file, node->right, nil);
    }

    else if (node->left != nil && node->right != nil)
    {
        if (node->color() == tree::Color_t::BLACK)
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#aaaaaa\"];\n";
        else
            file << "elem" << node << "[label=\"{<k> key: " << node->key() << " | <s> size: " << node->size() << " | {<l> l | <r> r}} \", fillcolor=\"#ff7777\"];\n";
        file << "elem" << node << ":<l>->elem" << node->left << ":<k>;\n";
        file << "elem" << node << ":<r>->elem" << node->right << ":<k>;\n";
        enum_link_nodes (file, node->left, nil);       
        enum_link_nodes (file, node->right, nil);
    }
}

void tree::RB_tree_t::dump(const char* filepath) const {
    std::ofstream file(filepath, std::ios::trunc);    

    file << "digraph G{\nrankdir=HR;\n";
    file << "node [shape=\"record\", fontsize=14, style=\"filled\"];\n";
    file <<"edge [color=\"black\", fontcolor=\"black\", fontsize=12];\n\n\n";
    //file << "NIL [shape=\"circle\", label=\"NIL\", fillcolor=\"#aaaaaa\"];\n";
    enum_link_nodes (file, root, nil);
    //file << "elem" << root << ":<k>->NIL;\n";
    file << "\n}";
} 
