#ifndef CINTERFACE_H_
#define CINTERFACE_H_

#ifdef __cplusplus
#include <vector>
#include <map>
#include "graph/PBQPGraph.hpp"
#include "math/InfinityWrapper.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*

#define cInterfaceSignature(X)   \
unsigned int addNode(X* data, unsigned short length, unsigned int index); \
void addEdge(unsigned int sourceIndex, unsigned int targetIndex, X* data); \
void dump(char* path, X type); \
void resetAndPrepare(unsigned int nodeAmount, bool useNodeRemapping);

cInterfaceSignature(unsigned int)

cInterfaceSignature(unsigned short)

cInterfaceSignature(unsigned long)

cInterfaceSignature(unsigned char)

cInterfaceSignature(float)

cInterfaceSignature(double) */

struct pbqpparsing {

#ifdef __cplusplus

PBQPGraph<InfinityWrapper<unsigned long>>* graph;
std::map<int, PBQPNode<InfinityWrapper<unsigned long>>*> nodes;
std::vector<PBQPNode<InfinityWrapper<unsigned long>>*> peo;
bool remapIndices = false;
static unsigned long counter = 0;

#endif

};


int addNode(pbqpparsing*,unsigned long* data, int length, int index);

void addEdgePBQP(pbqpparsing*, int sourceIndex, int targetIndex, unsigned long* data);

void solvePBQP(pbqpparsing*);

pbqpparsing* createInstancePBQP(bool useNodeRemapping, int nodeAmount);

void addToPEOPBQP(pbqpparsing*, int index);

void dumpPBQP(pbqpparsing*, char* path);

void freePBQP(pbqpparsing*);


#ifdef __cplusplus
} //end extern "C"
#endif



#endif /* CINTERFACE_H_ */
