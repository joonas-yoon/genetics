#include <bits/stdc++.h>
using namespace std;

#define GENE_SIZE   10
#define MAX_NUMBER  100
#define MUTATE_RATE 0.01

void randomSeed(int seed = 0){
	srand(time(NULL) + seed);
}

int generateNumber(int max){
	return rand()%max + 1;
}

class DNA {
private:
	string gene;
	int tryCount;
public:
	DNA(){
		tryCount = 0;
		gene.assign(GENE_SIZE, '0');
	}
	DNA(const DNA& d){
		tryCount = d.tryCount;
		gene = d.gene;
	}
	~DNA(){}
public:
	bool operator < (const DNA& d) const {
		return getTryCount() < d.getTryCount();
	}
	string getGene() const {
		return gene;
	}
	const int getTryCount() const {
		return tryCount;
	}
public:
	void makeRandomDNA(){
		for(int i=0; i<GENE_SIZE; ++i){
			int percent = generateNumber(11); // [0, 10]
			gene[i] = percent > 9 ? (percent-10+'a') : (percent+'0');
		}
	}

	// 문제를 풀어본다. 몇 번만에 해결할까
	int solve(const int& answer){
		// 이전에 풀어봤었다면 그 정보를 반환
		if(getTryCount() != 0) return getTryCount();
		
		int lower=1, upper=MAX_NUMBER, tries=0;
		while(lower <= upper){
			int guess = guessNumber(lower, upper, tries++);
			if(guess == answer) break;
			else if(guess < answer){
				lower = guess + 1;
			} else {
				upper = guess - 1;
			}
		}
		return (tryCount = tries);
	}
	
	// 교배
	DNA mating(const DNA& opponent){
		return this->mutation();
			
		// 교배 중에 일정 확률로 돌연변이가 나타난다.
		int mutateRate = (int)(MUTATE_RATE*100);
		if( mutateRate > 0 && rand()%mutateRate == 0 ){
			return this->mutation();
		}
		
		// 교차율은 70% 정도가 적당하다고 한다.
		
	}
	
	// 돌연변이
	DNA mutation(){
		DNA mutant(*this);
		return mutant.mutate();
	}
	
	DNA mutate(){
		std::reverse(this->gene.begin(), this->gene.end());
		return *this;
	}
	
private:
	int guessNumber(int lowest, int highest, int tries){
		int width = highest - lowest;
		double percent = getGeneOnPos(tries) * 0.1; // 1 to 0.1 (10%)
		// printf("%d + (%d - %d) * %.1lf\n", lowest, highest, lowest, percent);
		return lowest + (int)(width * percent);
	}
	int getGeneOnPos(int pos){
		char c =  gene[pos % GENE_SIZE];
		if(c <= '9')
			return c - '0';
		return c-'a'+10;
	}
};

vector<DNA> getSuperiors(vector<DNA> gen){
	// 상위 5%를 우성 유전자로 설정한다.
	int superiorsCount = gen.size() * 0.05;
	std::sort(gen.begin(), gen.end());
	
	vector<DNA> superiors;
	for(auto& dna : gen){
		printf("%s[%d] ", dna.getGene().c_str(), dna.getTryCount());
		superiors.push_back(dna);
		if(superiors.size() >= superiorsCount) break;
	}
	puts("");
	return superiors;
}

vector<DNA> changeGeneration(vector<DNA> prev){
	// 우성 유전자들을 교배/변이시킨 것들로 실험군을 다시 채운다.
	int goalSize = prev.size();
	vector<DNA> superiors = getSuperiors(prev);
	while(superiors.size() < goalSize){
		int curSize = superiors.size();
		for(int i=0; i<curSize; ++i){
			for(int j=curSize-1; j>i; ++j){
				DNA lChild = superiors[i].mating(superiors[j]);
				DNA rChild = superiors[j].mating(superiors[i]);
				superiors.push_back(lChild);
				if(superiors.size() >= goalSize) return superiors;
				superiors.push_back(rChild);
				if(superiors.size() >= goalSize) return superiors;
			}
		}
	}
	return superiors;
}

int main(){
	randomSeed();
	int answer = generateNumber(MAX_NUMBER);
	printf("answer: %d\n", answer);
	
	int totalGeneCount = 200;
	vector<DNA> generation(totalGeneCount);
	
	int nthGeneration = 0;
	while(++nthGeneration < 3){
		for(auto& dna : generation){
			// 첫번째 세대만 랜덤 유전자로 태어난다.
			if(nthGeneration == 1) dna.makeRandomDNA();
			dna.solve(answer);
			printf("%s(%d)\n", dna.getGene().c_str(), dna.getTryCount());
		}
		
		// 세대 교체 작업
		generation = changeGeneration(generation);
	}
	
	return 0;
}