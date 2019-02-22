#ifndef ANALYSIS_GRAPHHANDLER_CPP_
#define ANALYSIS_GRAPHHANDLER_CPP_

namespace pbqppapa {

template<typename T>
class PBQPGraph;

/**
 * Simple parent class for any class operating on a PBQPGraph (of which we have tons).
 * Not sure if making (abstract) parent classes like this really is the way to do things in C++
 * or just a habit I brought in from java
 */
template<typename T>
class PBQPHandler {

protected:
	PBQPGraph<T>* const graph;

public:
	PBQPHandler(PBQPGraph<T>* graph) :
			graph(graph) {
	}

	virtual ~PBQPHandler() {
	}
};

}

#endif /* ANALYSIS_GRAPHHANDLER_CPP_ */
