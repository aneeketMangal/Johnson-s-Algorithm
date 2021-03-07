#include <bits/stdc++.h>
#include <time.h>
using namespace std;
#define inf 999999

class BinaryHeap
{
    pair<int, int> *heapArray;
    int heapSize;
    int *pointerLocation; //stores the position of nodes in heapArray according to vertices for o(1) access in decrease key

public:
    BinaryHeap(int capacity)
    {
        heapSize = 0;
        heapArray = new pair<int, int>[capacity];
        pointerLocation = new int[capacity];
    }
    //it swaps two pointers for percoalate up and also do the requierd change in pointer location
    void swapUtility(pair<int, int> *x, pair<int, int> *y, int xi, int yi)
    {
        pointerLocation[(*y).second] = xi;
        pointerLocation[(*x).second] = yi;
        pair<int, int> temp = *x;
        *x = *y;
        *y = temp;
    }
    int parent(int index)
    {
        return (index - 1) / 2;
    }
    void percolateUp(int index)
    {
        while (index && (heapArray[parent(index)].first >= heapArray[index].first))
        {
            if (heapArray[parent(index)].first > heapArray[index].first)
                swapUtility(&heapArray[index], &heapArray[parent(index)], index, parent(index));
            index = parent(index);
        }
    }
    void insertKey(int k, int weight)
    {
        int i = heapSize;
        heapArray[i] = {weight, k};
        pointerLocation[k] = i;
        percolateUp(i);
        heapSize += 1;
    }

    pair<int, int> extractMin()
    {
        if (heapSize == 1)
        {
            heapSize -= 1;
            return heapArray[0];
        }
        else
        {
            pair<int, int> minElement = heapArray[0];
            heapArray[0] = heapArray[heapSize - 1];
            heapSize -= 1;
            Heapify(0);
            return minElement;
        }
    }

    void decreaseKey(int v, int newValue)
    {
        int i = pointerLocation[v];
        heapArray[i].first = newValue;
        percolateUp(i);
    }
    void Heapify(int index)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int temp = index;
        if (left < heapSize && heapArray[left] < heapArray[temp])
            temp = left;
        if (right < heapSize && heapArray[right] < heapArray[temp])
            temp = right;
        if (temp != index)
        {
            swapUtility(&heapArray[index], &heapArray[temp], index, temp);
            Heapify(temp);
        }
    }

    bool empty()
    {
        return (heapSize == 0);
    }
};

class ArrayHeap
{
    int *heapArray;
    int heapCapacity;
    int check;

public:
    ArrayHeap(int capacity)
    {
        check = 0;
        heapCapacity = capacity;
        heapArray = new int[capacity];
    }
    void insertKey(int k, int weight)
    {
        heapArray[k] = weight;
    }

    pair<int, int> extractMin()
    {
        int minElement = inf;
        int minIndex = 0;
        for (int i = 0; i < heapCapacity; i++) //to find minValue
        {
            if (heapArray[i] < minElement)
            {
                minElement = heapArray[i];
                minIndex = i;
            }
        }

        heapArray[minIndex] = inf; //setting it to be inf so that it couldn't be found again
        return make_pair(minElement, minIndex);
    }

    void decreaseKey(int v, int newValue)
    {
        heapArray[v] = newValue;
    }
    bool empty()
    {
        check += 1;
        if (check == heapCapacity)
            return true;
        return false;
    }
};

class BinomialHeap //decrease key functionality does not work as intended so i have instead used insert key again and again
{
    typedef struct unit
    {
        int weight;
        int degree;
        int vertice;
        unit *parent, *child, *partner;
    } BinomialNode;
    BinomialNode *root;
    int totalVertices;
    vector<BinomialNode *> pointerLocation;

public:
    BinomialHeap(int n)
    {
        for (int i = 0; i < n; i++)
            pointerLocation.push_back(NULL);
        root = NULL;
        totalVertices = n;
    }

    BinomialNode *unionOperation(BinomialNode *a, BinomialNode *b)
    {
        if (a || b)
        {
            BinomialNode *temp;
            if (!a)
                temp = b;
            else if (!b) //one of the heap is null cases
                temp = a;
            else //merging in case both are not null
            {
                BinomialNode *tempTrail; //it  acts as a sort of tail to the temp heap and merges a and b
                                         // this is pretty similar to merge operation in merge sort
                if (a->degree > b->degree)
                {
                    temp = b;
                    b = b->partner;
                }
                else
                {
                    temp = a;
                    a = a->partner;
                }
                tempTrail = temp;
                while (a && b)
                {
                    if (a->degree <= b->degree)
                    {
                        tempTrail->partner = a;
                        a = a->partner;
                    }
                    else
                    {
                        tempTrail->partner = b;
                        b = b->partner;
                    }
                    tempTrail = tempTrail->partner;
                }
                if (!a)
                    tempTrail->partner = b;
                else
                    tempTrail->partner = a;
            }
            //core of union functionality for binolial heaps
            BinomialNode *x = NULL;
            BinomialNode *y = temp;
            BinomialNode *z = temp->partner;
            while (z != NULL)
            {
                // if (x == y && y == z)
                //     break;
                if (y->degree != z->degree)
                {
                    x = y;
                    y = z;
                }
                // cout << "tested";
                else if ((z->partner) && (z->partner)->degree == y->degree)
                {
                    x = y;
                    y = z;
                }
                else
                {
                    if (y->weight >= z->weight) //here we will have to increase degree of z
                    {
                        if (x == NULL)
                        {
                            temp = z;
                            z->degree = temp->degree + 1;
                        }
                        else
                        {
                            x->partner = z;
                            z->degree = z->degree + 1;
                        }

                        y->parent = z;
                        y->partner = z->child;
                        z->child = y;
                        y = z;
                    }
                    else //increasing degree of y
                    {
                        y->partner = z->partner;
                        z->parent = y;
                        z->partner = y->child;
                        y->child = z;
                        y->degree = y->degree + 1;
                    }
                }
                z = y->partner;
            }
            return temp;
        }
        else
            return NULL;
    }
    bool empty()
    {
        return (root == NULL);
    }

    void insertKey(int k, int weight)
    {
        BinomialNode *newNode = new (BinomialNode);
        newNode->weight = weight;
        newNode->vertice = k;

        newNode->child = NULL;
        newNode->degree = 0;
        newNode->parent = NULL;
        newNode->partner = NULL;
        root = unionOperation(root, newNode);
        pointerLocation[k] = newNode;
    }

    pair<int, int> extractMin()
    {
        //first we will find pointer to the min Value node, also the pointer to its previous element
        BinomialNode *magic = root;
        BinomialNode *temp = root;
        BinomialNode *temp2 = NULL;
        BinomialNode *temp3 = root;

        int minW = magic->weight;
        int minV = magic->vertice;
        while (temp3->partner)
        {
            if (temp3->partner == temp3)
                break;
            if ((temp3->partner)->weight < minW)
            {
                minW = temp3->weight;
                minV = temp3->vertice;
                temp2 = temp3;
                temp = temp3->partner;
            }
            temp3 = temp3->partner;
        }

        pair<BinomialNode *, BinomialNode *> minN = make_pair(temp2, temp);

        if (root == minN.second)
            root = minN.second->partner;

        if (!minN.first && !((minN.second)->partner))
            magic = NULL;
        else if (!minN.first)
            magic = minN.second->partner;
        else
            (minN.first)->partner = (minN.second)->partner;

        if (minN.second->child != NULL)
        {
            BinomialNode *child = minN.second->child;
            while (child != NULL)
            {
                BinomialNode *next = child->partner;
                child->partner = magic;
                child->parent = NULL;
                magic = child;
                child = next;
            }
            (minN.second->child)->partner = NULL;
        }
        else
            magic = NULL;

        pair<int, int> ans = make_pair(minN.second->weight, minN.second->vertice);
        BinomialNode *finalTemp = minN.second->child;
        totalVertices -= 1;
        root = unionOperation(root, magic);
        return ans;
    }
    void decreaseKey(int k, int newValue) //this function does not works as intended
    //there is some mistake which I could not debug till the end
    {
        BinomialNode *temp2 = pointerLocation[k];

        temp2->weight = newValue;
        BinomialNode *parent = temp2->parent;
        while (parent && (temp2->weight < parent->weight)) //basically percolate up functionality is served by below code
        {
            int cw = temp2->weight;
            int cv = temp2->vertice;
            int pw = parent->weight;
            int pv = parent->vertice;
            temp2->weight = pw;
            temp2->vertice = pv;
            parent->weight = cw;
            parent->vertice = cv;
            pointerLocation[cv] = parent;
            pointerLocation[pv] = temp2;
            temp2 = parent;
            parent = parent->parent;
        }
    }
};

class FibonacciHeap
{
    typedef struct unit
    {
        int weight;
        int degree;
        int vertice;
        bool marked;
        unit *parent, *child, *right, *left;
    } FibonacciNode;

protected:
    vector<FibonacciNode *> pointerLocation; //to do find operation in o(1) each time
    FibonacciNode *root;
    int totalVertices;

public:
    FibonacciHeap(int n)
    {
        for (int i = 0; i < n; i++)
            pointerLocation.push_back(NULL);
        root = NULL;
        totalVertices = n;
    }
    FibonacciNode *mergeOperation(FibonacciNode *a, FibonacciNode *b)
    {
        //no merge in case there is atleast one is null
        if (!a)
            return b;
        if (!b)
            return a;

        FibonacciNode *c = a->right;
        FibonacciNode *d = b->left;
        //interchanging a and b in case where a->weight > b->weight
        if (a->weight > b->weight)
        {
            c = b->right;
            d = a->left;
            FibonacciNode *temp = a;
            a = b;
            b = temp;
        }

        b->left = a;
        a->right = b;
        c->left = d;
        d->right = c;
        return a;
    }
    void insertKey(int k, int weight)
    {
        FibonacciNode *newNode = new FibonacciNode;
        newNode->weight = weight;
        newNode->vertice = k;
        newNode->degree = 0;
        newNode->marked = false;
        newNode->child = NULL;
        newNode->parent = NULL;
        newNode->right = newNode;
        newNode->left = newNode;
        pointerLocation[k] = newNode; //to help in decrease key operation
        root = mergeOperation(root, newNode);
    }

    pair<int, int> extractMin()
    {
        FibonacciNode *temp = root;
        pair<int, int> ans = make_pair(temp->weight, temp->vertice);
        root = extractMinHelper(root);
        delete temp;
        return ans;
    }
    FibonacciNode *extractMinHelper(FibonacciNode *currentRoot)
    {
        totalVertices -= 1;
        FibonacciNode *temp = currentRoot->child;
        if (temp)
        {
            FibonacciNode *temp2 = temp;
            while (1) //traversing child list and setting their parents = 0
            {
                temp2->marked = 0;    //unmarking the node
                temp2->parent = NULL; //setting all their parents as zero because they have been cutOperation
                temp2 = temp2->right;
                if (temp2 == temp)
                    break;
            }
        }
        //single fibonacci tree in the system
        if (currentRoot->right == currentRoot)
            currentRoot = currentRoot->child;
        else
        {
            FibonacciNode *nLeft = currentRoot->left;
            FibonacciNode *nRight = currentRoot->right;
            nRight->left = nLeft;
            nLeft->right = nRight;
            currentRoot = mergeOperation(currentRoot->right, currentRoot->child); //here we neglect currentRoot and link currentRoot's right and currentRoot's children
        }
        //finally listing all fibo trees in the heap according to of their degrees
        if (currentRoot)
        {
            vector<FibonacciNode *> trees; //I am assuming a threshold of 2^127 nodes.
            for (int i = 0; i < 128; i++)
                trees.push_back(NULL);
            while (1)
            {
                FibonacciNode *test = trees[currentRoot->degree];
                if (test == currentRoot)
                    break;
                if (test) //if there is a fibonacci tree of given degree present in the heap
                {
                    trees[currentRoot->degree] = NULL;
                    FibonacciNode *testL = test->left;
                    testL->right = test->right;
                    test->right->left = test->left;

                    //to make test as a child of currentRoot
                    if (currentRoot->weight < test->weight)
                    {
                        test->left = test;
                        test->right = test;
                        test->parent = currentRoot;
                        currentRoot->degree = currentRoot->degree + 1;
                        currentRoot->child = mergeOperation(currentRoot->child, test);
                    }

                    else
                    {
                        if (currentRoot->right != currentRoot)
                        {
                            FibonacciNode *cL = currentRoot->left;
                            FibonacciNode *cR = currentRoot->right;

                            cL->right = test;
                            cR->left = test;
                            test->right = cR;
                            test->left = cL;
                        }

                        else//single tree case
                        {
                            test->right = test;
                            test->left = test;
                        } 
                        currentRoot->left = currentRoot; //creating linked list of one element here
                        currentRoot->right = currentRoot;
                        currentRoot->parent = test;
                        test->degree = test->degree + 1;
                        test->child = mergeOperation(test->child, currentRoot);
                        currentRoot = test;
                    }
                    currentRoot = currentRoot->right; //to move to next part
                }
                else
                {
                    trees[currentRoot->degree] = currentRoot;
                    currentRoot = currentRoot->right;
                }
            }
            FibonacciNode *min = currentRoot;
            //setting new min element after removing previous min
            while (1)
            {
                if (currentRoot->weight < min->weight)
                    min = currentRoot;
                currentRoot = currentRoot->right;
                if (currentRoot == min)
                    break;
            }
            return min;
        }
        return NULL;
    }
    FibonacciNode *cutOperation(FibonacciNode *a, FibonacciNode *b) //serves the cut operation of fibo heap
    {
        FibonacciNode *bLeft = b->left;
        FibonacciNode *bRight = b->right;
        FibonacciNode *bParent = b->parent;

        //single fibTree case
        if (b->right == b)
            bParent->child = NULL;
        else
        {
            bRight->left = bLeft;
            bLeft->right = bRight;
            bParent->child = bRight;
        }
        b->right = b;
        b->left = b;
        b->marked = 0;
        bParent->degree = bParent->degree - 1; //reducing the degree by 1
        return mergeOperation(a, b);
    }
    void cascadingCutOperation(FibonacciNode *parent, FibonacciNode *found) //serves cascading cut functionality
    {
        while (parent)
        {
            if (parent->marked == false)
                break;
            root = cutOperation(root, parent);
            found = parent;
            parent = found->parent; 
            found->parent = NULL; //making the parent null
        }
        if (parent && parent->parent && parent->marked == false)
            parent->marked = true; //unmarked node will be marked
    }
    void decreaseKey(int k, int newValue)
    {
        //find the FibonacciNode first
        FibonacciNode *temp = root;
        FibonacciNode *found = pointerLocation[k]; //using pre stored pointer locations

        found->weight = newValue; //setting new value
        FibonacciNode *parent = found->parent;
        if (parent && found->weight < parent->weight)
        {
            root = cutOperation(root, found);
            found->parent = NULL;
            cascadingCutOperation(parent, found);
            //setting min
            if (found->weight < root->weight)
                root = found;
        }
        if (root) //this piece of code resets the min pointer to avoid any seg fault just in case
        {
            FibonacciNode *temp2 = root;
            while (1)
            {
                if (temp2->weight < root->weight)
                    root = temp2;
                temp2 = temp2->right;
                if (temp2 == root)
                    break;
            }
        }
    }

    bool empty()
    {
        return root == NULL;
    }
};

void dijkstra_FibonacciHeap(int n, list<pair<int, int>> *graph, int source, int *distanceByBellman)
{
    // cout << "here";
    int distance[n];
    fill_n(distance, n, inf);
    distance[source] = 0;

    FibonacciHeap minHeap(n);
    for (int i = 0; i < n; i++)
    {
        if (i == source)
            minHeap.insertKey(source, 0);
        else
            minHeap.insertKey(i, inf);
    }
    //to store element being processed
    // minHeap.insert(make_pair(distance[source], source));
    while (!minHeap.empty())
    {

        pair<int, int> topElement = minHeap.extractMin();
        int currWeight = topElement.first;
        int curr = topElement.second;
        for (list<pair<int, int>>::iterator x = graph[curr].begin(); x != graph[curr].end(); ++x)
        {
            if (distance[(*x).first] > distance[curr] + (*x).second)
            {

                distance[(*x).first] = distance[curr] + (*x).second;   //relaxing
                minHeap.decreaseKey((*x).first, distance[(*x).first]); //inserting updated edge in heap
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (distance[i] == inf)
            cout << inf << " ";
        else
            cout << (distance[i] - (distanceByBellman[source] - distanceByBellman[i])) << " ";
    }
    cout << endl;
}
void dijkstra_ArrayHeap(int n, list<pair<int, int>> *graph, int source, int *distanceByBellman)
{
    // cout << "gjgb" ;
    int distance[n];
    fill_n(distance, n, inf);
    distance[source] = 0;

    ArrayHeap minHeap(n);
    for (int i = 0; i < n; i++)
    {
        if (i == source)
            minHeap.insertKey(source, 0);
        else
            minHeap.insertKey(i, inf);
    }
    //to store element being processed
    // minHeap.insert(make_pair(distance[source], source));
    while (!minHeap.empty())
    {

        pair<int, int> topElement = minHeap.extractMin();
        int currWeight = topElement.first;
        int curr = topElement.second;
        for (list<pair<int, int>>::iterator x = graph[curr].begin(); x != graph[curr].end(); ++x)
        {
            if (distance[(*x).first] > distance[curr] + (*x).second)
            {

                distance[(*x).first] = distance[curr] + (*x).second;   //relaxing
                minHeap.decreaseKey((*x).first, distance[(*x).first]); //inserting updated edge in heap
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (distance[i] == inf)
            cout << inf << " ";
        else
            cout << (distance[i] - (distanceByBellman[source] - distanceByBellman[i])) << " ";
    }
    cout << endl;
}
void dijkstra_BinaryHeap(int n, list<pair<int, int>> *graph, int source, int *distanceByBellman)
{
    int distance[n];
    fill_n(distance, n, inf);
    distance[source] = 0;

    BinaryHeap minHeap(n);
    for (int i = 0; i < n; i++)
    {
        if (i == source)
            minHeap.insertKey(source, 0);
        else
            minHeap.insertKey(i, inf);
    }
    //to store element being processed
    // minHeap.insert(make_pair(distance[source], source));
    while (!minHeap.empty())
    {

        pair<int, int> topElement = minHeap.extractMin();
        int currWeight = topElement.first;
        int curr = topElement.second;
        for (list<pair<int, int>>::iterator x = graph[curr].begin(); x != graph[curr].end(); ++x)
        {
            if (distance[(*x).first] > distance[curr] + (*x).second)
            {

                distance[(*x).first] = distance[curr] + (*x).second;   //relaxing
                minHeap.decreaseKey((*x).first, distance[(*x).first]); //inserting updated edge in heap
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (distance[i] == inf)
            cout << inf << " ";
        else
            cout << (distance[i] - (distanceByBellman[source] - distanceByBellman[i])) << " ";
    }
    cout << endl;
}
void dijkstra_BinomialHeap(int n, list<pair<int, int>> *graph, int source, int *distanceByBellman)
{
    int distance[n];
    fill_n(distance, n, inf);
    distance[source] = 0;

    BinomialHeap minHeap(n);
    for (int i = 0; i < n; i++)
    {
        if (i == source)
            minHeap.insertKey(source, 0);
        else
            minHeap.insertKey(i, inf);
    }
    //to store element being processed
    // minHeap.insert(make_pair(distance[source], source));
    while (!minHeap.empty())
    {

        pair<int, int> topElement = minHeap.extractMin();
        int currWeight = topElement.first;
        int curr = topElement.second;
        for (list<pair<int, int>>::iterator x = graph[curr].begin(); x != graph[curr].end(); ++x)
        {
            if (distance[(*x).first] > distance[curr] + (*x).second)
            {

                distance[(*x).first] = distance[curr] + (*x).second; //relaxing
                minHeap.insertKey((*x).first, distance[(*x).first]); //inserting updated edge in heap
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (distance[i] == inf)
            cout << inf << " ";
        else
            cout << (distance[i] - (distanceByBellman[source] - distanceByBellman[i])) << " ";
    }
    cout << endl;
}

int bellmanFord(int n, list<pair<int, int>> *graph, int source, int *distance)
{

    fill_n(distance, n, inf);
    distance[source] = 0;
    pair<bool, int> change[n];
    for (int i = 0; i < n; i++)
        change[i] = make_pair(0, 0);
    change[source].first = 1;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (change[j].first == true) //this if check ensures if children of particular node are to be updated or not, reeducing time complexity
            {
                change[j].first = 0;
                for (list<pair<int, int>>::iterator x = graph[j].begin(); x != graph[j].end(); ++x)
                {

                    if (distance[(*x).first] > distance[j] + (*x).second)
                    {
                        distance[(*x).first] = distance[j] + (*x).second;
                        change[(*x).first].second = change[j].second + 1;
                        change[(*x).first].first = 1;
                    }
                }
            }
        }
    }
    for (int j = 0; j < n; j++)
    {
        for (list<pair<int, int>>::iterator x = graph[j].begin(); x != graph[j].end(); ++x)
        {

            if (distance[(*x).first] > distance[j] + (*x).second)
                return 1;
        }
    }
    for (int j = 0; j < n; j++)
    {
        for (list<pair<int, int>>::iterator x = graph[j].begin(); x != graph[j].end(); ++x)
            (*x).second += (distance[j] - distance[(*x).first]);
    }
    return 0;
}

int main(int argc, char **argv)
{
    int test;
    cin >> test;
    double time[test];
    for (int k = 0; k < test; k++)
    {
        clock_t begin = clock();
        int n, D;
        cin >> n >> D;
        int distance[n] = {0};
        list<pair<int, int>> graph[n + 1];
        int temp;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> temp;

                if (temp != inf && j != i)
                    graph[i].push_back(make_pair(j, temp));
            }
        }

        int error = 0;
        if (D == 1)
        {
            for (int j = 0; j < n; j++)
                graph[n].push_back(make_pair(j, 0));
            error = bellmanFord(n + 1, graph, n, distance);
        }

        for (int j = 0; j < n; j++)
        {
            for (auto k : graph[j])
            {
                if (k.second < 0)
                {
                    error = 1;
                    break;
                }
            }
        }
        
        if (!error)
        {
            for (int j = 0; j < n; j++)
            {
                if (argc == 1)
                    dijkstra_FibonacciHeap(n, graph, j, distance);
                else
                {
                    // cout << "yeah";
                    if (argv[1] == string("1"))
                        dijkstra_ArrayHeap(n, graph, j, distance);
                    if (argv[1] == string("2"))
                        dijkstra_BinaryHeap(n, graph, j, distance);
                    if (argv[1] == string("3"))
                        dijkstra_BinomialHeap(n, graph, j, distance);
                    if (argv[1] == string("4"))
                        dijkstra_FibonacciHeap(n, graph, j, distance);
                }
            }
        }

        else
            cout << -1 << endl;

        clock_t end = clock();
        double time_taken = (double)(end - begin) / CLOCKS_PER_SEC;
        time[k] = time_taken;
    }
    for (int k = 0; k < test; k++)
        cout << fixed << setprecision(10) << time[k] << " ";
    cout << endl;
}
