#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>


namespace input 
{

enum Commands : char {KEY = 'k', KTHMIN = 'm', NLESS = 'n'};

void handle_input_error() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if ((std::cin >> std::ws).eof()) {
      throw std::runtime_error("Input: EOF reached");
    }
}

template<typename T> 
T get_val() {
    
    T value;

    std::cin >> value;

    while(!(std::cin && (std::isspace(std::cin.peek()) || std::cin.eof()))) {
        handle_input_error();
        std::cin >> value;
    }

    return value;
}

template<> 
char get_val() {
    
    char value;

    std::cin >> value;

    if (std::cin.eof()) {
        throw std::runtime_error("Input: EOF reached");
    }

    while((!std::cin) || (!std::isspace(std::cin.peek()))) {
        handle_input_error();
        std::cin >> value;
    }

    if ( value != Commands::KEY && value != Commands::KTHMIN && value != Commands::NLESS ) {
        std::cout << "Available commands:\n'k' - insert key\n'm' - kth order statistics\n'n' - number of elements less than given number" << std::endl;
        handle_input_error();
        std::cin >> value;
    }

    return value;
}


std::vector< std::pair<char, int> > get_commands() {

    if ((std::cin >> std::ws).eof()) {
        throw std::runtime_error("empty file");
    }

    std::vector< std::pair<char, int> > commands;

    while (!(std::cin >> std::ws).eof()) {
        char command = get_val<char>();
        int argument = get_val<int>();
        commands.push_back(std::make_pair(command, argument));
    }

    return commands;
}

} // input