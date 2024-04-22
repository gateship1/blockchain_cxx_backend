#include <iostream>
#include <string>

#include "blockchain.hpp"

auto main() -> int {

    Blockchain blockchain;

    std::string block_data;
    size_t chain_difficult { blockchain.get_difficulty() };
    size_t max_iterations{ blockchain.get_max_iterations() };

    bool block_it_up = true;
    while (block_it_up != false) {
        std::cout << "\n*******************************\n";
        std::cout << " 1 - Add block.\n";
        std::cout << " 2 - Change difficulty.\n";
        std::cout << " 3 - Change maximum iterations.\n";
        std::cout << " 4 - Print the blockchain.\n";
        std::cout << " 5 - Print mining info.\n";
        std::cout << " 6 - Exit.\n";
        std::cout << " Enter your choice and press return: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter block data: \n";
            {
                char c;
                std::cin.get(c);
            }
            std::cin.clear();
            std::getline(std::cin,block_data);
            if (blockchain.mine(static_cast<std::string>(block_data)))
            {
                auto last_block{blockchain.get_end_of_chain()};
                std::cout << "\nBlock mined:\n"
                          << last_block << "\n";
            }
            else 
            {
                std::cout << "\nMaximum iterations exceed! - Block not mined\n" << "\n";
            }
            break;
        case 2:
            std::cout << "Enter difficulty: \n";
            std::cin >> chain_difficult;
            blockchain.set_difficulty(chain_difficult);
            break;
        case 3:
            std::cout << "Enter maximum iterations: \n";
            std::cin >> max_iterations;
            blockchain.set_max_iterations(max_iterations);
            break;
        case 4:
            std::cout << "--------------------\nBlockchain: \n";
            for (size_t i = blockchain.get_chain_length() - 1; blockchain.get_chain_length() > i; --i) {
                auto block{blockchain.get_block(i)};
                std::cout << block << "\n";
            }
            std::cout << "--------------------\n";
            break;
        case 5:
            std::cout << "--------------------\nMining Info: \n";
            std::cout << "Mining Difficulty: " << blockchain.get_difficulty() << "\n";
            std::cout << "Maximum Iterations: " << blockchain.get_max_iterations() << "\n";
            std::cout << "--------------------\n";
            break;
        case 6:
            std::cout << "End of Simulation.\n";
            block_it_up = false;
            break;
        default:
            std::cout << "Not a Valid Choice. \n";
            break;
        }
    }

    return 0;

}
