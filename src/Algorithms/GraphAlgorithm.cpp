#include "GraphAlgorithm.h"
#include <algorithm>
#include <queue>
#include <tuple>

static std::string FormatEdgeLabel(int a, int b) {
  int lo = std::min(a, b) + 1;
  int hi = std::max(a, b) + 1;
  return "(" + std::to_string(lo) + "," + std::to_string(hi) + ")";
}

void PrimGraph::SetGraphInfo(int numVertices, const std::vector<Edge> &edges) {
  m_NumVertices = numVertices;
  m_Edges = edges;
  m_Steps.clear();
}

void PrimGraph::SimulatePrims() {
  // Prim simulation records each decision as a GraphStep for UI playback.
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

int KruskalGraph::Find(std::vector<int> &parent, int x) {
  // Returns representative root of x in Union-Find.
  while (parent[x] != x)
    x = parent[x];
  return x;
}

bool KruskalGraph::Union(std::vector<int> &parent, int x, int y) {
  // Joins two components; false means the edge would create a cycle.
  int rootX = Find(parent, x);
  int rootY = Find(parent, y);

  if (rootX == rootY)
    return false;

  parent[rootX] = rootY;
  return true;
}

void KruskalGraph::SimulateKruskals() {
  // Kruskal simulation: sort by weight, accept only non-cycling edges.
  m_Steps.clear();

  std::vector<Edge> sortedEdges = m_Edges;
  std::sort(sortedEdges.begin(), sortedEdges.end(),
            [](const Edge &a, const Edge &b) { return a.weight < b.weight; });


  std::vector<int> parent(m_NumVertices);
  for (int i = 0; i < m_NumVertices; i++) {
    parent[i] = i;
  }

  std::vector<int> componentId(m_NumVertices);
  for (int i = 0; i < m_NumVertices; i++) {
    componentId[i] = i;
  }
  std::vector<std::pair<int, int>> mstEdges;
  int mstCost = 0;

  for (const Edge &e : sortedEdges) {
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
      step.type = GraphStepType::Reject;
      step.description = "Skipped " + FormatEdgeLabel(e.src, e.dest) +
                         " (would form a cycle)";
      m_Steps.push_back(step);
    }
  }

  GraphStep done;
  done.type = GraphStepType::Done;
  done.algorithm = GraphAlgorithmType::Kruskal;
  done.componentId = componentId;
  done.mstEdges = mstEdges;
  done.mstCost = mstCost;
  done.description = "MST complete!  Total cost = " + std::to_string(mstCost);
  m_Steps.push_back(done);
}
