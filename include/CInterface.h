#ifndef CINTERFACE_H_
#define CINTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

int addNode(unsigned long* data, int length, int index);

void addEdge(int sourceIndex, int targetIndex, unsigned long* data);

void dump();

void enableNodeRemapping();

void setNodeAmount(unsigned int amount);

#ifdef __cplusplus
} //end extern "C"
#endif



#endif /* CINTERFACE_H_ */
