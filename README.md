## Genetic Algorithm

#### options

g++ -std=c++11 -O2

#### make.sh

makefile와 같은 기능을 수행할 쉘 스크립트 파일이다.

```
$ chmod +x ./make.sh
$ ./make.sh [file]
```
- file이 없다면 main.cpp 를 자동으로 찾아서 컴파일 후 실행한다.
- 입력은 "파일명.in"을 읽고, 출력은 콘솔과 "파일명.out"으로 출력된다.

#### Rule

- 70% 교차율을 적용
-  1% 돌연변이 발생을 적용