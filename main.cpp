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
#include "methods.h"


using namespace std;



void do_GA(string input_file, ofstream& file_out){
    srand(time(NULL));
    time_t st = time(NULL);
    GraphHandler gh = GraphHandler(input_file);

//    gh.print();
    vector<Chromosome*>* population = new vector<Chromosome*>();
    MAX_NUM = gh.get_V();
    gen_population(population, &gh);
    sort(population->begin(), population->end(), compare);
    time_t remain = TIME_LIMIT-(time(NULL)-st);
    int total_best = -999999999;
    int best_score = -999999999;
    int epoch=0;
    float last_converge;
    int xover_per_generation = int(POPULATION_SIZE * XOVER_RATIO);
    ///GA Start
    while(remain >0 ){
        vector<Chromosome*>* offsprings = new vector<Chromosome*>();
        for (int count=0; count < xover_per_generation; ++count){
            Chromosome* offspring = new Chromosome();
            // Selection
            int p1 = select_random();
            int p2 = p1;
            while (p1 == p2){
                p2 = select_random();
            }
//            cout<<p1<<", "<<p2<<endl;

            // Xover
            one_point_xover( offspring, population->at(p1), population->at(p2), &gh);
            // Mutation
            mutation(offspring);
            //get score and regularize the new offspring
            regularize(offspring, &gh);
            offsprings->push_back(offspring);
        }

        // Sort
        sort( population->begin(), population->end(), compare);
        // Replacement
        //replace_elitism(offsprings, population);
        replace_worse(offsprings, population);
        sort( population->begin(), population->end(), compare);
        //local optimization
        do_local_optimize(population, &gh);
        sort(population->begin(), population->end(), compare);

        // print best
        best_score = get_best_score(population);

        if(total_best<best_score){
            total_best = best_score;
//            cout<<total_best<<endl;
        }

        epoch++;
        float converge = how_converge(population);
        //if (true){
       if(epoch%10==0){
            //cout<< "time:"<<(time(NULL)-st)<<"/ epoch: " << epoch << "/ best_score: "<< best_score<<"/ converge: "<< converge <<endl;
            //cout<<best_score<<endl;
        }
        remain = TIME_LIMIT-(time(NULL)-st);
        last_converge = converge;

        // Added to free memory
        while(!offsprings->empty()) {
            delete offsprings->back();
            offsprings->pop_back();
        }

        ///CAUTION
        offsprings->clear();
        delete(offsprings);
    }

    file_out<< "time:"<<(time(NULL)-st)<<"/ epoch: " << epoch << "/ best_score: "<< best_score<<"/ converge: "<< last_converge <<endl;
//printVec(population);
}

int main(int argc, char** argv) {
    string input_file;
    string output_file;

    if(argc == 3){
        input_file = string(argv[1]);
        output_file = string(argv[2]);
    }
    else{
        input_file = "input/instance2.txt";
        output_file = "hello.txt";
    }
    
    ofstream file_out;
    file_out.open(output_file.c_str());
    
    for (int ps = 50; ps<350; ps+=50){
        for(int xr = 1; xr<10; xr++){
            for(int mr = 3; mr<34; mr+=10){
                for(int optr=1; optr<10; optr++){
                        POPULATION_SIZE = ps;
                        XOVER_RATIO = xr/10.0;
                        MUTATION_RATE = mr*0.05;
                        OPTIMIZE_RATIO = optr/10.0;
                        file_out<<"POPULATION: "<<POPULATION_SIZE<<"/ XOVER: "<<XOVER_RATIO<<"/ MUATION: "<<MUTATION_RATE<<"/ OPTIMIZE: "<<OPTIMIZE_RATIO<<endl;
                        for(int count=0; count<50; count++){
                            do_GA(input_file, file_out);
                        }
                }
            }
        }
    }
    file_out.close();

    ///FOR SUBMIT
    /*
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
     */
    return 0;
}
