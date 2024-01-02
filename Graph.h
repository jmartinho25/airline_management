
#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstddef>
#include <vector>
#include <queue>
#include <stack>
#include <list>

using namespace std;

template<class T>
class Edge;

template<class T>
class Graph;

template<class T>
class Vertex;


/****************** Provided structures  ********************/

/**
 * @brief Defines what a Vertex is and its composition
 */
template<class T>
class Vertex {
    T info;                // contents
    vector<Edge<T> > adj;  // list of outgoing edges
    bool visited;          // auxiliary field
    bool processing;       // auxiliary field
    int indegree;          // auxiliary field
    int num;               // auxiliary field
    int low;               // auxiliary field

    void addEdge(Vertex<T> *d, double w, string airline);

    bool removeEdgeTo(Vertex<T> *d);

public:
    Vertex(T in);

    T getInfo() const;

    void setInfo(T in);

    bool isVisited() const;

    void setVisited(bool v);

    bool isProcessing() const;

    void setProcessing(bool p);

    const vector<Edge<T>> &getAdj() const;

    void setAdj(const vector<Edge<T>> &adj);

    int getIndegree() const;

    void setIndegree(int indegree);

    int getNum() const;

    void setNum(int num);

    int getLow() const;

    void setLow(int low);

    friend class Graph<T>;

    void increaseInDegree() {
        indegree++;

    }
};

/**
 * @brief Defines what an Edge is and its composition
 */
template<class T>
class Edge {
    Vertex<T> *dest;      // destination vertex
    double weight;         // edge weight
    std::string airlineCode;
public:
    Edge(Vertex<T> *d, double w, std::string airline);

    Vertex<T> *getDest() const;

    void setDest(Vertex<T> *dest);

    double getWeight() const;

    void setWeight(double weight);

    friend class Graph<T>;

    friend class Vertex<T>;

    std::string getAirlineCode() const;
};

/**
 * @brief Defines what a Graph is and its composition
 */
template<class T>
class Graph {
    vector<Vertex<T> *> vertexSet;      // vertex set
    int _index_;                        // auxiliary field
    stack<Vertex<T>> _stack_;           // auxiliary field
    list<list<T>> _list_sccs_;        // auxiliary field

    void dfsVisit(Vertex<T> *v, vector<T> &res) const;

    bool dfsIsDAG(Vertex<T> *v) const;

public:
    Vertex<T> *findVertex(const T &in) const;

    int getNumVertex() const;

    bool addVertex(const T &in);

    bool removeVertex(const T &in);

    bool addEdge(const T &sourc, const T &dest, double w, std::string airline);

    bool removeEdge(const T &sourc, const T &dest);

    vector<Vertex<T> *> getVertexSet() const;

    vector<T> dfs() const;

    vector<T> dfs(const T &source) const;

    vector<T> bfs(const T &source) const;

    vector<T> topsort() const;

    bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

/**
    * @brief Constructor of the Vertex class.
    * @details Time complexity: O(1)
    * @param in Vertex information
    */
template<class T>
Vertex<T>::Vertex(T in): info(in) {}

/**
    * @brief Constructor of the Edge class.
    * @details Time complexity: O(1)
    * @param d Edge destination vertex
    * @param w Edge weight
    * @param airline Edge airline, as each edge will represent a flight
    */
template<class T>
Edge<T>::Edge(Vertex<T> *d, double w, std::string airline)
        : dest(d), weight(w), airlineCode(airline) {}


/**
* @brief Gets the number of vertexes in a graph.
* @details Time complexity: O(1)
* @return Number of vertexes in a specific Graph (using the size of the vertexSet)
*/
template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

/**
* @brief Gets the set of vertexes in a graph.
* @details Time complexity: O(1)
* @return Set of vertexes in a specific Graph (in this case a vector containing all the vertexes of the graph)
*/
template<class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/**
* @brief Gets the information of a vertex.
* @details Time complexity: O(1)
* @return Information of the vertex
*/
template<class T>
T Vertex<T>::getInfo() const {
    return info;
}

/**
* @brief Sets the information of a vertex to the information received as parameter
* @param in Information to be set as the vertex information
*/
template<class T>
void Vertex<T>::setInfo(T in) {
    Vertex::info = in;
}

/**
* @brief Gets the process state of a vertex
* @details Time complexity: O(1)
* @return True if the vertex is processing, False otherwise
*/
template<class T>
bool Vertex<T>::isProcessing() const {
    return processing;
}

/**
* @brief Sets the process state of a vertex to the state received as parameter
* @details Time complexity: O(1)
* @param p Process state to be set as the vertex process state
*/
template<class T>
void Vertex<T>::setProcessing(bool p) {
    Vertex::processing = p;
}

/**
* @brief Gets the destination vertex of an edge
* @details Time complexity: O(1)
* @return Destination vertex of the edge
*/
template<class T>
Vertex<T> *Edge<T>::getDest() const {
    return dest;
}


template<class T>
void Edge<T>::setDest(Vertex<T> *d) {
    Edge::dest = d;
}

template<class T>
double Edge<T>::getWeight() const {
    return weight;
}

/**
* @brief Gets the code of the edge airline
* @details Time complexity: O(1)
* @return Airline code
*/
template<class T>
std::string Edge<T>::getAirlineCode() const {
    return airlineCode;
}


template<class T>
void Edge<T>::setWeight(double weight) {
    Edge::weight = weight;
}

/**
 * @brief Function to find a vertex with a given content(information).
 * @details Time complexity: O(V) where V is the number of vertices in vertexSet
 * @param in Information to be used in th search of the vertex
 * @return The vertex with the given information if it´s found, NULL if any vertex with that information is found
 */
template<class T>
Vertex<T> *Graph<T>::findVertex(const T &in) const {
    for (auto v: vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/**
* @brief Gets the visit state of a vertex
* @details Time complexity: O(1)
* @return True if the vertex has already been visited, False otherwise
*/
template<class T>
bool Vertex<T>::isVisited() const {
    return visited;
}

/**
* @brief Gets the number of incoming edges of a given vertex
* @details Time complexity: O(1)
* @return Number of incoming edges of the vertex
*/
template<class T>
int Vertex<T>::getIndegree() const {
    return indegree;
}

/**
* @brief Sets the number of incoming edges of a given vertex to a value given as parameter
* @details Time complexity: O(1)
* @param indegree Number to be set as the number of incoming edges of the vertex
*/
template<class T>
void Vertex<T>::setIndegree(int indegree) {
    Vertex::indegree = indegree;
}

/**
* @brief Gets the order in which the vertex is visited (num)
* @details Time complexity: O(1)
* @return Order in which the vertex is visited (num)
*/
template<class T>
int Vertex<T>::getNum() const {
    return num;
}

/**
* @brief Sets the order in which the vertex is visited (num) to a value given as parameter
* @details Time complexity: O(1)
* @param num Number to be set as the order in which the vertex is visited (num)
*/
template<class T>
void Vertex<T>::setNum(int num) {
    Vertex::num = num;
}

/**
* @brief Gets the smallest num reachable by the subtree that starts in the vertex (low)
* @details Time complexity: O(1)
* @return Smallest num reachable by the subtree that starts in the vertex (low)
*/
template<class T>
int Vertex<T>::getLow() const {
    return low;
}

template<class T>
void Vertex<T>::setLow(int low) {
    Vertex::low = low;
}

/**
* @brief Sets the visit state of a vertex to the state received as parameter
* @details Time complexity: O(1)
* @param v Visit state to be set as the vertex visit state
*/
template<class T>
void Vertex<T>::setVisited(bool v) {
    Vertex::visited = v;
}

/**
* @brief Gets the outgoing edges of the vertex
* @details Time complexity: O(1)
* @return Vector including all outgoing edges of the vertex
*/
template<class T>
const vector<Edge<T>> &Vertex<T>::getAdj() const {
    return adj;
}

template<class T>
void Vertex<T>::setAdj(const vector<Edge<T>> &adj) {
    Vertex::adj = adj;
}


/**
 * @brief  Adds a vertex with a given content or info (in) to a graph.
 * @param in Vertex information
 * @return True if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}


/**
 * @brief Adds an edge to a graph (this), given the contents of the source and destination vertices and the edge weight (w).
 * @param sourc Source Vertex
 * @param dest Destination Vertex
 * @param w Edge Weight
 * @param airline Edge/Flight airline
 * @return True if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w, std::string airline) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2, w, airline);
    return true;
}

/**
 * @brief Function to add an outgoing edge to a vertex, with a given destination vertex (d), edge weight (w) and airline.
 * @details Time complexity: O(1)
 * @param d Destination vertex of the edge
 * @param w Weight of the edge
 * @param airline Airline of the edge
 */
template<class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w, std::string airline) {
    adj.push_back(Edge<T>(d, w, airline));
}

/**
 * @brief Function to remove an outgoing edge (with a given destination (d)) from a vertex.
 * @details O(E) where E is the number of outgoing edges from the current vertex
 * @param d Destination vertex of the edge
 * @return True if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    for (auto it = adj.begin(); it != adj.end(); it++)
        if (it->dest == d) {
            adj.erase(it);
            return true;
        }
    return false;
}

/****************** BFS ********************/
/**
 * @brief Performs a breadth-first search (bfs) in a graph, starting from the vertex with the given source contents (source).
 * @details Time complexity: O(V + E) where V is the number of vertices and E is the number of edges in the graph.
 * @param source Information of the source vertex
 * @return Vector with the contents of the vertices by bfs order.
 */
template<class T>
vector<T> Graph<T>::bfs(const T &source) const {
    vector<T> res;
    auto s = findVertex(source);
    if (s == NULL)
        return res;
    queue<Vertex<T> *> q;
    for (auto v: vertexSet)
        v->visited = false;
    q.push(s);
    s->visited = true;
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        res.push_back(v->info);
        for (auto &e: v->adj) {
            auto w = e.dest;
            if (!w->visited) {
                q.push(w);
                w->visited = true;
            }
        }
    }
    return res;
}

#endif /* GRAPH_H_ */
