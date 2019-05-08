//
// Created by park on 19. 4. 16.
//

#include <fstream>
#include <iostream>
#include <list>
#include <bitset>
#include <math.h>

#include "GraphHandler.h"

using namespace std;

GraphHandler::GraphHandler(string filename){
    ifstream fs(filename.c_str());
    string line;
    getline(fs, line);


    sscanf(line.c_str(), "%d %d", &_V, &_E);
    _edges = new list<Edge*>();

    for (string line; getline(fs,line); ){
        int from, to, weight;

        if (line.length() <= 0){
            continue;
        }

        sscanf(line.c_str(), "%d %d %d", &from, &to, &weight);
        Edge* new_edge = new Edge(from-1,to-1,weight);
        _edges->push_back(new_edge);
    }
}



GraphHandler::~GraphHandler(){
    if(_edges != NULL){
        // To do
    }
}

int GraphHandler::get_V(){
    return _V;
}



void GraphHandler::print(){
    cout<<"V: "<<_V<<", E: "<<_E<<endl;
    list<Edge*>::iterator iter;
    int _weight;
    int _v_a = -1, _v_b=-1;
    for (iter = _edges->begin(); iter != _edges->end(); iter++){

        _v_a = (*iter)->_from;
        _v_b = (*iter)->_to;
        _weight = (*iter)->_weight;

        cout<<_v_a<<" "<<_v_b<<" "<<_weight<<endl;

    }

}

void GraphHandler::compute_score(Chromosome* chrom){
    list<Edge*>::iterator iter;
    int weight;
    int score=0;
    for(iter = _edges->begin(); iter != _edges->end(); iter++){
        weight = (*iter)->_weight;
        bitset<L> seq = chrom->_sequence;
        if( seq[(*iter)->_from] != seq[(*iter)->_to] ){
            score+=weight;
        }
    }
    chrom->_score = score;
}
