#include "DataStruct.h"

using namespace std;



class MyDFS{
public:
    MyDFS(string dataFile, string resultFile);
    //~MyDFS();
    bool loadData();
    int computeCircle();
    void printAllCircle();
    void printAllNodeAndArc();
    //void storeResult(string resultFile);
private:
    bool initGraph();
    void findAllCycle(Graph* p, int nodeNum, int pathNum);
    bool existCycle(int* temp);

private:
    Graph* this_graph;
    int visited[MAX_NODE]={0};//表示节点是否被访问，初始化都未被访问
    int path[10000];
    int cycle_path[100000][10];
    int path_count = 0;
private:
    string data_file;//用于存储数据的位置
    string result_file;//用于存储
    //参数
};

Arc* mallocArc(int nearNode){
    Arc* p = (Arc *)malloc(sizeof(Arc));
    p->near_node = nearNode;
    p->next_arc = NULL;
    return p;
}

Node* mallocNode(int name){
    Node* p = (Node *)malloc(sizeof(Node));
    p->first_arc = NULL;
    p->node_name = name;
    return p;
}

bool MyDFS::initGraph() {
    //初始化图
    Graph* p = (Graph *)malloc(sizeof(Graph));
    if(!p)return false;
    p->node_num = 0;
    p->arc_num = 0;
    p->node_list = (Node *)malloc(sizeof(Node)*MAX_NODE);
    for (int i = 0; i < MAX_NODE; ++i) {
        p->node_list[i].node_name = -1;
        p->node_list[i].first_arc = NULL;
    }
    this_graph = p;
    //初始化全局变量
    for (int i = 0; i < MAX_PATH; ++i) {
        path[i] = -1;
    }
    for (int i = 0; i < 60; ++i) {
        for (int j = 0; j < 10; ++j) {
            cycle_path[i][j] = -1;
        }
    }
    return true;
}

MyDFS::MyDFS(string dataFile, string resultFile) {
    data_file = dataFile;
    result_file = resultFile;
    initGraph();
}

bool MyDFS::loadData() {
    ifstream infile(data_file, ios::binary);
    if(!infile){
        exit(0);
    }
    unsigned int size = static_cast<unsigned int>(infile.seekg(0, std::ios::end).tellg());
    char *buf = new char[size];
    infile.seekg(0, ios::beg).read(&buf[0],size);
    infile.close();
    char *p = buf;
    while(*p !='\0'){
        int tmp;
        int start=0;
        int end=0;
        int num_count = 1;
        while((tmp = *(p++)) !='\n'){
            if(tmp == ','){
                num_count++;
                continue;
            }
            else if(num_count == 1){
                start = start*10+(tmp-48);
                continue;
            }
            else if(num_count == 2){
                end = end*10+(tmp-48);
                continue;
            }
            else continue;
        }
        if(this_graph->node_list[end].node_name == -1) {
            this_graph->node_list[end].node_name = end;
            this_graph->node_num++;
        }
        if(this_graph->node_list[start].node_name == -1){
            this_graph->node_list[start].node_name = start;
            this_graph->node_list[start].first_arc = mallocArc(end);
            this_graph->node_num++;
            this_graph->arc_num++;
        }else{
            if(Arc* q = this_graph->node_list[start].first_arc){
                while(q->next_arc!=NULL){
                    q = q->next_arc;
                }
                q->next_arc = mallocArc(end);
                this_graph->arc_num++;
            }else{
                this_graph->node_list[start].first_arc = mallocArc(end);
                this_graph->arc_num++;
            }
        }
        //p++;
    }
//    for(Arc* t = this_graph->node_list[1].first_arc;t!=NULL;t=t->next_arc){
//        printf("%d\n",t->near_node);
//    }
    return true;
}

void MyDFS::printAllNodeAndArc(){
    Graph* p = this_graph;
    for(int i =0;i<MAX_NODE;i++){
        if(p->node_list[i].node_name != -1){
            printf("这是节点%d\n",p->node_list[i].node_name);
            for(Arc* q = p->node_list[i].first_arc;q!=NULL;q=q->next_arc){
                printf("包含边：%d\n",q->near_node);
            }
            printf("--------------------------\n");
        }
    }
    return;
}

int MyDFS::computeCircle(){
    Graph *p = this_graph;
    path_count = 0;
    for(int i=0;i<MAX_NODE;i++){
        if(p->node_list[i].node_name != -1)findAllCycle(p,i,0);
    }
    return path_count;
}

bool MyDFS::existCycle(int* temp){
    for (int i = 0; i < path_count; ++i) {
        int *p,*q;
        p=cycle_path[i];
        q=temp;
        while(*(p++) == *(q++)){
            if(*p == -1 && *q == -1){
                return true;
            }
        }
    }
    return false;
}

void MyDFS::findAllCycle(Graph* p, int nodeNum, int pathNum){
    Arc *q;
    visited[nodeNum]=1;
    path[pathNum] = nodeNum;
    for (q = p->node_list[nodeNum].first_arc; q!= NULL ; q=q->next_arc) {
        if(visited[q->near_node]){//找到了回路
            for (int i = 0; i < pathNum; ++i) {
                if(path[i] == q->near_node){//从path[i]找到的回路
                    //char* write_buf = (char *)malloc(sizeof(char)*(pathNum+1-i));
                    int temp[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
                    int min = 100000;
                    int idx = -1;
                    for (int j = i; j <= pathNum; ++j) {
                        if(path[j] < min){
                            min = path[j];
                            idx = j;
                        }
                    }
                    if(pathNum - i > 7 || pathNum - i < 3){//找到的节点长度大于7或者小于3
                        break;
                    }
                    for (int k = 0; k < pathNum - i +1; ++k) {
                        temp[k] = path[i+(idx-i+k)%(pathNum - i +1)];   //用心领悟！
                    }
//                    if(temp[0]==2){
//                        printf("日怪！");
//                    }
                    if(existCycle(temp))break;
                    else {
                        for (int o = 0; o < 10; ++o) {
                            cycle_path[path_count][o] = temp[o];
                        }
                        path_count++;
                        break;
                    }
                }
            }
            continue;
        }else{//没有找到回路，继续深度遍历
            findAllCycle(p,q->near_node,pathNum+1);
        }
    }
    visited[nodeNum] = 0;
    path[pathNum] = -1;
}

void MyDFS::printAllCircle() {
    for (int i = 0; i < path_count; ++i) {
        for (int j = 0; j < 10; ++j) {
            if(cycle_path[i][j] != -1)printf("%d ",cycle_path[i][j]);
            else break;
        }
        printf("\n");
    }
    return;
}

int main() {
    clock_t start,end;
    start = clock();
    string dataFile = "/home/lyc/simulate/test_data_test.txt";
    string resultFile = "/home/lyc/data/prediction.txt";
    MyDFS my_dfs(dataFile,resultFile);
    my_dfs.loadData();
    printf("载入数据用时%f\n",(double)(clock()-start)/CLOCKS_PER_SEC);
    start = clock();
    //my_dfs.printAllNodeAndArc();
    int i = my_dfs.computeCircle();
    printf("计算回路用时%f\n",(double)(clock()-start)/CLOCKS_PER_SEC);
    printf("共有%d条回路\n",i);
    my_dfs.printAllCircle();
    return 0;
}













