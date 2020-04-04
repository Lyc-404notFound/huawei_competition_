//
// Created by lyc on 2020/4/1.
//

#ifndef HUAWEI_COMPETITION_DATASTRUCT_H
#define HUAWEI_COMPETITION_DATASTRUCT_H

#endif //HUAWEI_COMPETITION_DATASTRUCT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#define MAX_NODE 27000
#define MAX_PATH 10

typedef struct Arc{
    int near_node;
    Arc* next_arc;
}Arc;
typedef struct Node{
    int node_name;
    Arc* first_arc;
}Node;
typedef struct Graph{
    int node_num,arc_num;
    Node* node_list;
}Graph;


