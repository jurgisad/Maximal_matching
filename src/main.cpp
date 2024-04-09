#include "header.h"

int main()
{
    int option;

    std::cout << "Choose an option:\n";
    std::cout << "1. Use a txt file\n";
    std::cout << "2. Manually input n, m, and edges\n";
    std::cout << "3. Generate n m graph\n";
    std::cout << "Enter the option number (1, 2, or 3): ";
    std::cin >> option;

    switch (option)
    {
    case 1:
    {
        std::string filename;
        std::cout << "Enter the filename: ";
        std::cin >> filename;

        std::ifstream file(filename);
        int n, m;
        if (file >> n >> m)
        {
            file.close();
            run_test(filename, n, m);
        }
        else
        {
            std::cerr << "Error reading n and m from the file.\n";
        }
    }
    break;
    case 2:
    {
        Blossom blossomSolver(0);
        std::string filename = "manual_input.txt";
        manual_input(blossomSolver, filename);
    }
    break;
    case 3:
    {
        int n, m;
        std::cout << "Enter the number of vertices (n): ";
        std::cin >> n;
        std::cout << "Enter the number of edges (m): ";
        std::cin >> m;
        auto edges = generate_pairs(n, m);
        std::ofstream generatedGraphFile("generated_graph.txt");
        if (generatedGraphFile.is_open())
        {
            generatedGraphFile << n << " " << m << "\n";
            for (const auto& edge : edges)
            {
                generatedGraphFile << edge.first << " " << edge.second << "\n";
            }
            generatedGraphFile.close();
            std::cout << "Generated graph saved to generated_graph.txt\n";
        }
        else
        {
            std::cerr << "Error saving generated graph to file.\n";
            break;
        }
        run_test("generated_graph.txt", n, m);
    }
    break;
    default:
        std::cerr << "Invalid option. Exiting.\n";
        return 1;
    }
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
