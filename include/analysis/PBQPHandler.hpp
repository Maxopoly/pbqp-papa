#ifndef ANALYSIS_GRAPHHANDLER_CPP_
#define ANALYSIS_GRAPHHANDLER_CPP_

template<typename T>
class PBQPGraph;

/**
 * Simple parent class for any class operating on a PBQPGraph (of which we have tons)
 */
template<typename T>
class PBQPHandler {

protected:
	PBQPGraph<T>* const graph;

public:
	PBQPHandler(PBQPGraph<T>* graph) :
			graph(graph) {
	}

	virtual ~PBQPHandler();
};

#endif /* ANALYSIS_GRAPHHANDLER_CPP_ */
