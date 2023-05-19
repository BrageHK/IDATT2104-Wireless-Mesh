#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <map>
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
    //TODO: unit_32
    int id;
    int x;
    int y;
    int z;
    double signalPower;
    const double MIN_SIGNAL_STRENGTH = 0.02;
    std::map<int, RoutingTableEntry> routingTable;
    std::vector<Node*> allNodes;

public:
    Node(int nodeId, int xPos, int yPos, int zPos, double power);

    void receiveRoutingTable(const std::map<int, RoutingTableEntry>& receivedTable, int neighborId);

    void sendRoutingTable(Node& neighbor);

    double calculateSignalStrength(int destX, int destY, int destZ);

    void printRoutingTable() const;

    void exchangeRoutingTableWith(Node &other);

    void broadcast();

    std::map<int, Node *> getNodesInRadius();

    void updateAllNodes(std::vector<Node*> &allNodes);

    void AddRoute(RoutingTableEntry &entry, int id);

    void printNeighborTables() const;

    void updateRoutingTable(const std::map<int, RoutingTableEntry> &receivedTable, int neighborId);
};

#endif // NODE_H
