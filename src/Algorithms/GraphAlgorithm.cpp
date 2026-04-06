#include "GraphAlgorithm.h"
#include <algorithm>

void PrimGraph::SetGraphInfo(int numVertices, const std::vector<Edge> &edges) {
  m_NumVertices = numVertices;
  m_Edges = edges;
  m_Steps.clear();
}

void PrimGraph::SimulatePrims() {
  m_Steps.clear();

  // Todo: Implement Prim's algorithm
}

void KruskalGraph::SetGraphInfo(int numVertices, const std::vector<Edge> &edges) {
  m_NumVertices = numVertices;
  m_Edges = edges;
  m_Steps.clear();
}

// Find the root of node x (used to detect if two nodes are in the same set)
int KruskalGraph::Find(std::vector<int> &parent, int x) {
  while (parent[x] != x)
    x = parent[x];
  return x;
}

// Join the sets of x and y. Returns false if they're already in the same set
// (meaning adding this edge would create a cycle).
bool KruskalGraph::Union(std::vector<int> &parent, int x, int y) {
  int rootX = Find(parent, x);
  int rootY = Find(parent, y);

  if (rootX == rootY)
    return false; // already connected — would form a cycle

  parent[rootX] = rootY; // merge the two sets
  return true;
}

void KruskalGraph::SimulateKruskals() {
  m_Steps.clear();

  // Sort all edges by weight (smallest first)
  std::vector<Edge> sortedEdges = m_Edges;
  std::sort(sortedEdges.begin(), sortedEdges.end(),
            [](const Edge &a, const Edge &b) { return a.weight < b.weight; });

  // Initialize Union-Find — each node starts as its own parent
  std::vector<int> parent(m_NumVertices);
  for (int i = 0; i < m_NumVertices; i++) {
    parent[i] = i;
  }

  // Track which component (color) each node belongs to for visualization
  std::vector<int> componentId(m_NumVertices);
  for (int i = 0; i < m_NumVertices; i++) {
    componentId[i] = i;
  }
  std::vector<std::pair<int, int>> mstEdges;
  int mstCost = 0;

  // Process each edge in order of weight
  for (const Edge &e : sortedEdges) {

    // Record a "considering" step before deciding
    GraphStep step;
    step.u = e.src;
    step.v = e.dest;
    step.weight = e.weight;
    step.type = GraphStepType::Consider;
    step.componentId = componentId;
    step.mstEdges = mstEdges;
    step.mstCost = mstCost;
    step.description = "Considering edge " + std::to_string(e.src + 1) +
                       " -- " + std::to_string(e.dest + 1) +
                       "  (weight " + std::to_string(e.weight) + ")";
    m_Steps.push_back(step);

    if (Union(parent, e.src, e.dest)) {
      // No cycle — add edge to MST
      // Merge component colors so connected nodes share the same color
      int keepColor = componentId[e.src];
      int mergeColor = componentId[e.dest];
      for (int i = 0; i < m_NumVertices; i++) {
        if (componentId[i] == mergeColor)
          componentId[i] = keepColor;
      }

      mstEdges.push_back({e.src, e.dest});
      mstCost += e.weight;

      step.type = GraphStepType::Accept;
      step.componentId = componentId;
      step.mstEdges = mstEdges;
      step.mstCost = mstCost;
      step.description = "Added edge " + std::to_string(e.src + 1) +
                         " -- " + std::to_string(e.dest + 1) +
                         " to MST  (+" + std::to_string(e.weight) +
                         ", total = " + std::to_string(mstCost) + ")";
      m_Steps.push_back(step);
    } else {
      // Would form a cycle — skip this edge
      step.type = GraphStepType::Reject;
      step.description = "Skipped " + std::to_string(e.src + 1) +
                         " -- " + std::to_string(e.dest + 1) +
                         "  (would form a cycle)";
      m_Steps.push_back(step);
    }
  }

  // Final step — MST is complete
  GraphStep done;
  done.type = GraphStepType::Done;
  done.componentId = componentId;
  done.mstEdges = mstEdges;
  done.mstCost = mstCost;
  done.description = "MST complete!  Total cost = " + std::to_string(mstCost);
  m_Steps.push_back(done);
}
