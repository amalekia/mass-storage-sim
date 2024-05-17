#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "diskSim.h"

#define MAX_TRACKS 4000
using namespace std;

// Define the Disk class
class Disk {
public:
    int initialPosition;
    vector<int> track_requests;

    Disk(int track) : initialPosition(track) {}

    void addRequest(int request) {
        track_requests.push_back(request);
    }
};

// FCFS
int FCFS(const Disk& disk, const vector<int>& tracks) {
    int totalDistance = 0;
    int currentPosition = disk.initialPosition;
    for (const auto& track : tracks) {
        totalDistance += abs(currentPosition - track);
        currentPosition = track;
    }
    return totalDistance;
}

// SSTF
int SSTF(const Disk& disk, const vector<int>& tracks) {
    int totalDistance = 0;
    int currentPosition = disk.initialPosition;
    vector<int> remainingTracks = tracks;
    vector<int> sortedTracks;
    while (!remainingTracks.empty()) {
        int minDistance = MAX_TRACKS;
        int minIndex = -1;
        for (size_t i = 0; i < remainingTracks.size(); i++) {
            int distance = abs(currentPosition - remainingTracks[i]);
            if (distance < minDistance) {
                minDistance = distance;
                minIndex = i;
            }
        }
        totalDistance += minDistance;
        currentPosition = remainingTracks[minIndex];
        sortedTracks.push_back(remainingTracks[minIndex]);
        remainingTracks.erase(remainingTracks.begin() + minIndex);
    }
    return totalDistance;
}

// SCAN
int SCAN(const Disk& disk, const vector<int>& tracks, const char* direction) {
    int totalDistance = 0;

    vector<int> remainingTracks = tracks;
    sort(remainingTracks.begin(), remainingTracks.end());

    if (strcmp(direction, "positive") == 0) {
        if (remainingTracks[0] > disk.initialPosition) {
            totalDistance = (remainingTracks[remainingTracks.size()-1] - disk.initialPosition);
        }
        else {
            totalDistance = (MAX_TRACKS - disk.initialPosition) + (MAX_TRACKS - remainingTracks[0]);
        }
    }
    else {
        if (remainingTracks[remainingTracks.size()-1] < disk.initialPosition) {
            totalDistance = (disk.initialPosition - remainingTracks[0]);
        }
        else {
            totalDistance = (disk.initialPosition) + (remainingTracks[remainingTracks.size() - 1]);
        }
    }
    return totalDistance;
}

// CSCAN
int CSCAN(const Disk& disk, const vector<int>& tracks, const char* direction) {
    int totalDistance = 0;

    vector<int> remainingTracks = tracks;
    sort(remainingTracks.begin(), remainingTracks.end());

    int initialIdx = find(remainingTracks.begin(), remainingTracks.end(), disk.initialPosition) - remainingTracks.begin();
    for (const auto& track :remainingTracks) {
        if (track >= disk.initialPosition) {
            initialIdx = find(remainingTracks.begin(), remainingTracks.end(), track) - remainingTracks.begin();
            break;
        }
    }

    if (strcmp(direction, "positive") == 0) {
        if (remainingTracks[0] > disk.initialPosition) {
            totalDistance = (remainingTracks[remainingTracks.size()-1] - disk.initialPosition);
        }
        else {
            totalDistance = (MAX_TRACKS - disk.initialPosition) + MAX_TRACKS + remainingTracks[initialIdx-1];
        }
    }
    else {
        if (remainingTracks[remainingTracks.size()-1] < disk.initialPosition) {
            totalDistance = (disk.initialPosition - remainingTracks[0]);
        }
        else {
            totalDistance = (disk.initialPosition) + MAX_TRACKS + remainingTracks[initialIdx];
        }
    }
    return totalDistance;
}

// LOOK
int LOOK(const Disk& disk, const vector<int>& tracks, const char* direction) {
    int totalDistance = 0;

    vector<int> remainingTracks = tracks;
    sort(remainingTracks.begin(), remainingTracks.end());

    if (strcmp(direction, "positive") == 0) {
        if (remainingTracks[0] > disk.initialPosition) {
            totalDistance += (remainingTracks[remainingTracks.size()-1] - disk.initialPosition);
        }
        else {
            totalDistance += (remainingTracks[remainingTracks.size()-1] - disk.initialPosition) 
                                + (remainingTracks[remainingTracks.size()-1] - remainingTracks[0]);
        }
    }
    else {
        if (remainingTracks[remainingTracks.size()-1] < disk.initialPosition) {
            totalDistance += (disk.initialPosition - remainingTracks[0]);
        }
        else {
            totalDistance += (disk.initialPosition - remainingTracks[0]) 
                                + (remainingTracks[remainingTracks.size() - 1] - remainingTracks[0]);
        }
    }
    return totalDistance;
}

// CLOOK
int CLOOK(const Disk& disk, const vector<int>& tracks, const char* direction) {
    int totalDistance = 0;

    vector<int> remainingTracks = tracks;
    sort(remainingTracks.begin(), remainingTracks.end());

    int initialIdx = find(remainingTracks.begin(), remainingTracks.end(), disk.initialPosition) - remainingTracks.begin();
    for (const auto& track :remainingTracks) {
        if (track >= disk.initialPosition) {
            initialIdx = find(remainingTracks.begin(), remainingTracks.end(), track) - remainingTracks.begin();
            break;
        }
    }

    if (strcmp(direction, "positive") == 0) {
        if (remainingTracks[0] > disk.initialPosition) {
            totalDistance += (remainingTracks[remainingTracks.size()-1] - disk.initialPosition);
        }
        else {
            totalDistance = (remainingTracks[remainingTracks.size()-1] - disk.initialPosition) 
                                + (remainingTracks[remainingTracks.size()-1] - remainingTracks[0])
                                + (remainingTracks[initialIdx-1] - remainingTracks[0]);
        }
    }
    else {
        if (remainingTracks[remainingTracks.size()-1] < disk.initialPosition) {
            totalDistance = (disk.initialPosition - remainingTracks[0]);
        }
        else {
            totalDistance = (disk.initialPosition - remainingTracks[0]) 
                                + (remainingTracks[remainingTracks.size() - 1] - remainingTracks[0]);
        }
    }
    return totalDistance;
}

int main(int argc, char* argv[]) {

    const char* direction = "positive";

    int initialPosition = atoi(argv[1]);

    if (initialPosition < 0) {
        direction = "negative";
    }

    initialPosition = abs(initialPosition);

    if (initialPosition < -4999 || initialPosition > 4999) {
        cout << "Error: Invalid initial position" << endl;
        return 1;
    }

    // Create the disk
    Disk disk(initialPosition);

    // Open the input file
    ifstream inputFile(argv[2]);
    if (!inputFile.is_open()) {

        // Generate 100 random tracks
        vector<int> tracks;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 4999);
        for (int i = 0; i < 100; i++) {
            disk.addRequest(dis(gen));
        }
    }
    else {
        // Read the requests
        while (!inputFile.eof()) {
            int track;
            inputFile >> track;
            disk.addRequest(track);
        }
        // Close the input file
        inputFile.close();
    }

    // FCFS
    int totalDistance = FCFS(disk, disk.track_requests);
    cout << "FCFS " << totalDistance << endl;

    // SSTF
    totalDistance = SSTF(disk, disk.track_requests);
    cout << "SSTF " << totalDistance << endl;

    // SCAN
    totalDistance = SCAN(disk, disk.track_requests, direction);
    cout << "SCAN " << totalDistance << endl;

    // C-SCAN
    totalDistance = CSCAN(disk, disk.track_requests, direction);
    cout << "C-SCAN " << totalDistance << endl;

    // LOOK
    totalDistance = LOOK(disk, disk.track_requests, direction);
    cout << "LOOK " << totalDistance << endl;

    // C-LOOK
    totalDistance = CLOOK(disk, disk.track_requests, direction);
    cout << "C-LOOK " << totalDistance << endl;

    return 0;
}