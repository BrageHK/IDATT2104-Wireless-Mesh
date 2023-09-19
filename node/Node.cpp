#include "Node.h"
#include "../topography/Topography.h"

Node::Node(int nodeId, int xPos, int yPos, int zPos, double power, Topography* topography)
        : id(nodeId), x(xPos), y(yPos), z(zPos), signalPower(power), topography(topography) {
    routingTable[id] = std::make_tuple(id, 0, 0);
}

int Node::getId() const {
    return id;
}

void Node::setPosition(int xPos, int yPos, int zPos) {
    x = xPos;
    y = yPos;
    z = zPos;
}


// This method sends routing table information to other nodes in range to updateNodePointers the other nodes.
void Node::broadcast() {
    this->routingTable[id] = std::make_tuple(id, std::get<1>(routingTable[id]), std::get<2>(routingTable[id]) + 2);
    for(Node* node : getNodesInRadius()) {
        if(node->id != this->id) { // Do not send to self
            sendRoutingTable(*node);
        }
    }
}

std::vector<Node*> Node::getNodesInRadius() {
    std::vector<Node*> nodesInRadius;
    bool noNodesInRange = true;
    for (Node* otherNode : allNodes) {
        if (this->id != otherNode->id) { // Skip the node itself
            double signalStrength = calculateSignalStrength(otherNode);
            if (signalStrength >= MIN_SIGNAL_STRENGTH) {
                nodesInRadius.push_back(otherNode);
                noNodesInRange = false;
            }
        }
    }
    if(noNodesInRange) {
        // something
    }

    return nodesInRadius;
}

// not needed
void Node::updateOwnTableFromAllInRange() {
    for (Node* otherNode : getNodesInRadius()) {
        updateRoutingTable(otherNode->routingTable, id);
    }
}

void Node::updateRoutingTable(const RoutingTable& tableB, int neighborId) {
    RoutingTable tableA = this->routingTable;
    for (const auto& [destination, entryB] : tableB) {
        int nextHopB, numHopsB, seqNumberB;
        std::tie(nextHopB, numHopsB, seqNumberB) = entryB;

        if (tableA.find(destination) == tableA.end()) {  // If the destination does not exist in A's table, add it
            tableA[destination] = std::make_tuple(neighborId, numHopsB + 1, seqNumberB);
        }
        else {  // If the destination exists, updateNodePointers it based on DSDV conditions
            auto& [_, numHopsA, seqNumberA] = tableA[destination];

            if (seqNumberB > seqNumberA) {
                tableA[destination] = std::make_tuple(neighborId, numHopsB + 1, seqNumberB);
            }
            else if (seqNumberB == seqNumberA && numHopsB + 1 < numHopsA) {
                tableA[destination] = std::make_tuple(neighborId, numHopsB + 1, seqNumberB);
            }
        }
    }
    this->routingTable = tableA;
}

void Node::receiveRoutingTable(RoutingTable& receivedTable, int neighborId) {
    updateRoutingTable(receivedTable, neighborId);
}

void Node::sendRoutingTable(Node& neighbor) {
    neighbor.receiveRoutingTable(routingTable, id);
}

double Node::calculateSignalStrength(Node* node) {
    return calculateSignalStrength(node->x, node->y, node->z);
}

double Node::calculateSignalStrength(int destX, int destY, int destZ) {
    if(topography->isObstructionBetween(x, y, z, destX, destY, destZ))
        return -1.0;
    double distance = std::sqrt(std::pow(x - destX, 2) + std::pow(y - destY, 2) + std::pow(z - destZ, 2));
    if(distance == 0) return signalPower;
    double signalStrength = signalPower / (2.0 * M_PI * distance * distance);

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
        std::cout << "Destination: " << entry.first;
        std::cout << ", Next Hop: " << std::get<0>(entry.second);
        std::cout << ", Number of hops: " << std::get<1>(entry.second);
        std::cout << ", Sequence Number: " << std::get<2>(entry.second) << std::endl;
    }
    std::cout << std::endl;
}

void Node::updateAllNodes(std::vector<Node*> &allNodes) {
    this->allNodes = allNodes;
}

double Node::getSignalPower() const {
    return signalPower;
}

int Node::getZ() const {
    return z;
}

int Node::getY() const {
    return y;
}

int Node::getX() const {
    return x;
}

void Node::sendMessage(int receiverId, std::string basicString, std::vector<std::pair<Node*, Node*>>& connectedDrones) {

    if(routingTable.find(receiverId) == routingTable.end()) {
        std::cout << "Node " << id << " does not have a route to node " << receiverId << std::endl;
        return;
    }
    int nextNode = std::get<0>(routingTable[receiverId]);
    connectedDrones.emplace_back(this, allNodes[nextNode]);
    if(nextNode == id) {
        std::cout << "Message received by node " << id << ": " << basicString << std::endl;
        return;
    }
    std::cout << "Message sent from node " << id << " to node " << nextNode << std::endl;
    allNodes[nextNode]->sendMessage(receiverId, basicString, connectedDrones);
}
