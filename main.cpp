#include <bits/stdc++.h>
using namespace std;

#define POPULATION  200
#define GENE_SIZE   10
#define MAX_NUMBER  100000
#define MUTATE_RATE 100
#define LAST_GENERATION 100

#define printRange(l, r, g) printf("[%6d, %6d](%5d) ", l, r, g);

void randomSeed(int seed = 0){
	srand(time(NULL) + seed);
}

int generateNumber(int max){
	return rand()%max + 1;
}

char numToHex(int num){
	if(num > 9) return num-10+'a';
	return num+'0';
}

class DNA {
private:
	string gene;
	int tryCount;
	int deviation;
public:
	DNA(){
		tryCount = 0;
		deviation = -1;
		gene.assign(GENE_SIZE, '0');
	}
	DNA(const DNA& d){
		tryCount = d.tryCount;
		deviation = d.deviation;
		gene = d.gene;
	}
	~DNA(){
		gene.clear();
	}
public:
	bool operator < (const DNA& d) const {
		if(getTryCount() == d.getTryCount()){
			return deviation < d.deviation;
		}
		return getTryCount() < d.getTryCount();
	}
	string getGene() const {
		return gene;
	}
	const int getTryCount() const {
		return tryCount;
	}
	void setDeviation() {
		deviation = 0;
		for(int i=1; i<gene.length(); ++i){
			deviation += abs(gene[i] - gene[i-1]);
		}
	}
public:
	void makeRandomDNA(){
		for(int i=0; i<GENE_SIZE; ++i){
			int percent = rand()%11; // [0, 10]
			gene[i] = percent > 9 ? (percent-10+'a') : (percent+'0');
		}
		setDeviation();
	}

	// 문제를 풀어본다. 몇 번만에 해결할까
	int solve(const int& answer, bool print = false){
		int lower=1, upper=MAX_NUMBER, tries=0;
		while(lower <= upper){
			int guess = guessNumber(lower, upper, tries++);
			
			if(print) printRange(lower, upper, guess);
			
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
		this->mate(opponent);
		
		// 교배 후에 일정 확률로 돌연변이가 나타난다.
		if( rand()%MUTATE_RATE == 0 )
			this->mutation();
		this->setDeviation();
		
		return *this;
	}
	
	// 돌연변이
	DNA mutation(){
		return this->mutateByClick();
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
	

	DNA mate(const DNA& opponent){
		// 간단한 구현을 위해 일점 교차를 사용
		string oppn = opponent.gene;
		
		int len = oppn.length();
		for(int i=0; i<len; ++i){
			if( i%2 ) this->gene[i] = oppn[i];
		}
		return *this;
	}
	
	DNA mutateByReverse(){
		std::reverse(this->gene.begin(), this->gene.end());
		return *this;
	}
	
	DNA mutateByClick(){
		// 염색체에서 2개 이상의 유전자가 변이된다.
		int changingPoint = rand()%(GENE_SIZE-2)+2;
		int length = this->gene.length();
		for(int i=0; i<changingPoint; ++i){
			char randomHex = numToHex(rand()%11); // [0, 11]
			this->gene[rand()%length] = randomHex;
		}
		return *this;
	}
};

vector<DNA> getSuperiors(vector<DNA> gen){
	// 상위 5%를 우성 유전자로 설정한다.
	int superiorsCount = gen.size() * 0.05;
	std::sort(gen.begin(), gen.end());
	
	vector<DNA> superiors;
	for(auto& dna : gen){
		superiors.push_back(dna);
		if(superiors.size() >= superiorsCount) break;
	}
	return superiors;
}

int selectCandidate(int full, int missrate){
	int bound = full - missrate;
	if(bound-1 <= 0) return 0;
	bound = rand()%(bound-1)+1;
	int num, limit = 100;
	do {
		num = rand()%full;
	} while(num > bound && limit-- > 0);
	return num;
}

void changeGeneration(vector<DNA>& prev){
	// 실험군을 다시 채운다.
	int goalSize = prev.size();
	
	vector<DNA> next, supers = getSuperiors(prev);
	// 상위 염색체들은 교배된다.
	for(auto& s1 : supers){
		for(auto& s2 : supers){
			next.push_back(s1.mating(s2));
		}
	}
	
	// 나머지는 우선 순위 기반 확률적으로 선택된다.
	while(next.size() < goalSize){
		for(int i=0; i<goalSize; ++i){
			int cand1 = selectCandidate(goalSize, i);
			int cand2 = selectCandidate(goalSize, i);
			DNA child = prev[cand1].mating(prev[cand2]);
			next.push_back(child);
			if(next.size() >= goalSize) break;
		}
	}
	prev.clear();
	prev = next;
}

void printProgramInfo(){
	printf("population of generation: %d\n", POPULATION);
	printf("length of gene: %d\n", GENE_SIZE);
	printf("range of answer: 1 ~ %d\n", MAX_NUMBER);
	printf("incidence of mutation: 1/%d (%.2lf%%)\n", MUTATE_RATE, 1./MUTATE_RATE*100);
	
	// 최악의 경우 MAX_NUMBER까지의 수를 구할 때의 연산 횟수 (log2(MAX_NUMBER))
	int worstCount = 0;
	for(int i=1; i<MAX_NUMBER; i*=2) worstCount += 1;
	printf("worst count of this game: %d\n", worstCount);
	
	puts("\n");
}

int countWithBinarySearch(const int& n){
	int l=0, r=MAX_NUMBER, count=0;
	while(l <= r){
		int mid = (l+r)/2;
		if(mid > n) r = mid-1;
		else l = mid+1;
		
		printRange(l, r, mid);
		count++;
	}
	puts("");
	return count;
}

int main(){
	randomSeed();
	printProgramInfo();
	
	int totalGeneCount = POPULATION;
	vector<DNA> generation(totalGeneCount);
	
	int nthGeneration = 0;
	while(++nthGeneration < LAST_GENERATION){
		int answer = generateNumber(MAX_NUMBER);
	
		for(auto& dna : generation){
			// 첫번째 세대만 랜덤 유전자로 태어난다.
			if(nthGeneration == 1) dna.makeRandomDNA();
			dna.solve(answer);
		}
		
		for(auto& d : getSuperiors(generation)){
			printf("%10s(%2d) ", d.getGene().c_str(), d.getTryCount());
		}
		puts("");
		
		// 세대 교체 작업
		changeGeneration(generation);
	}
	
	// 3번 정도 테스트
	for(int i=0; i<3; ++i){
		int newAnswer = generateNumber(MAX_NUMBER);
		printf("\n\nguess %d:\n", newAnswer);
		
		// 진화가 끝난 유전자로 문제를 해결해보기
		DNA lastChild = generation.front();
		lastChild.solve(newAnswer, true);
		puts("");
		
		// 바이너리 서치로 찾아보기
		countWithBinarySearch(newAnswer);
	}
	
	return 0;
}