#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <unordered_map>
#include <queue>
#include <cmath>

struct RoutingTableEntry {
    int destination;
    int nextHop;
    double signalStrength;
    int sequenceNumber;
};

class Node {
private:
    int id;
    int x;
    int y;
    int z;
    double signalPower;
    std::unordered_map<int, RoutingTableEntry> routingTable;
    std::unordered_map<int, std::unordered_map<int, RoutingTableEntry>> neighborTables;
    std::vector<Node*> allNodes;

public:
    Node(int nodeId, int xPos, int yPos, int zPos, double power);

    void receiveRoutingTable(const std::unordered_map<int, RoutingTableEntry>& receivedTable, int neighborId);

    void updateRoutingTable();

    void sendRoutingTable(Node& neighbor);

    double calculateSignalStrength(int destX, int destY, int destZ);

    void printRoutingTable() const;

    void exchangeRoutingTableWith(Node &other);

    void broadcast();

    std::vector<Node *> getNodesInRadius();

    void updateAllNodes(std::vector<Node*> &allNodes);

    void AddRoute(RoutingTableEntry &entry, int id);

    void printNeighborTables() const;
};

#endif // NODE_H
