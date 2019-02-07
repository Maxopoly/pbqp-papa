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

#define CINTERFACESIG(TYPENAME,SHORTNAME) \
struct pbqp_ ## SHORTNAME ## _parsing; \
struct pbqp_ ## SHORTNAME ## _solution { \
unsigned short* selections; \
unsigned int length; \
}; \
struct pbqp_ ## SHORTNAME ## _edge { \
unsigned int sourceIndex; \
unsigned int targetIndex; \
unsigned short sourceLength; \
unsigned short targetLength; \
TYPENAME* matrixData; \
}; \
struct pbqp_ ## SHORTNAME ## _node { \
unsigned int index; \
unsigned short vectorLength; \
TYPENAME* vectorData; \
unsigned short incidentEdgeCount; \
pbqp_ ## SHORTNAME ## _edge** incidentEdges; \
}; \
unsigned int pbqp_ ## SHORTNAME ## _addNode(struct pbqp_ ## SHORTNAME ## _parsing*, TYPENAME* data, \
		unsigned short length, unsigned int index); \
void pbqp_ ## SHORTNAME ## _addEdge(struct pbqp_ ## SHORTNAME ## _parsing*, unsigned int sourceIndex, \
		unsigned int targetIndex, TYPENAME* data); \
struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _solve(struct pbqp_ ## SHORTNAME ## _parsing*); \
struct pbqp_ ## SHORTNAME ## _parsing* pbqp_ ## SHORTNAME ## _createInstance( \
		bool useNodeRemapping, unsigned int nodeAmount); \
void pbqp_ ## SHORTNAME ## _addToPEO(struct pbqp_ ## SHORTNAME ## _parsing*, unsigned int index); \
void pbqp_ ## SHORTNAME ## _finishPEO(struct pbqp_ ## SHORTNAME ## _parsing*); \
void pbqp_ ## SHORTNAME ## _dump(struct pbqp_ ## SHORTNAME ## _parsing*, char* path); \
void pbqp_ ## SHORTNAME ## _free(struct pbqp_ ## SHORTNAME ## _parsing*); \
//struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _solveGurobi(struct pbqp_ ## SHORTNAME ## _parsing*);

CINTERFACESIG(unsigned int,uint)

CINTERFACESIG(unsigned short,ushort)

CINTERFACESIG(unsigned long,ulong)

/*
 CINTERFACESIG(unsigned char,uchar)

 CINTERFACESIG(float,float)

 CINTERFACESIG(double,double) */

#ifdef __cplusplus
} //end extern "C"
#endif

#endif /* CINTERFACE_H_ */
