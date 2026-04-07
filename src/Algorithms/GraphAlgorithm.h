#pragma once
#include <string>
#include <utility>
#include <vector>

// MST simulation contracts:
// - Input: vertex count + weighted edges.
// - Output: ordered GraphStep snapshots for UI replay.
// - Supported algorithms: Kruskal and Prim.

struct Edge {
  // Undirected weighted edge between src and dest.
  int src, dest, weight;
};

enum class GraphStepType { Consider, Accept, Reject, Done };
enum class GraphAlgorithmType { Kruskal, Prim };

struct GraphStep {
  // Active edge examined in this step.
  int u = -1, v = -1, weight = 0;
  GraphStepType type = GraphStepType::Consider;
  GraphAlgorithmType algorithm = GraphAlgorithmType::Kruskal;
  // Prim-only tree snapshot, used for node coloring.
  std::vector<int> treeMembership;
  // Component assignment snapshot for visualization.
  std::vector<int> componentId;
  // MST edges accepted so far at this point in replay.
  std::vector<std::pair<int, int>> mstEdges;
  int mstCost = 0;
  std::string description;
};

class GraphAlgorithm {
public:
  virtual ~GraphAlgorithm() = default;

  // Sets simulation input graph.
  virtual void SetGraphInfo(int numVertices,
                            const std::vector<Edge> &edges) = 0;
  // Produces step-by-step snapshots for Kruskal replay.
  virtual void SimulateKruskals() {}
  // Produces step-by-step snapshots for Prim replay.
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
