#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <string>

std::vector<std::pair<int, int>> generate_pairs(int n, int m);

struct Blossom
{
    int numVertices, numIDs;
    std::vector<int> mate;
    std::vector<std::vector<int>> blossomVertices; //<- atitikmuo b
    std::vector<int> blossomParent, depth, blossomLabel;
    std::vector<std::vector<int>> unmatchedEdges;

    Blossom(int n) : numVertices(n)
    {
        numIDs = n + n / 2;
        mate.assign(n, -1);
        blossomVertices.resize(numIDs);
        blossomParent.resize(numIDs);
        depth.resize(numIDs);
        blossomLabel.resize(numIDs);
        unmatchedEdges.assign(numIDs, std::vector<int>(numIDs, -1));
    }

    void add_edge(int u, int v)
    {
        unmatchedEdges[u][v] = u;
        unmatchedEdges[v][u] = v;
    }

    void match(int u, int v)
    {
        unmatchedEdges[u][v] = unmatchedEdges[v][u] = -1;
        mate[u] = v;
        mate[v] = u;
    }

    std::vector<int> trace(int x)
    {
        std::vector<int> verticesInBlossom;
        while (true)
        {
            while (blossomLabel[x] != x) x = blossomLabel[x];
            if (!verticesInBlossom.empty() && verticesInBlossom.back() == x) break;
            verticesInBlossom.push_back(x);
            x = blossomParent[x];
        }
        return verticesInBlossom;
    }

    void contract(int c, int x, int y, std::vector<int>& verticesInX, std::vector<int>& verticesInY)
    {
        blossomVertices[c].clear();
        int representativeVertex = verticesInX.back();
        while (!verticesInX.empty() && !verticesInY.empty() && verticesInX.back() == verticesInY.back())
        {
            representativeVertex = verticesInX.back();
            verticesInX.pop_back();
            verticesInY.pop_back();
        }
        blossomVertices[c].push_back(representativeVertex);
        blossomVertices[c].insert(blossomVertices[c].end(), verticesInX.rbegin(), verticesInX.rend());
        blossomVertices[c].insert(blossomVertices[c].end(), verticesInY.begin(), verticesInY.end());
        for (int i = 0; i <= c; i++)
        {
            unmatchedEdges[c][i] = unmatchedEdges[i][c] = -1;
        }
        for (int z : blossomVertices[c])
        {
            blossomLabel[z] = c;
            for (int i = 0; i < c; i++)
            {
                if (unmatchedEdges[z][i] != -1)
                {
                    unmatchedEdges[c][i] = z;
                    unmatchedEdges[i][c] = unmatchedEdges[i][z];
                }
            }
        }
    }

    std::vector<int> lift(std::vector<int>& verticesInX)
    {
        std::vector<int> A;
        while (verticesInX.size() >= 2)
        {
            int z = verticesInX.back();
            verticesInX.pop_back();
            if (z < numVertices)
            {
                A.push_back(z);
                continue;
            }
            int w = verticesInX.back();
            int i = (A.size() % 2 == 0 ? std::find(blossomVertices[z].begin(), blossomVertices[z].end(), unmatchedEdges[z][w]) - blossomVertices[z].begin() : 0);
            int j = (A.size() % 2 == 1 ? std::find(blossomVertices[z].begin(), blossomVertices[z].end(), unmatchedEdges[z][A.back()]) - blossomVertices[z].begin() : 0);
            int k = blossomVertices[z].size();
            int dif = (A.size() % 2 == 0 ? i % 2 == 1 : j % 2 == 0) ? 1 : k - 1;
            while (i != j)
            {
                verticesInX.push_back(blossomVertices[z][i]);
                i = (i + dif) % k;
            }
            verticesInX.push_back(blossomVertices[z][i]);
        }
        return A;
    }

    int solve()
    {
        for (int ans = 0;; ans++)
        {
            std::fill(depth.begin(), depth.end(), 0);
            std::queue<int> Q;
            for (int i = 0; i < numIDs; i++) blossomLabel[i] = i;
            for (int i = 0; i < numVertices; i++)
            {
                if (mate[i] == -1)
                {
                    Q.push(i);
                    blossomParent[i] = i;
                    depth[i] = 1;
                }
            }
            int c = numVertices;
            bool aug = false;
            while (!Q.empty() && !aug)
            {
                int x = Q.front();
                Q.pop();
                if (blossomLabel[x] != x) continue;
                for (int y = 0; y < c; y++)
                {
                    if (blossomLabel[y] == y && unmatchedEdges[x][y] != -1)
                    {
                        if (depth[y] == 0)
                        {
                            blossomParent[y] = x;
                            depth[y] = 2;
                            blossomParent[mate[y]] = y;
                            depth[mate[y]] = 1;
                            Q.push(mate[y]);
                        }
                        else if (depth[y] == 1)
                        {
                            std::vector<int> verticesInX = trace(x);
                            std::vector<int> verticesInY = trace(y);
                            if (verticesInX.back() == verticesInY.back())
                            {
                                contract(c, x, y, verticesInX, verticesInY);
                                Q.push(c);
                                blossomParent[c] = blossomParent[blossomVertices[c][0]];
                                depth[c] = 1;
                                c++;
                            }
                            else
                            {
                                aug = true;
                                verticesInX.insert(verticesInX.begin(), y);
                                verticesInY.insert(verticesInY.begin(), x);
                                std::vector<int> A = lift(verticesInX);
                                std::vector<int> B = lift(verticesInY);
                                A.insert(A.end(), B.rbegin(), B.rend());
                                for (int i = 0; i < (int)A.size(); i += 2)
                                {
                                    match(A[i], A[i + 1]);
                                    if (i + 2 < (int)A.size()) add_edge(A[i + 1], A[i + 2]);
                                }
                            }
                            break;
                        }
                    }
                }
            }
            if (!aug) return ans;
        }
    }
};

bool read_graph_from_file(const std::string& filename, Blossom& blossomSolver);

int run_test(const std::string& filename, int numVertices, int numEdges);

void manual_input(Blossom& blossomSolver, const std::string& filename);

#endif // HEADER_H
