#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <limits.h>

#include <iostream>
#include <fstream>

#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <vector>

#include <time.h>
#include <assert.h>

#include <bitset>
#include "GraphHandler.h"


using namespace std;

/// Parameters
int TIME_LIMIT = 3; //50
int POPULATION_SIZE = 350;     //300;170
int XOVER_PER_GENERATION = 180;//240;93
float MUTATION_RATE = 0.015;//0.015

int MAX_NUM;

bool compare(Chromosome* c1, Chromosome* c2){
    long sc1 = c1->_score;
    long sc2 = c2->_score;
    return sc1 < sc2;
}

void printVec(vector<Chromosome*>* vv){
    vector<Chromosome*>::iterator iter;
    for(iter = vv->begin(); iter!= vv->end() ; iter++ ){
        cout<< (*iter)->_score << "\t\t/"   << ((*iter)->_sequence)<<endl;
    }
    cout<<endl;
}

void regularize(Chromosome* chrom, GraphHandler* gh){
    // Regualrize chromosome and coumpute score
    // rule: last bit must be 1

    bitset<L> seq = chrom->_sequence;
    if (! seq[0]){
        for(int i=0;i<MAX_NUM;i++){
            chrom->_sequence = seq.flip(i);
        }
    }
    gh->compute_score(chrom);
}


void gen_population (vector<Chromosome*>* pop, GraphHandler* gh){
    // Generate new random population

    for(int chrom_num=0; chrom_num<POPULATION_SIZE; chrom_num++){
        bitset<L> new_seq(0);
        for(int gene_num=0; gene_num < MAX_NUM; gene_num++){
            if(rand()%2 == 1){
                new_seq[gene_num]=1;
            }
        }
        Chromosome* new_chrom = new Chromosome(new_seq,0);
        regularize(new_chrom, gh);
        pop->push_back(new_chrom);
    }
}

int select(){
    int num = rand()%POPULATION_SIZE;
//    int num = POPULATION_SIZE/2 + rand()%(POPULATION_SIZE/2);
    return num;

}

void xover(Chromosome* offspring, Chromosome* p1, Chromosome* p2, GraphHandler* gh){
//p1, p2 : parents
    bitset<L> seq1 = p1->_sequence;
    bitset<L> seq2 = p2->_sequence;

    bitset<L> new_seq(0);

    for(int i=0; i<MAX_NUM; i++){
        int r = rand()%2;
        if(r == 0 ){
            new_seq.set( i, seq1[i] );
        }else{
            new_seq.set( i, seq2[i] );
        }
    }
    offspring->_sequence = new_seq;

}

void mutation(Chromosome* offspring){

    bitset<L> seq = offspring->_sequence;
    double prob = ((double) rand() / (RAND_MAX));
    for(int i=0; i<MAX_NUM; i++){
        if (prob < MUTATION_RATE){
            seq.flip(i);
        }
    }

    offspring->_sequence = seq;

}

void replace_(vector<Chromosome*>* offsprings, vector<Chromosome*>* population){
    //offsprings의 사이즈 만큼 숫자를 골라서 (상위 2는 제외) 하위의 것들을 모두 교체
    int size = offsprings->size();
    for(int i=0; i<size; i++){
        Chromosome* old_c = population->at(i);
        Chromosome* new_c = offsprings->at(i);
//        cout<<"NEW: "<<new_c->_sequence<<endl;
        old_c->_sequence = new_c->_sequence;
        old_c->_score = new_c ->_score;
        //population->at(i) = offsprings->at(i);
    }

}

int get_best_score(vector<Chromosome*>* population){
    Chromosome* best = population->back();
    return best->_score ;
}


int main() {
    srand(time(0));
    time_t st = time(NULL);
    GraphHandler gh = GraphHandler(string("instance1.txt"));

//    gh.print();

    vector<Chromosome*>* population = new vector<Chromosome*>();
    MAX_NUM = gh.get_V();
    gen_population(population, &gh);
    sort(population->begin(), population->end(), compare);
    time_t remain = TIME_LIMIT-(time(NULL)-st);

    int total_best = -999999999;
    int best_score = -999999999;
    int epoch=0;

    while(remain >0){
        //       cout<<remain<<endl;
        vector<Chromosome*>* offsprings = new vector<Chromosome*>();
        for (int count=0; count < XOVER_PER_GENERATION; ++count){
            Chromosome* offspring = new Chromosome();

            // Selection
            int p1 = select();
            int p2 = p1;

            while (p1 == p2){
                p2 = select();
            }
//            cout<<p1<<", "<<p2<<endl;

            // Xover
            xover( offspring, population->at(p1), population->at(p2), &gh);
            // Mutation
            mutation(offspring);
            //get score of new offspring
            regularize(offspring, &gh);
            offsprings->push_back(offspring);
        }

        // Sort
        sort( population->begin(), population->end(), compare);
        // Replacement
        replace_(offsprings, population);
        // print best
        best_score = get_best_score(population);

        if(total_best<best_score){
            total_best = best_score;
//            cout<<total_best<<endl;
        }

        epoch++;
        if (epoch%100==0){
//            cout<<best_score<<endl;
        }
        remain = TIME_LIMIT-(time(NULL)-st);

        // Added to free memory
        while(!offsprings->empty()) {
            delete offsprings->back();
            offsprings->pop_back();
        }

        ///CAUTION
        offsprings->clear();
        delete(offsprings);

    }
    sort( population->begin(), population->end(), compare);
    Chromosome* champ = population->back();
    bitset<L> champ_seq = champ->_sequence;
    vector<int>* champ_vec = new vector<int>();

    for(int x=0; x<MAX_NUM; x++){
        if(champ_seq[x]==0){
            champ_vec->push_back((x+1));
        }
    }

    ofstream of("maxcut.out");
    if(of.is_open()){
        vector<int>::iterator iter;
        iter = champ_vec->begin();
        of<<(*iter);
        iter++;
        for(; iter != champ_vec->end(); iter++){
            of<<" "<< (*iter);
        }
    }

    // memory cleanup
    delete champ_vec;

    //printVec(population);
    //maxout 에 인덱스 1더하기!!!
    return 0;
}
