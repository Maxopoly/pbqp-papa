#ifndef CINTERFACE_H_
#define CINTERFACE_H_

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


int addNode(unsigned long* data, int length, int index);

void addEdge(int sourceIndex, int targetIndex, unsigned long* data);

void solve();

void enableNodeRemapping();

void setNodeAmount(unsigned int amount);

#ifdef __cplusplus
} //end extern "C"
#endif



#endif /* CINTERFACE_H_ */
