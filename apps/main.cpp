#include <iostream>
#include "tree.hpp"
#include "input_commands.hpp"

#define DUMP_MODE

#ifdef DUMP_MODE
    #include "dump.hpp"       
#endif

int main() try {

    tree::RB_tree_t tree;

    std::vector< std::pair<char, int> > commands = input::get_commands();
    std::vector<int> output;

    for (size_t i = 0; i < commands.size(); ++i) {
        switch (commands[i].first) {
            case input::Commands::KEY:
                tree.insert(commands[i].second);
                break;
            case input::Commands::KTHMIN:
                output.push_back(tree.order_statistic(commands[i].second));
                break;
            case input::Commands::NLESS:
                output.push_back(tree.nKeys_less_than(commands[i].second));
                break;
            default:
                throw std::runtime_error("Invalid input");
        }    
    }

    for (auto v : output) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;

    #ifdef DUMP_MODE
        tree.dump("dump.dot");
        system ("dot dump.dot -T png -o dump.png");
    #endif

    return EXIT_SUCCESS;
}
catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

