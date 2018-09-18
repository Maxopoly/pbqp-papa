#ifndef CINTERFACE_H_
#define CINTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

int addNode(unsigned long* data, int length);

void addEdge(int sourceIndex, int targetIndex, unsigned long* data);

void dump();

#ifdef __cplusplus
} //end extern "C"
#endif



#endif /* CINTERFACE_H_ */
