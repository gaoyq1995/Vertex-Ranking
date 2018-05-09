#include <iostream>
#include <time.h>
#include <list>
#include <stack>
#include <vector>
using namespace std;

int V = 9;   //总结点数

struct adjNode  //边表结点
{
	int adjvertex;  //邻接点域
	float weight;   //权重
	adjNode *next;   //指向下一个邻接点
};

struct Node    //顶点表节点
{
	int vertex;     //顶点域
	adjNode *firstedge;  //边表头指针
	Node *nextNode;     //指向下一个顶点
};

class Edge
{
public:
	int fromNode;
	int toNode;
	float weight;
	int operator==(const Edge &e){return (this->fromNode == e.fromNode && this->toNode == e.toNode);}
	int operator!=(const Edge &e){return (this->fromNode != e.fromNode || this->toNode != e.toNode);}
};

class GraphLinkedTable   //图的邻接链表表示
{
public:
	Node *head;   //顶点表头指针
	int VertexCount;    //顶点数目
	int *Vertex;   //保存当前图中所有顶点的id
	int Nspanningtrees;   //生成树个数
	vector<vector <GraphLinkedTable>> SpanningForest;     //生成森林

	GraphLinkedTable operator=(const GraphLinkedTable &gr);

	GraphLinkedTable();
	GraphLinkedTable(int count);
	~GraphLinkedTable(){};
	void setVertexCount(int count){ VertexCount = count;};
	void setVertex();
	bool ownVertex(int node);
	void InsertEdge(int from, int to, float weight);
	void RemoveEdge(int from, int to);  //从图中删除有向边(from,to)
	void RemoveNode (int node);    //从图中删除结点node
	void BuildGraph();
	void Print();
	int getConnectedVertices();   //图中连通的顶点数
	int getIndex(int node);   //获得node在Vertex中的下标
	void DFS(int node, bool *visited, float *distance);   //从node结点出发DFS遍历
	bool IsDescendant(int form,int to);   //判断to是否为from的后裔结点
	int *Distance(int node);
	void ChangeWeight(int from, int to);

	float SumDistance(int node);   //生成树中结点node到其他结点的距离之和
	float AvgSumDistance(int node);
	float Eccentricity(int node);
	float AvgEcc(int node);
	float *Vector();
	float NetworkDistance(GraphLinkedTable G, int vertexnum);

	void Find();
};

GraphLinkedTable GraphLinkedTable::operator=(const GraphLinkedTable &gr)
{
	Nspanningtrees = gr.Nspanningtrees;
	head = gr.head;
	Vertex = gr.Vertex;
	VertexCount = gr.VertexCount;
	return gr;
}

GraphLinkedTable::GraphLinkedTable()
{
	Nspanningtrees = 0;
	head = new Node;
	head -> firstedge = NULL;
	head -> vertex = -1;
	head -> nextNode = NULL;
}

GraphLinkedTable::GraphLinkedTable(int count)    //构造函数
{
	setVertexCount(count);
	setVertex();
	Nspanningtrees = 0;
	head = new Node;
	head -> firstedge = NULL;
	head -> vertex = -1;
	head -> nextNode = NULL;
}  

void GraphLinkedTable::setVertex()
{
	Vertex = new int [VertexCount];
	for (int i = 0;i < VertexCount; i++)
		Vertex[i] = -1;
}

bool GraphLinkedTable::ownVertex(int node)
{
	Node * h = head;
	while(h -> vertex != -1)
	{
		if (h -> vertex == node && h->firstedge!=NULL)
			return true;
		else
		{
			adjNode * adj = h->firstedge;
			while(adj != NULL)
			{
				if (adj -> adjvertex == node)
					return true;
				adj = adj -> next;
			}
		}
		h = h -> nextNode;
	}
	return false;
}

void GraphLinkedTable::InsertEdge(int from, int to, float weight)  //插入有向边(from,to)
{
	Node *h = head;
    Node * temp;

	//更新Vertex
	int ind = getIndex(from);
	if (ind == -1)
	{
		int i = 0;
		for (i = 0; Vertex[i] != -1 && i < VertexCount; i++);
		Vertex[i] = from;
	}
	ind = getIndex(to);
	if (ind == -1)
	{
		int i = 0;
		for (i = 0; Vertex[i] != -1 && i < VertexCount; i++);
		Vertex[i] = to;
	}

	if (head -> vertex == -1)  //当前顶点表为空，则图也为空
	{
		head -> vertex = from;
		adjNode *newnode = new adjNode;
		newnode -> adjvertex = to;
		newnode -> weight = weight;
		newnode -> next = NULL;
		head -> firstedge = newnode;

		Node *new_to = new Node;  //将结点to也加入顶点表中
		new_to -> firstedge = NULL;
		new_to -> nextNode = NULL;
		new_to -> vertex = to;

		//head -> nextNode -> nextNode = NULL; 
		Node *tail = new Node;
		tail -> vertex = -1;
		tail -> firstedge = NULL;
		tail -> nextNode = NULL;
		//head -> nextNode -> nextNode = tail;

		new_to ->nextNode = tail;
		head -> nextNode = new_to;
	}

	else
	{
	    while (h -> vertex  != -1)
	    {
		    if (h -> vertex == from)    //当前顶点表中已经存在from结点
		    {
			    adjNode *first = h -> firstedge;  //在边表的头结点插入新的边表结点
			    adjNode *newnode = new adjNode;
			    newnode -> adjvertex = to;
			    newnode -> weight = weight;
			    newnode -> next = first;
			    h -> firstedge = newnode;
			    break;
		    }
		    temp = h;   //temp指向当前顶点表中最后一个顶点
		    h = h -> nextNode;
	    }
	
	    if (h -> vertex == -1)   //当前顶点表中不存在from结点
	    {
		    Node *newnode = new Node;
		
		    adjNode *adj = new adjNode;
		    adj -> adjvertex = to;
		    adj -> next = NULL;
		    adj -> weight = weight;

		    newnode -> firstedge = adj;
		    newnode -> nextNode = NULL;
		    newnode -> vertex = from;
		    temp -> nextNode = newnode;

		    Node *tail = new Node;
		    tail -> vertex = -1;
		    tail -> firstedge = NULL;
		    tail -> nextNode = NULL;
		    temp -> nextNode -> nextNode = tail;
	    }
	}

	h = head;
	while (h -> vertex != -1)
	{
		if (h -> vertex == to)
			return;
		else
			temp = h;
			h = h -> nextNode;
	}
	if (h -> vertex == -1)   //当前顶点表中不存在to结点
	{
		Node *newnode = new Node;
		newnode -> firstedge = NULL;
		newnode -> nextNode = NULL;
		newnode -> vertex = to;
		temp -> nextNode = newnode;

		Node *tail = new Node;
		tail -> vertex = -1;
		tail -> firstedge = NULL;
		tail -> nextNode = NULL;
		temp -> nextNode -> nextNode = tail;
	}
}

void GraphLinkedTable::RemoveNode (int node)  //从图中删除结点node
{ 
	
	if (head -> vertex == node)     //node为顶点表中第一个顶点
		head = head -> nextNode;
	Node *temp = head;
	Node *p = head;   //指向要删除顶点的上一个顶点
    while (temp -> vertex != -1)
	{
		if (temp -> vertex == node)    //从顶点表中删除node
		{
			p -> nextNode = temp -> nextNode;
		}
		else   //从边表结点中删除node
		{
			adjNode * adjnode = temp -> firstedge;
			adjNode * q = temp -> firstedge;
			if (adjnode -> adjvertex == node)
				temp -> firstedge = adjnode -> next;
			else
			{
				while (adjnode != NULL)
				{
					if (adjnode -> adjvertex == node)
					{
						q -> next = adjnode -> next;
						break;
					}
					q = adjnode;
					adjnode = adjnode -> next;
				}
			}
		}
		p = temp;
		temp = temp -> nextNode;
    }	
	for (int i = getIndex(node); i < VertexCount - 1; i ++)
	{
		Vertex[i] = Vertex[i + 1];
	}
	VertexCount = VertexCount - 1;
    V --;
}

void GraphLinkedTable::RemoveEdge (int from, int to)
{
	Node * temp = head;
	while (temp -> vertex != -1)
	{
		if (temp -> vertex == from)   //从顶点表中查找结点from
		{
			
			if (temp -> firstedge -> adjvertex == to)  //to是边表from中的第一个顶点
			{
				temp -> firstedge = temp -> firstedge -> next;
				break;
			}
			else
			{
				adjNode *adjnode = temp -> firstedge;
				adjNode *last = temp -> firstedge;
				while (adjnode != NULL)
				{
					if (adjnode -> adjvertex == to)
					{
					    last -> next = adjnode ->next;
						break;
					}
					last = adjnode;
					adjnode = adjnode -> next;
				}
			}
			break;
		}
		temp = temp -> nextNode;
	}
}

void GraphLinkedTable::BuildGraph()     //构建一个图
{
	//setVertexCount(V);

	InsertEdge(0, 1, 1);
	InsertEdge(0, 3, 1);
	InsertEdge(0, 4, 1);
	InsertEdge(1, 0, 1);
	InsertEdge(1, 2, 1);
	InsertEdge(1, 3, 1);
	InsertEdge(1, 4, 1);
	InsertEdge(2, 5, 1);
	InsertEdge(2, 6, 1);
	InsertEdge(2, 8, 1);
	InsertEdge(3, 0, 1);
	InsertEdge(3, 1, 1);
	InsertEdge(3, 4, 1);
	InsertEdge(3, 5, 1);
	InsertEdge(4, 2, 1);
	InsertEdge(4, 5, 1);
	InsertEdge(5, 1, 1);
	InsertEdge(5, 2, 1);
	InsertEdge(5, 8, 1);
	InsertEdge(6, 2, 1);
	InsertEdge(6, 5, 1);
	InsertEdge(7, 5, 1);
	InsertEdge(7, 8, 1);
	InsertEdge(8, 2, 1);
	InsertEdge(8, 5, 1);
	InsertEdge(8, 7, 1);

	//InsertEdge(0,1,1);
	////InsertEdge(0,2,1);
	////InsertEdge(1,0,1);
	//InsertEdge(2,1,1);
}

void GraphLinkedTable::Print()
{
	Node *h = head;
	while (h -> vertex != -1)
	{
		cout << h -> vertex << " ---> ";
		adjNode *edge = h -> firstedge;
		while (edge != NULL)
		{ 
			//cout << edge -> weight << "  ";
			cout << edge -> adjvertex << ", ";
			edge = edge -> next;
		}
		cout<<endl;
		h = h -> nextNode;
	}
}

int GraphLinkedTable::getIndex(int node)
{
	for (int i = 0; i < VertexCount; i++)
	{
		if (Vertex[i] == node)
			return i;
	}
	return -1;
}

int GraphLinkedTable::getConnectedVertices ()   //获得图中所有度不为零的顶点数
{
	int *tally = new int [VertexCount];
	for (int i = 0; i < VertexCount; i++)
		tally[i] = 0;
	Node *h = head;
	while (h -> vertex != -1)
	{
		
		adjNode *adj = h -> firstedge;
		while (adj != NULL)
		{
			tally[getIndex(h -> vertex)] ++;
			tally[getIndex(adj -> adjvertex)] ++;
			adj = adj -> next;
		}
		h = h -> nextNode;
	}

	int connected = 0;
	for (int i =0; i < VertexCount; i++)
	{
		if (tally[i] > 0)
			connected ++;
        //cout<<tally[i]<<" ";
	}
	return connected;
}

void GraphLinkedTable::DFS(int node, bool *visited, float *distance)
{
	visited[getIndex(node)] = true;
	//cout<<node<<" ";
	Node * h = head;
	while ( h -> vertex != -1)
	{
		if (h -> vertex == node)
		{
			adjNode *adj = h -> firstedge;
			while (adj != NULL)
			{
				if (visited[getIndex(adj->adjvertex)] == false)
				{
					//des.push_back(adj -> adjvertex);
					distance[getIndex(adj -> adjvertex)] = adj -> weight + distance[getIndex(node)];

					DFS(adj->adjvertex, visited, distance);//,des);					
				}
				adj = adj -> next;
			}
		}
		h = h -> nextNode;
	}
}

bool GraphLinkedTable::IsDescendant(int from, int to)  //判断to是否为from的后裔结点
{
	bool * visited = new bool[VertexCount];
	for (int i = 0; i < VertexCount; i++)
	{
		visited[i] = false;
	}
	float *distance = new float[VertexCount];
	for (int i = 0; i < VertexCount; i++)
		distance[i] = 0;
	DFS(from, visited, distance);
	if (getIndex(to)==-1)
		return false;
	if (visited[getIndex(to)] == true)
		return true;
	else
		return false;
}

void GraphLinkedTable::ChangeWeight(int from, int to)
{
	Node *h = head;
	while (h ->vertex != -1)
	{
		if (h -> vertex == from)
		{
		    adjNode *adj = h -> firstedge;
		    while(adj != NULL)
		    {
				if (adj -> adjvertex == to)
				{
					adj -> weight = adj -> weight * 2;
				}
			    adj = adj -> next;
		    }
		}
		h = h ->nextNode;
	}	
}

float GraphLinkedTable::SumDistance(int node)
{
	float sum = 0;
	bool * visited = new bool [VertexCount];
	for (int i = 0; i < VertexCount; i++)
		visited [i] = false;
	float *distance  = new float [VertexCount];
	for (int i = 0; i < VertexCount; i++)
		distance [i] = 0;
	DFS(node, visited, distance);
	for (int i = 0; i < VertexCount; i++)
		sum += distance[i];
	return sum;
}

float GraphLinkedTable::AvgSumDistance(int node)
{
	float sum = SumDistance(node);
	return sum / Nspanningtrees;
}

float GraphLinkedTable::Eccentricity(int node)
{
	bool * visited = new bool [VertexCount];
	for (int i = 0; i < VertexCount; i++)
		visited [i] = false;
	float *distance  = new float [VertexCount];
	for (int i = 0; i < VertexCount; i++)
		distance [i] = 0;
	DFS(node, visited, distance);
	float ecc = distance[0];
	for (int i = 0; i < VertexCount; i++)
	{
		if (ecc < distance[i])
			ecc = distance [i];
	}
	return ecc;
}

//float GraphLinkedTable::AvgEcc(int node)
//{
//	float ecc = Eccentricity(node);
//
//}

float *GraphLinkedTable::Vector()
{
	float *Dv = new float[VertexCount];
	for (int i = 0; i < VertexCount; i++)
		Dv[i] = AvgSumDistance(Vertex[i]);
	return Dv;
}

//float GraphLinkedTable::NetworkDistance(GraphLinkedTable G, int vertexnum)
//{
//	float *Dv1 = Vector();
//}

void GraphLinkedTable::Find()
{
	float *factor = new float[VertexCount];
	for (int i = 0; i < VertexCount;i++)
	{

		//factor 
	}
}

list<GraphLinkedTable> SpanningTrees;

int n = 0;

void GROW(GraphLinkedTable G, GraphLinkedTable T,GraphLinkedTable *L, list<Edge> F)
{
	int TnSpannedVertex = T.getConnectedVertices();
	if (TnSpannedVertex == V)
	{	
		SpanningTrees.push_front(T);
		*L = SpanningTrees.front();
		/*cout<<"---------spanning tree "<<n<<"-----------"<<endl;
	    L->Print ();*/
	    //cout<<"------------------------------------------"<<endl;
		n ++;
		G.Nspanningtrees ++;
	}
	else
	{
		list<Edge>FF;
		int b = 1;
		do
		{
			int size = F.size();
			if (size == 0)
				break;
			Edge e = F.front();
			F.pop_front();

			int u = e.fromNode;
			int v = e.toNode;
			T.InsertEdge(u, v, e.weight);

			list<Edge> Fcopy(F);

			//push each edge(v,w) in G, where w does not belong to T, onto F
			Node *h = G.head;
			while (h -> vertex != -1)
			{
				if (h -> vertex == v)
				{
					adjNode *adj = h -> firstedge;
					while (adj != NULL)
					{
						if (T.ownVertex (adj -> adjvertex) == false)
						{
							Edge edge;
							edge.fromNode = v;
							edge.toNode = adj -> adjvertex;
							edge.weight = adj -> weight;
							F.push_front(edge);
						}
						adj = adj -> next;
					}
				}
				h = h -> nextNode;
			}

			//remove each edge (w,v) where w belongs to T, from F
			h = G.head;
			while (h -> vertex != -1)
			{			
				if (T.ownVertex(h->vertex) == true)
				{
					adjNode * adj = h -> firstedge;
				    while (adj != NULL)
				    {
					    if ( adj -> adjvertex == v)
					    {
						    Edge edge;
						    edge.fromNode = h -> vertex;
						    edge.toNode = v;
						    edge.weight = adj -> weight;
						    F.remove(edge);
					    }
					    adj = adj -> next;
				    }
				}
				h = h -> nextNode;
			}

			GROW(G, T, L, F);

			F = Fcopy;  //restore F
			T.RemoveEdge(u, v);
			G.RemoveEdge(u, v);
			FF.push_front(e);

			b = 1;
			h = G.head;
			while (h -> vertex != -1)
			{
				adjNode *adj = h -> firstedge;
				while(adj != NULL)
				{
					/*cout<<"v="<<v<<endl; 
					L->Print();
					cout<<(L ->head->vertex== -1)<<endl;*/
					if (adj -> adjvertex == v)
					{
						//L->Print();
						if (L ->head->vertex== -1)
							break;
						if (L->IsDescendant (v, h->vertex) == false)  //w is not a descendant of v
						{
							b = 0;
							break;
						}
					}
					adj = adj -> next;
				}
				h = h -> nextNode;
			}
		}
		while (!b);

		//pop each edge e from FF, push e onto F, and add e to G
		while(!FF.empty())
		{
			Edge e = FF.front();
			FF.pop_front();
			F.push_front(e);
			G.InsertEdge(e.fromNode, e.toNode, e.weight);
		}
	}
}

void S(GraphLinkedTable G)
{
	//cout<<"V="<<V<<endl;
    GraphLinkedTable T(V);
	GraphLinkedTable *L = new GraphLinkedTable;

    int nspanningtrees;
    
	for (int i = 0;i < G.VertexCount; i++)
	{
		list <Edge> F;
		int rootvertex = G.Vertex[i];
		Node * h = G.head;
		while(h -> vertex != -1)
		{
			if (h -> vertex == rootvertex)
			{
				adjNode * adj = h -> firstedge;
				while (adj != NULL)
				{   
					Edge e;
					e.fromNode = h -> vertex;
					e.toNode = adj -> adjvertex;
					e.weight = adj -> weight;
					F.push_front(e);
					adj = adj -> next;
				}
			}
			h = h -> nextNode;
		}
		GROW(G, T, L, F);
    }
	cout<<n<<endl;
	//cout<<SpanningTrees.size()<<endl;
}

void main()
{
	clock_t start,finish;
	double totaltime;
	start = clock();

	GraphLinkedTable Graph(V);	
	Graph.Print();
	Graph.BuildGraph();
	cout<<"---------------Graph---------------"<<endl;
	Graph.Print ();
	cout<<"-----------------------------------"<<endl;

	/*Graph.RemoveNode(8);
	Graph.RemoveNode(5);*/
	//Graph.Print();
	S(Graph);	

	finish = clock();
	totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
	cout<<"\n此程序运行的时间为"<<totaltime<<"秒!"<<endl;
}