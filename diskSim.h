#ifndef DISK_SIM_H
#define DISK_SIM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class Disk;

int FCFS(const Disk& disk, const vector<int>& tracks);
int SSTF(const Disk& disk, const vector<int>& tracks);
int SCAN(const Disk& disk, const vector<int>& tracks, const char* direction);
int CSCAN(const Disk& disk, const vector<int>& tracks, const char* direction);
int LOOK(const Disk& disk, const vector<int>& tracks, const char* direction);
int CLOOK(const Disk& disk, const vector<int>& tracks, const char* direction);

#endif // DISK_SIM_H