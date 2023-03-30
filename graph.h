#include "imnodes.h"
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
    void draw();
    ~Graph();
};

Graph::Graph()
{
    this->g.resize(MAX_SIZE);
    this->attributes.resize(MAX_SIZE);
    this->nodes.resize(MAX_SIZE);
}

void Graph::addNode(int id, int type){
    this->nodes[id]={id, type};
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
}

void Graph::draw(){
    for(Node node : this->nodes){
        if(node.id == 0) continue;
        ImNodes::BeginNode(node.id);
            ImNodes::BeginNodeTitleBar();
                ImGui::Text("Hello");
            ImNodes::EndNodeTitleBar();

            for(Attribute input : node.inputAttributes){
                ImNodes::BeginInputAttribute(input.id);
                ImNodes::EndInputAttribute();
            }
            for(Attribute output : node.outputAttributes){
                ImNodes::BeginOutputAttribute(output.id);
                ImNodes::EndOutputAttribute();
            }
        ImNodes::EndNode();
    }
    auto it = this->connections.begin();
    for(int i = 0; i < this->g.size(); i++){
        for(int j = 0; j < this->g[i].size(); j++){
            ImNodes::Link(i+j, i, g[i][j]);
            it++;
        }
    }
}

Graph::~Graph()
{
}
