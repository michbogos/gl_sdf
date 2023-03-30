#include<vector>
#include<iostream>

#define MAX_SIZE 10000

struct Attribute{
    int id;
    int variant;
    bool isInput;
    int parent;
};

struct Node{
    int id;
    int type;
    std::vector<Attribute> inputAttributes = {};
    std::vector<Attribute> outputAttributes = {};
};


class Graph
{
private:
    /* data */
public:
    std::vector<std::vector<int>> g;
    std::vector<Attribute> attributes;
    std::vector<Node> nodes;
    std::vector<int> connections;
    Graph();
    void addNode(int id, int type);
    void addInputAttribute(int id, int nodeId, int variant);
    void addOutputAttribute(int id, int nodeId, int variant);
    void connect(int u, int v, int id);
    ~Graph();
};

Graph::Graph()
{
    this->g.resize(MAX_SIZE);
    this->attributes.resize(MAX_SIZE);
    this->nodes.resize(MAX_SIZE);
}

void Graph::addNode(int id, int type){
    this->nodes[id] = {id, type};
}

void Graph::addInputAttribute(int id, int nodeId, int variant){
    Attribute tmp;
    tmp.id = id;
    tmp.parent = nodeId;
    tmp.variant = variant;
    tmp.isInput = true;

    this->nodes[nodeId].inputAttributes.push_back(tmp);
    this->attributes[id] = tmp;
}

void Graph::addOutputAttribute(int id, int nodeId, int variant){
    Attribute tmp;
    tmp.id = id;
    tmp.parent = nodeId;
    tmp.variant = variant;
    tmp.isInput = false;

    this->nodes[nodeId].outputAttributes.push_back(tmp);
    this->attributes[id] = tmp;
}

void Graph::connect(int u, int v, int id){
    this->g[u].push_back(v);
    this->connections.push_back(id);
}

Graph::~Graph()
{
}


int main(){
    Graph graph;
    graph.addNode(100, 1);
    graph.addNode(101, 1);
    graph.addNode(102, 1);
    graph.addNode(103, 1);

    graph.addInputAttribute(0, 100, 1);
    graph.addOutputAttribute(1, 100, 1);

    std::cout << "Ok\n";
    graph.addInputAttribute(2, 101, 1);
    graph.addOutputAttribute(3, 101, 1);

    graph.addInputAttribute(4, 102, 1);
    graph.addOutputAttribute(5, 102, 1);

    graph.addInputAttribute(6, 103, 1);
    graph.addOutputAttribute(7, 103, 1);
    return 0;
}