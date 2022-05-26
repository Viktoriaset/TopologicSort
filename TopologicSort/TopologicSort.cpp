#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Vertex {
    int numberInGraph;
    int groupStatus = 0;
    bool isVisited = false;
    std::vector<int> edge_to;
    std::vector<int> edge_from;

    void addEdgeFrom(int v) {
        if (edge_from.size() == 0) {
            groupStatus = 1;
        }

        edge_from.push_back(v);
    }

    void addEdgeTo(int v) {
        edge_to.push_back(v);
    }
};

bool Comporator(Vertex l, Vertex r) {
    if (l.groupStatus < r.groupStatus) {
        return true;
    }
    else if (l.groupStatus == r.groupStatus) {
        if (l.numberInGraph < r.numberInGraph) {
            return true;
        }
    }

    return false;
}

template<typename Type>
void MergeSort(Type* buf, size_t l, size_t r, bool (*comp)(Type l, Type r));
template<typename Type>
static void merge(Type* buf, size_t l, size_t r, size_t m, bool (*comp)(Type l, Type r));

template<typename Type>
void MergeSort(std::vector<Type>& buf, size_t l, size_t r, bool (*comp)(Type l, Type r))
{

    if (l >= r) return;

    size_t m = (l + r) / 2;


    MergeSort(buf, l, m, comp);
    MergeSort(buf, m + 1, r, comp);
    merge(buf, l, r, m, comp);
}

template<typename Type>
static void merge(std::vector<Type>& buf, size_t l, size_t r, size_t m, bool (*comp)(Type l, Type r))
{
    if (l >= r || m < l || m > r) return;
    if (r == l + 1 && comp(buf[r], buf[l])) { //buf[l] > buf[r]
        std::swap(buf[l], buf[r]);
        return;
    }

    std::vector<Type> tmp(&buf[l], &buf[r] + 1);

    for (size_t i = l, j = 0, k = m - l + 1; i <= r; ++i) {
        if (j > m - l) {
            buf[i] = tmp[k++];
        }
        else if (k > r - l) {
            buf[i] = tmp[j++];
        }
        else {
            buf[i] = (comp(tmp[j], tmp[k])) ? tmp[j++] : tmp[k++]; //tmp[j] < tmp[k]
        }
    }
}

void DFT(std::vector<Vertex>& graph, int vertex) {
    if (graph[vertex].isVisited) {
        return;
    }

    graph[vertex].isVisited = true;

    for (int i = 0; i < graph[vertex].edge_to.size(); i++) {
        int index_next_vertex = graph[vertex].edge_to[i];
        graph[index_next_vertex].groupStatus = std::max(graph[index_next_vertex].groupStatus, graph[vertex].groupStatus + 1);
        DFT(graph, index_next_vertex);
    }

    return;
}

int main()
{
    std::ifstream inp("input.txt");
    std::ofstream out("output.txt");

    int n, m;

    inp >> n >> m;

    std::vector<Vertex> graph(n + 1);// graph[0] don't will use

    for (int i = 1; i < graph.size(); i++) {
        graph[i].numberInGraph = i;
    }

    for (int i = 0; i < m; i++) {
        int vertex_from, vertex_to;
        inp >> vertex_from >> vertex_to;
        graph[vertex_from].addEdgeTo(vertex_to);
        graph[vertex_to].addEdgeFrom(vertex_from);
    }

    for (int i = 1; i < graph.size(); i++) {
        if (graph[i].groupStatus == 0) {
            DFT(graph, i);
        }
    }


    MergeSort<Vertex>(graph, 1, graph.size() - 1, Comporator);

    for (int i = 1; i < graph.size(); i++) {
        out << graph[i].numberInGraph << ' ';
    }

    return 0;
}

