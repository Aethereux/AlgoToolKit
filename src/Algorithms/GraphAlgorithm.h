#pragma once
#include <string>
#include <vector>

// Requirements from PDF:
// - Simulate Kruskal's Algorithm (Edge sorting, selection, cycle detection, MST
// formation)
// - Simulate Prim's Algorithm (Starting vertex, Edge selection, Growing MST)
// - Required input: Number of vertices, weighted edges

struct Edge {
  int src, dest, weight;
};

class GraphAlgorithm {
public:
  virtual ~GraphAlgorithm() = default;

  // TODO: Implement a system to accept vertices and edges from the user
  virtual void SetGraphInfo(int numVertices,
                            const std::vector<Edge> &edges) = 0;

  // TODO: Implement Kruskal's algorithm simulation logic
  virtual void SimulateKruskals() = 0;

  // TODO: Implement Prim's algorithm simulation logic
  virtual void SimulatePrims() = 0;
};
