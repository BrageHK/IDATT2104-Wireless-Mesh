#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "node/Node.h"
#include "worker/Workers.h"

using namespace std;

int main() {
    Node node1(1, 0, 0, 0, 20);
    Node node2(2, 2, 2, 2, 20);

    //mutex mutex_wait;

    node2.exchangeRoutingTableWith(node1);
    node1.exchangeRoutingTableWith(node2);

    /*
    Workers worker_threads(4);
    worker_threads.start(); // Create 4 internal threads

    worker_threads.start();
    worker_threads.post([&node1] {

        cout << "Hello from thread " << this_thread::get_id() << endl;
    });

    worker_threads.stop();*/

    return 0;
}
