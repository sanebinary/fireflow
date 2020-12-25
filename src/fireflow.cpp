#include <iostream>
#include <string>
#include <csignal>
#include "CLI11.hpp"
#include "capture.h"
#include "packet.h"
using namespace std;

void sigIntHandler(int signal);
int packet::totalPackets = 0;

Capture *capture_interface = NULL;

int main(int argc, char* argv[]){
    signal(SIGINT, sigIntHandler);
    CLI::App app{"FIREFLOW: USTH ANTI_DDOS"};

    string interface, debuglog, packetlog;
    int window = 0;
    app.add_option("-i,--interface", interface, "Capture interface")->required();
    app.add_option("-l", debuglog, "Dump to log file");
    app.add_option("-p", packetlog,  "Where to dump packet log");
    app.add_option("-w", window, "The amount of packet to read");
    CLI11_PARSE(app, argc, argv);

    capture_interface = new Capture(interface, debuglog, packetlog, window);
    (*capture_interface).init_logging();
    (*capture_interface).start_pfring_capture();
}


void sigIntHandler(int signal) {
    std::cout << "\nExiting..." << std::endl;
    (*capture_interface).stop_pfring_capture();
    exit(signal);
}