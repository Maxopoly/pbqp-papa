#ifndef ANALYSIS_GRAPHHANDLER_CPP_
#define ANALYSIS_GRAPHHANDLER_CPP_

template<typename T>
class PBQP_Graph;

/**
 * Simple parent class for any class operating on a PBQPGraph (of which we have tons)
 */
template<typename T>
class PBQPHandler {

protected:
	PBQP_Graph<T>* const graph;

public:
	PBQPHandler(PBQP_Graph<T>* graph) : graph(graph) {
	}

	virtual ~PBQPHandler();
};



#endif /* ANALYSIS_GRAPHHANDLER_CPP_ */
