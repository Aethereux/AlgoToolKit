#pragma once
#include <string>
#include <utility>
#include <vector>

// Requirements from PDF:
// - Simulate Kruskal's Algorithm (Edge sorting, selection, cycle detection, MST
// formation)
// - Simulate Prim's Algorithm (Starting vertex, Edge selection, Growing MST)
// - Required input: Number of vertices, weighted edges

struct Edge {
  int src, dest, weight;
};

enum class GraphStepType { Consider, Accept, Reject, Done };
enum class GraphAlgorithmType { Kruskal, Prim };

struct GraphStep {
  int u = -1, v = -1, weight = 0;
  GraphStepType type = GraphStepType::Consider;
  GraphAlgorithmType algorithm = GraphAlgorithmType::Kruskal;
  std::vector<int> treeMembership;           // Prim: 1 if node is in current tree
  std::vector<int> componentId;              // per-node component ID for coloring
  std::vector<std::pair<int, int>> mstEdges; // accepted edges so far
  int mstCost = 0;
  std::string description;
};

class GraphAlgorithm {
public:
  virtual ~GraphAlgorithm() = default;

  virtual void SetGraphInfo(int numVertices,
                            const std::vector<Edge> &edges) = 0;
  virtual void SimulateKruskals() {} 
  virtual void SimulatePrims()    {}
};

class KruskalGraph : public GraphAlgorithm {
public:
  void SetGraphInfo(int numVertices, const std::vector<Edge> &edges) override;
  void SimulateKruskals() override;

  const std::vector<GraphStep> &GetSteps() const { return m_Steps; }
  int GetVertexCount() const { return m_NumVertices; }
  const std::vector<Edge> &GetEdges() const { return m_Edges; }

private:
  // Union-Find helpers for cycle detection
  int Find(std::vector<int> &parent, int x);
  bool Union(std::vector<int> &parent, int x, int y);

  int m_NumVertices = 0;
  std::vector<Edge> m_Edges;
  std::vector<GraphStep> m_Steps;
};

class PrimGraph : public GraphAlgorithm {
public:
  void SetGraphInfo(int numVertices, const std::vector<Edge> &edges) override;
  void SimulatePrims() override;

  const std::vector<GraphStep> &GetSteps() const { return m_Steps; }
  int GetVertexCount() const { return m_NumVertices; }
  const std::vector<Edge> &GetEdges() const { return m_Edges; }

private:
  int m_NumVertices = 0;
  std::vector<Edge> m_Edges;
  std::vector<GraphStep> m_Steps;
};
