
  <h3 align="center">Johnson's Algorithm</h3>




## About The Project
* <h3><a href="https://en.wikipedia.org/wiki/Johnson%27s_algorithm">About the algorithm</a></h3>

### Input format

* First line: Number of Test Cases <b>T</b>
* For each test case, First line/row indicate N D (single space separated); where,
     1. <b>N</b> is number of vertices in directed graph where vertices are labelled 1 to N (not 0 indexing)
     2. <b>D</b> specifies whether Directed Graph or undirected graph (D=0 means undirected, else it is directed)
     3. This is then followed by N lines each containing N integers (basically NxN Adjacency or say weight matrix representation of the graph of that test case).

### Assumptions
* There are no self-loops in the graph
* Diagonal entries are zero.
* The weight = 999999 indicates that there is no edge between those two vertices considered.
* -999 ≤ Adj matrix values ≤ 999 
* Whenever choices with equal considerations, prefer to consider vertices in lexicographic ordering.


## Output format
* If there is any negative weight cycle in graph, print -1.
* Else print NxN shortest path matrix.
Print 999999 if the vertex is not reachable from the source vertex considered.

## Instructions to run
<b>First compile the code using g++ main.cpp</b>
* ./a.out 1
* ./a.out 2
* ./a.out 3
* ./a.out 4 OR ./a.out
1: Arrays Heap, 2: binary Heap, 3: Binomial Heap, 4: Fibonacci Heap, Default: Fibonacci Heap
