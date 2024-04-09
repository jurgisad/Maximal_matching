#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void generate_pairs(int n, int m, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << n << " " << m << std::endl;

    // Generate random pairs of vertices
    for (int i = 0; i < m; ++i) {
        int u = rand() % n;
        int v = rand() % n;
        file << u << " " << v << std::endl;
    }

    file.close();
}

int main() {
    // Seed the random number generator
    std::srand(std::time(0));

    // Generate 10 text files for each set of vertices
    for (int numVertices = 500; numVertices <= 1000; numVertices += 50) {
        for (int i = 0; i < 10; ++i) {
            int numEdges = (numVertices*(numVertices-1))/2;
            std::string filename = "graph3_" + std::to_string(numVertices) + "_" + std::to_string(i) + ".txt";

            generate_pairs(numVertices, numEdges, filename);

            std::cout << "File generated: " << filename << std::endl;
        }
    }

    return 0;
}
