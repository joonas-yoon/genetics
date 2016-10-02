## Genetic Algorithm

#### Description

유전 알고리즘을 통해 이진탐색(Binary search)을 학습시키고 싶었다.

염색체를 DNA라는 클래스로 정의했으며, 염색체는 다음과 같은 유전자 정보를 가진다.

- gene: 염색체
- tryCount: 정답을 맞추기까지 걸린 시도 횟수
- deviation: 유전자 사이의 거리 차이

각 유전자는 1부터 10사이의 정수를 의미하며, Hex값으로 표현된다. 이 숫자는 전체 구간에서 검색할 구간의 비율을 의미한다. 예를 들어, 5라면 구간의 50% 지점을 mid로 설정해본다.

염색체는 `GENE_SIZE`의 길이만큼을 주기로 유전자의 정보를 읽는다.

Binary search는 매번 절반을 쪼개 확인하므로, 이 프로그램의 염색체로 표현하자면 `555555...`와 같다. 따라서, 이번 연구의 목표는 `55555....`로 최종진화하는 것이다.

#### Testcases

- 1-1.out ~ 1-5.out:
```
population of generation: 200
length of gene: 10
range of answer: 1 ~ 100000
incidence of mutation: 1/100 (1.00%)
worst count of this game: 17

제 100세대까지의 진화를 확인했다.
```
유전자 정보가 안정되지 않는다. 돌연변이 발생률을 낮추거나, 500세대까지 진화를 지켜보거나 해야겠다.

- 2-1.out ~ 2-3.out:
```
population of generation: 200
length of gene: 10
range of answer: 1 ~ 1000000
incidence of mutation: 1/200 (0.50%)
worst count of this game: 20

제 200세대까지의 진화를 확인했다.
```
2-1: 정해보다 수를  2.2% 빠르게 찾게 진화함

2-2: 정해보다 수를 10.4% 느리게 찾게 진화함

2-3: 정해보다 수를  3.3% 느리게 찾게 진화함

...

평균적으로 얼마나 빠르게 진화하는 지 알아보았더니, 4~5% 정도 이진탐색보다 느리게 검색한다.

(5% 느리다는 의미는 이진탐색이 100번만에 찾는다면, 진화가 끝난 유전자는 95번만에 찾는다는 뜻)

가장 빠른 경우는 3% 전후인 반면, 가장 느린 경우에는 27% 전후로 비교적 많이 느리게 진화한다.


#### Updates

16/10/02[commits#b605ff802] : 교차 방식을 2점 교차로 변경, 0 ~ MAX_NUMBER 까지의 모든 수에 대해서 적합도를 테스트한다.

#### Compile option

```
g++ -std=c++11 -O2
```

#### make.sh

makefile와 같은 기능을 수행할 쉘 스크립트 파일이다.

```
$ chmod +x ./make.sh
$ ./make.sh [file]
```
- file이 없다면 main.cpp 를 자동으로 찾아서 컴파일 후 실행한다.
- 입력은 "파일명.in"을 읽고, 출력은 콘솔과 "파일명.out"으로 출력된다.
