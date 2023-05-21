#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <map>
#include <queue>
#include <cmath>
#include <map>
#include <tuple>

class Topography;

using RoutingTable = std::map<int, std::tuple<int, double, int>>;

class Node {
private:
    //TODO: update  som of the values to unit_32
    int id;
    int x;
    int y;
    int z;
    double signalPower;
    const double MIN_SIGNAL_STRENGTH = 0.02;
    RoutingTable routingTable;
    std::vector<Node*> allNodes;
    Topography* topography;

public:
    Node(int nodeId, int xPos, int yPos, int zPos, double power, Topography* topography);

    int getX() const;

    int getY() const;

    int getZ() const;

    double getSignalPower() const;

    void sendRoutingTable(Node& neighbor);

    double calculateSignalStrength(int destX, int destY, int destZ);

    void printRoutingTable() const;

    void broadcast();

    std::vector<Node *> getNodesInRadius();

    void updateAllNodes(std::vector<Node*> &allNodes);

    void receiveRoutingTable(RoutingTable& receivedTable, int neighborId);

    void updateOwnTableFromAllInRange();

    void updateRoutingTable(const RoutingTable &tableB, int neighborId);

    double calculateSignalStrength(Node *node);

    int getId() const;

    void setPosition(int xPos, int yPos, int zPos);

    void sendMessage(int receiverId, std::string basicString, std::vector<std::pair<Node*, Node*>>& connectedDrones);
};

#endif // NODE_H
