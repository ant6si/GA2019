//
// Created by ant6si on 2019-05-13.
//

#ifndef GA2019_METHODS_H
#define GA2019_METHODS_H
using namespace std;
/// Parameters
int TIME_LIMIT = 178; //50
int POPULATION_SIZE = 300;     //300;170
int XOVER_PER_GENERATION = 240;//240;93
//for selection
float MAX_FITNESS = 1.1;
float MIN_FITNESS = 1;


float MUTATION_RATE = 0.015;//0.015
int MAX_NUM; // valid gene length

bool compare(Chromosome* c1, Chromosome* c2){
    long sc1 = c1->_score;
    long sc2 = c2->_score;
    return sc1 < sc2;
}

void printVec(vector<Chromosome*>* vv){
    vector<Chromosome*>::iterator iter;
    for(iter = vv->begin(); iter!= vv->end() ; iter++ ){
        cout<< (*iter)->_score<<endl;
        //cout<< (*iter)->_score << "\t\t/"   << ((*iter)->_sequence)<<endl;
        //cout<<(*iter)<<endl;
    }
    cout<<endl;
}

bool is_equal(bitset<L> a, bitset<L> b, int max){
    bool flag = true;
    for(int i=0; i<max;i++){
        if(a[i] != b[i]) flag = false;
    }
    return flag;
}

float how_converge(vector<Chromosome*>* vv){
    Chromosome* best = vv->back();
    int c = 0; //count
    int s = vv->size(); // size
    vector<Chromosome*>::reverse_iterator riter(vv->rbegin());
    for(; riter != vv->rend(); ++riter){
        if((*riter)->_score == (best->_score)){
            //cout<<"same"<<endl;
            c++;
        }
        else{
            break;
        }
    }
    float converge = float(c) / float(s);
    return converge;
}

void regularize(Chromosome* chrom, GraphHandler* gh){
    // Regularize chromosome and compute score
    // rule: last bit must be 1
/*
    if (chrom->_sequence[0] == 0){
        //cout<<"before reg: "<<(chrom->_sequence)<<endl;
        for(int i=0;i<MAX_NUM;i++) {
            chrom->_sequence.flip(i);
        }

        //cout<<"after reg: "<< (chrom->_sequence) <<endl;
    }
*/
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

/// Selection
float compute_fitness(int rank){
    float fitness = MAX_FITNESS + (rank-1) * (MIN_FITNESS-MAX_FITNESS)/(float(POPULATION_SIZE)-1.0);
    return fitness;
}
int select(){
    int num;
    float sum_of_fitness = float(POPULATION_SIZE)/2 * (MAX_FITNESS-MIN_FITNESS);
    double point = ((double) rand() / (RAND_MAX)) * sum_of_fitness;
    float sum = 0;
    for (int i=POPULATION_SIZE-1; i>=0; i--){
        sum += compute_fitness(POPULATION_SIZE-i);
        if(point<sum){
            num = i;
            break;
        }
    }
    return num;
}

int select_random(){
    int num = rand()%POPULATION_SIZE;
    return num;
}

/// Crossover
void xover(Chromosome* offspring, Chromosome* p1, Chromosome* p2, GraphHandler* gh){
// p1, p2 : parents
// uniform crossover
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

void one_point_xover(Chromosome* offspring, Chromosome* p1, Chromosome* p2, GraphHandler* gh){
    /// Need to Check
    int point = rand()%MAX_NUM;
    for (int i=0; i<MAX_NUM; i++){
        if(i<point){
            offspring->_sequence.set(i,p1->_sequence[i]);
        }else{
            offspring->_sequence.set(i,p2->_sequence[i]);
        }
    }
}

/// Mutation
void mutation(Chromosome* offspring){
    bitset<L> seq = offspring->_sequence;
    double p = ((double) rand() / (RAND_MAX));
    for(int i=0; i<MAX_NUM; i++){
        if (p < MUTATION_RATE){
            seq.flip(i);
        }
    }
    offspring->_sequence = seq;
}


/// Replacement
void replace_worse(vector<Chromosome*>* offsprings, vector<Chromosome*>* population){
    //offsprings 의 사이즈 만큼 숫자를 골라서 (상위 2는 제외) 하위의 것들을 모두 교체
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

void replace_elitism(vector<Chromosome*>* offsprings, vector<Chromosome*>* population){
    // top 30 elitism
    int size = offsprings->size();
    for (int i=0; i<size; i++){
        int r = rand()%POPULATION_SIZE;
        while(r > POPULATION_SIZE-30){
            r = rand()%POPULATION_SIZE;
        }
        Chromosome* old_c = population->at(r);
        Chromosome* new_c = offsprings->at(i);
        old_c->_sequence = new_c->_sequence;
        old_c->_score = new_c -> _score;
    }

}

/// local optimization
void local_optimize(Chromosome* chrom){
    bitset<L> seq = chrom->_sequence;
    /// To Do
}

int get_best_score(vector<Chromosome*>* population){
    Chromosome* best = population->back();
    return best->_score ;
}



#endif //GA2019_METHODS_H
