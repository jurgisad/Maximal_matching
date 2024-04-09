#include "header.h"
std::vector<std::pair<int, int>> generate_pairs(int n, int m)
{
    std::vector<int> numbers;
    for (int i = 0; i < n; ++i)
    {
        numbers.push_back(i);
    }

    std::vector<std::pair<int, int>> pairs;

    for (int i = 0; i < numbers.size(); i++)
    {
        for (int j = i + 1; j < numbers.size(); j++)
        {
            pairs.emplace_back(i, j);
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(pairs.begin(), pairs.end(), g);

    if (m < pairs.size())
    {
        pairs.resize(m);
    }

    //for (const auto& p : pairs) {
    //std::cout << "(" << p.first << ", " << p.second << ")\n";
    //}

    return pairs;
}

bool read_graph_from_file(const std::string& filename, Blossom& blossomSolver)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    int n, m;
    if (!(file >> n >> m))
    {
        std::cerr << "Error reading the number of vertices and edges from file." << std::endl;
        file.close();
        return false;
    }

    int u, v;
    for (int i = 0; i < m; ++i)
    {
        if (!(file >> u >> v))
        {
            std::cerr << "Error reading edge pair from file." << std::endl;
            file.close();
            return false;
        }
        blossomSolver.add_edge(u, v);
    }

    file.close();
    return true;
}
int run_test(const std::string& filename, int numVertices, int numEdges)
{
    Blossom blossomSolver(numVertices);

    if (!read_graph_from_file(filename, blossomSolver))
    {
        return -1;
    }

    int solution = blossomSolver.solve();

    std::string outputFilename = "solution_" + filename;
    std::ofstream outputFile(outputFilename);
    if (outputFile.is_open())
    {
        outputFile << "Test Case: " << filename << "\n";
        outputFile << "Number of Vertices: " << numVertices << ", Number of Edges: " << numEdges << "\n";
        outputFile << "Solution: " << solution << "\n";
        outputFile << "---------------------------------\n";
        outputFile.close();
        std::cout << "Solution saved to: " << outputFilename << "\n";
    }
    else
    {
        std::cerr << "Error saving solution to file.\n";
    }

    std::ofstream outputFileMatched("graph_solved.txt");
    if (outputFileMatched.is_open())
    {
        for (int i = 0; i < blossomSolver.numVertices; i++)
        {
            if (i <= blossomSolver.mate[i])
            {
                outputFileMatched << i << ' ' << blossomSolver.mate[i] << '\n';
            }
        }
        outputFileMatched.close();
        std::cout << "Matched edges written to graph_solved.txt\n";
    }
    else
    {
        std::cerr << "Error writing matched edges to file.\n";
    }

    return solution;
}
void manual_input(Blossom& blossomSolver, const std::string& filename)
{
    int n, m;
    std::ofstream file(filename);

    if (file.is_open())
    {
        std::cout << "Enter the number of vertices (n): ";
        std::cin >> n;
        file << n << " ";
        std::cout << "Enter the number of edges (m): ";
        std::cin >> m;
        file << m << "\n";

        Blossom blossomSolver(n);

        for (int i = 0; i < m; ++i)
        {
            int u, v;
            std::cout << "Enter edge " << i + 1 << " (format: u v): ";
            std::cin >> u >> v;
            file << u << " " << v << "\n";
            blossomSolver.add_edge(u, v);
        }

        file.close();

        run_test(filename, n, m);
    }
    else
    {
        std::cerr << "Error opening file for manual input.\n";
    }
}
