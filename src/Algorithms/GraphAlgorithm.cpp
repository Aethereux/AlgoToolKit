#include "GraphAlgorithm.h"
#include <algorithm>
#include <queue>
#include <tuple>

namespace {
std::string FormatEdgeLabel(int a, int b) {
  int lo = std::min(a, b) + 1;
  int hi = std::max(a, b) + 1;
  return "(" + std::to_string(lo) + "," + std::to_string(hi) + ")";
}
} // namespace

void PrimGraph::SetGraphInfo(int numVertices, const std::vector<Edge> &edges) {
  m_NumVertices = numVertices;
  m_Edges = edges;
  m_Steps.clear();
}

void PrimGraph::SimulatePrims() {
  m_Steps.clear();

  if (m_NumVertices <= 0)
    return;

  std::vector<std::vector<std::pair<int, int>>> adj(m_NumVertices);
  for (const Edge &e : m_Edges) {
    if (e.src < 0 || e.src >= m_NumVertices || e.dest < 0 ||
        e.dest >= m_NumVertices)
      continue;
    adj[e.src].push_back({e.dest, e.weight});
    adj[e.dest].push_back({e.src, e.weight});
  }

  std::vector<bool> inTree(m_NumVertices, false);
  std::vector<int> componentId(m_NumVertices);
  for (int i = 0; i < m_NumVertices; ++i)
    componentId[i] = i;

  std::vector<std::pair<int, int>> mstEdges;
  int mstCost = 0;
  int componentsBuilt = 0;

  std::priority_queue<std::tuple<int, int, int>,
                      std::vector<std::tuple<int, int, int>>,
                      std::greater<std::tuple<int, int, int>>>
      frontier;

  auto buildTreeMembership = [&]() {
    std::vector<int> membership(m_NumVertices, 0);
    for (int i = 0; i < m_NumVertices; ++i)
      membership[i] = inTree[i] ? 1 : 0;
    return membership;
  };

  for (int start = 0; start < m_NumVertices; ++start) {
    if (inTree[start])
      continue;

    const int colorId = start;
    inTree[start] = true;
    componentId[start] = colorId;
    ++componentsBuilt;

    for (const auto &next : adj[start]) {
      if (!inTree[next.first])
        frontier.push({next.second, start, next.first});
    }

    while (!frontier.empty()) {
      const auto [w, u, v] = frontier.top();
      frontier.pop();

      GraphStep step;
      step.u = u;
      step.v = v;
      step.weight = w;
      step.type = GraphStepType::Consider;
      step.algorithm = GraphAlgorithmType::Prim;
      step.treeMembership = buildTreeMembership();
      step.componentId = componentId;
      step.mstEdges = mstEdges;
      step.mstCost = mstCost;
      step.description = "Considering edge " + FormatEdgeLabel(u, v) +
                         " (weight " + std::to_string(w) + ")";
      m_Steps.push_back(step);

      if (inTree[v]) {
        step.type = GraphStepType::Reject;
        step.description = "Skipped " + FormatEdgeLabel(u, v) +
                           " (destination already in tree)";
        m_Steps.push_back(step);
        continue;
      }

      inTree[v] = true;
      componentId[v] = colorId;
      mstEdges.push_back({u, v});
      mstCost += w;

      step.type = GraphStepType::Accept;
      step.treeMembership = buildTreeMembership();
      step.componentId = componentId;
      step.mstEdges = mstEdges;
      step.mstCost = mstCost;
      step.description = "Added edge " + FormatEdgeLabel(u, v) + " to MST (+" +
                         std::to_string(w) + ", total = " +
                         std::to_string(mstCost) + ")";
      m_Steps.push_back(step);

      for (const auto &next : adj[v]) {
        if (!inTree[next.first])
          frontier.push({next.second, v, next.first});
      }
    }
  }

  GraphStep done;
  done.type = GraphStepType::Done;
  done.algorithm = GraphAlgorithmType::Prim;
  done.treeMembership = buildTreeMembership();
  done.componentId = componentId;
  done.mstEdges = mstEdges;
  done.mstCost = mstCost;
  if (componentsBuilt <= 1) {
    done.description = "Prim complete! Total cost = " + std::to_string(mstCost);
  } else {
    done.description = "Prim complete (forest with " +
                       std::to_string(componentsBuilt) +
                       " components). Total cost = " + std::to_string(mstCost);
  }
  m_Steps.push_back(done);
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
    step.algorithm = GraphAlgorithmType::Kruskal;
    step.componentId = componentId;
    step.mstEdges = mstEdges;
    step.mstCost = mstCost;
    step.description = "Considering edge " + FormatEdgeLabel(e.src, e.dest) +
               " (weight " + std::to_string(e.weight) + ")";
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
      step.description = "Added edge " + FormatEdgeLabel(e.src, e.dest) +
                         " to MST (+" + std::to_string(e.weight) +
                         ", total = " + std::to_string(mstCost) + ")";
      m_Steps.push_back(step);
    } else {
      // Would form a cycle — skip this edge
      step.type = GraphStepType::Reject;
      step.description = "Skipped " + FormatEdgeLabel(e.src, e.dest) +
                         " (would form a cycle)";
      m_Steps.push_back(step);
    }
  }

  // Final step — MST is complete
  GraphStep done;
  done.type = GraphStepType::Done;
  done.algorithm = GraphAlgorithmType::Kruskal;
  done.componentId = componentId;
  done.mstEdges = mstEdges;
  done.mstCost = mstCost;
  done.description = "MST complete!  Total cost = " + std::to_string(mstCost);
  m_Steps.push_back(done);
}
