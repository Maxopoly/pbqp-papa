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
unsigned int pbqp_ ## SHORTNAME ## _addNode(pbqp_ ## SHORTNAME ## _parsing*, TYPENAME* data, \
		unsigned short length, unsigned int index); \
void pbqp_ ## SHORTNAME ## _addEdge(pbqp_ ## SHORTNAME ## _parsing*, unsigned int sourceIndex, \
		unsigned int targetIndex, TYPENAME* data); \
void pbqp_ ## SHORTNAME ## _solve(pbqp_ ## SHORTNAME ## _parsing*); \
pbqp_ ## SHORTNAME ## _parsing* pbqp_ ## SHORTNAME ## _createInstance( \
		bool useNodeRemapping, unsigned int nodeAmount); \
void pbqp_ ## SHORTNAME ## _addToPEO(pbqp_ ## SHORTNAME ## _parsing*, unsigned int index); \
void pbqp_ ## SHORTNAME ## _dump(pbqp_ ## SHORTNAME ## _parsing*, char* path); \
void pbqp_ ## SHORTNAME ## _free(pbqp_ ## SHORTNAME ## _parsing*);


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
