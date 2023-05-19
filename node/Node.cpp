#include "Node.h"

Node::Node(int nodeId, int xPos, int yPos, int zPos, double power) : id(nodeId), x(xPos), y(yPos), z(zPos), signalPower(power) {
    routingTable[id] = {id, id, signalPower, 0};
}

void Node::receiveRoutingTable(const std::map<int, RoutingTableEntry>& receivedTable, int neighborId) {
    updateRoutingTable(receivedTable, neighborId);
}

// This method sends routing table information to other nodes in range to update the other nodes.
void Node::broadcast() {
    this->routingTable[id].sequenceNumber += 2;
    for(auto& node : getNodesInRadius()) {
        if(node.first != this->id) {  // Do not send to self
            sendRoutingTable(*node.second);
        }
    }
}


std::map<int, Node*> Node::getNodesInRadius() {
    std::map<int, Node*> nodesInRadius;
    bool noNodesInRange = true;

    for (Node* otherNode : allNodes) {
        if (this->id != otherNode->id) { // Skip the node itself
            double signalStrength = calculateSignalStrength(otherNode->x, otherNode->y, otherNode->z);
            if (signalStrength >= MIN_SIGNAL_STRENGTH) {
                /*if(this->routingTable[id].sequenceNumber & 1) {
                    this->routingTable[id].sequenceNumber++;
                }*/
                nodesInRadius[otherNode->id] = otherNode;
                noNodesInRange = false;
            }
        }
    }
    if(noNodesInRange) {
        std::cout << "No nodes in range of node " << this->id << std::endl;
        /*if(this->routingTable[id].sequenceNumber & 0)
            this->routingTable[id].sequenceNumber++;*/
    }

    std::cout << "number of nodes in range: " << nodesInRadius.size() << std::endl;
    return nodesInRadius;
}

/*
void Node::updateRoutingTable(const std::map<int, RoutingTableEntry>& receivedTable, int neighborId) {
    std::map<int, RoutingTableEntry> newRoutingTable = routingTable;

    for (const auto& entry : receivedTable) {
        int destination = entry.first;
        const RoutingTableEntry& neighborEntry = entry.second;

        Node* nodeToCheck = allNodes[destination];

        double signalStrengthToNeighbor = calculateSignalStrength(nodeToCheck->x, nodeToCheck->y, nodeToCheck->z);
        if (signalStrengthToNeighbor > MIN_SIGNAL_STRENGTH && (newRoutingTable.count(destination) == 0 ||
                                                              signalStrengthToNeighbor > newRoutingTable[destination].signalStrength ||
                                                              (signalStrengthToNeighbor == newRoutingTable[destination].signalStrength &&
                                    neighborEntry.sequenceNumber > newRoutingTable[destination].sequenceNumber))) {
            newRoutingTable[destination] = neighborEntry;
            newRoutingTable[destination].nextHop = neighborId;
            newRoutingTable[destination].signalStrength = signalStrengthToNeighbor;
            newRoutingTable[this->id].sequenceNumber += 2;
        }
    }

    routingTable = newRoutingTable;
    //broadcast();
    //printRoutingTable();
}*/
void Node::updateRoutingTable(const std::map<int, RoutingTableEntry>& receivedTable, int neighborId) {
    std::map<int, RoutingTableEntry> newRoutingTable = routingTable;

    std::map<int, Node*> nodesInRadius = getNodesInRadius();

    for (const auto& entry : receivedTable) {
        int destination = entry.first;
        const RoutingTableEntry& neighborEntry = entry.second;


        // don't update your own node
        if(destination == this->id) {
            continue;
        }

        // if this node is within range of the node to check from the neighbor table, check if there is a stronger signal from this node to the node to check
        if(nodesInRadius[destination]) { // if the node is in range
            Node* nodeToCheck = allNodes[destination]; // Checking the neighbor node
            double signalStrengthToNeighbor = calculateSignalStrength(nodeToCheck->x, nodeToCheck->y, nodeToCheck->z);
            if(signalStrengthToNeighbor < MIN_SIGNAL_STRENGTH) {
                exit(69);
            }
            if (newRoutingTable.count(destination) == 0) {// Node is in radius but not in routing table
                newRoutingTable[destination] = neighborEntry;
                newRoutingTable[destination].nextHop = neighborId;
                newRoutingTable[destination].signalStrength = signalStrengthToNeighbor;
            } else { // Node is in radius and in routing table
                if (signalStrengthToNeighbor > newRoutingTable[destination].signalStrength) {// if the signal strength is better than the current signal strength in the routing table update the routing table
                    newRoutingTable[destination] = neighborEntry;
                    newRoutingTable[destination].nextHop = destination;
                    newRoutingTable[destination].signalStrength = signalStrengthToNeighbor;
                    newRoutingTable[destination].sequenceNumber = neighborEntry.sequenceNumber;
                }
                if(signalStrengthToNeighbor == newRoutingTable[destination].signalStrength && neighborEntry.sequenceNumber > newRoutingTable[destination].sequenceNumber) {
                    newRoutingTable[destination].sequenceNumber = neighborEntry.sequenceNumber;
                }
            }
        } else { // if the destination node is not in range of this node
            if(neighborEntry.sequenceNumber < newRoutingTable[destination].sequenceNumber)
                continue;
            if (newRoutingTable.count(destination) == 0) { // if the node is not in the routing table
                newRoutingTable[destination] = neighborEntry;
                newRoutingTable[destination].nextHop = neighborId;
                newRoutingTable[destination].signalStrength = neighborEntry.signalStrength;
            } else { // if the node is in the routing table
                if (newRoutingTable[destination].signalStrength > neighborEntry.signalStrength) { // if the signal strength is better than the current signal strength in the routing table update the routing table
                    newRoutingTable[destination] = neighborEntry;
                    newRoutingTable[destination].nextHop = destination;
                    newRoutingTable[destination].signalStrength = neighborEntry.signalStrength;
                    newRoutingTable[destination].sequenceNumber = neighborEntry.sequenceNumber;
                }
                if(neighborEntry.signalStrength == newRoutingTable[destination].signalStrength && neighborEntry.sequenceNumber > newRoutingTable[destination].sequenceNumber) {
                    newRoutingTable[destination].sequenceNumber = neighborEntry.sequenceNumber;
                }
            }
        }
    }

    routingTable = newRoutingTable;
    //broadcast();
    //printRoutingTable();
}




void Node::sendRoutingTable(Node& neighbor) {
    neighbor.receiveRoutingTable(routingTable, id);
}

void Node::AddRoute(RoutingTableEntry& entry, int id) {
    routingTable[id] = entry;
}

double Node::calculateSignalStrength(int destX, int destY, int destZ) {
    double distance = std::sqrt(std::pow(x - destX, 2) + std::pow(y - destY, 2) + std::pow(z - destZ, 2));
    if(distance == 0) return signalPower;
    double signalStrength = signalPower / (4.0 * M_PI * distance * distance);

    std::cout << "Signal strength from Node " << id << " to (" << destX << ", " << destY << ", " << destZ << ") is " << signalStrength << std::endl;

    // Check if the signal strength is too low
    if (signalStrength < MIN_SIGNAL_STRENGTH) {
        return -1.0; // Return -1 indicating a weak signal
    }
    if(signalStrength > signalPower) {
        return signalPower;
    }

    return signalStrength;
}



void Node::printRoutingTable() const {
    std::cout << "Routing Table for Node " << id << ":" << std::endl;
    for (const auto& entry : routingTable) {
        std::cout << "id: " << entry.first;
        std::cout << ", Destination: " << entry.second.destination;
        std::cout << ", Next Hop: " << entry.second.nextHop;
        std::cout << ", Signal Strength: " << entry.second.signalStrength;
        std::cout << ", Sequence Number: " << entry.second.sequenceNumber << std::endl;
    }
    std::cout << std::endl;
}

void Node::updateAllNodes(std::vector<Node*> &allNodes) {
    this->allNodes = allNodes;
}
