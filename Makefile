out=out
src=src
Projects=Projects
ProjectsOut=${Projects}/out
Project=Blink

CXX=g++
CXXFLAGS=-Isrc -std=c++11
LDFLAGS=-lSDL2

all: ${Projects}/${Project}

${Projects}/%: ${ProjectsOut}/%.o ${out}/main.o
	${CXX} -o $(shell echo $< | perl -pe 's|.o$$||') $^ ${LDFLAGS} ${CXXFLAGS}

${out}/%.o: ${src}/%.cc
	${CXX} -o $@ -c $< ${CXXFLAGS}
${out}/%.o: ${src}/%.cc
	${CXX} -o $@ -c $< ${CXXFLAGS}
${ProjectsOut}/%.o: ${Projects}/%.cc
	${CXX} -o $@ -c $< ${CXXFLAGS}
