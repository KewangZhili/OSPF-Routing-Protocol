#include <iostream>
#include <vector>
#include <set>
#include<queue>
#include <unordered_map>
using namespace std;
class router{
    public:
    int router_id;
    vector<pair<router*,int> > neighbours;
    unordered_map<int,int > routing_table;
    unordered_map<int, int> findist; // to store distances to all other nodes which are reachable
    void add_neighbour(router* new_neighbour,int weight) {neighbours.push_back(make_pair(new_neighbour,weight));}
    void update_routing_table()
    {
        unordered_map<router *, int> dist;    // to store distances to all other nodes which are reachable
        unordered_map<router *, router *> parent;//to store previous nodes in shortest path
        parent[this]=NULL;
        queue<router*>q;//used to find all reachable nodes in bfs manner
        for(int i=0;i<neighbours.size();i++)
        {
            dist[neighbours[i].first]=INT_MAX;
            q.push(neighbours[i].first);
            parent[neighbours[i].first]=this;
        }
        dist[this]=0;
        while(!q.empty())
        {
            router* current=q.front();
            q.pop();
            int p = current->neighbours.size();
            for (int i = 0; i < p; i++)
            {
                router *current_neighbour = current->neighbours[i].first;
                if (dist.find(current_neighbour) == dist.end())
                    {
                        dist[current_neighbour] = INT_MAX;
                        q.push(current_neighbour);
                    }
            }
        }
        int n=dist.size();
        //priority queue to decrease time complexity fron O(n^2) to O(nlogn)
        priority_queue<pair<int,router*>,vector<pair<int,router* > >,greater<pair<int,router*> > >pq;
        pq.push(make_pair(0,this));
        while(!pq.empty())
        {
            router* pqfront=pq.top().second;
            int dis=pq.top().first;
            pq.pop();
            for(auto it:pqfront->neighbours)
            {
                router* v=it.first;
                int w=it.second;
                if(dist.find(v)!=dist.end()&&dis+w<dist[v])
                {
                    dist[v]=dis+w;
                    pq.push(make_pair(dis+w,v));
                    parent[v]=pqfront;
                }
            }
        }
        //cout<<"hello"<<endl;
        //to find next hop for each of the nodes
        for(auto it:dist)
        {
            router* curr=it.first;
            if(curr->router_id!=this->router_id)
            {
                int present=curr->router_id;
                routing_table[present]=present;
                //cout<<present<<endl;
                int temp_id=parent[curr]->router_id;
                //cout<<"temp:"<<temp_id<<endl;
                router* parent_r=parent[curr];
                //cout<<"it.second:"<<it.second<<endl;
                while(temp_id!=this->router_id)
                {
                    routing_table[present]=temp_id;
                    parent_r=parent[parent_r];
                    //cout<<"parent_r->router_id:"<<parent[curr]->router_id<<endl;
                    temp_id=parent_r->router_id;
                    //cout<<"temp2:"<<temp_id<<endl;
                }
            }
            findist[it.first->router_id]=it.second;
        }
        //cout<<"hello2"<<endl;
    }
    void print_routing_table()
    {
        cout<<"-----------------------------------------------------------\n";
        cout<<"            routing table for router with id = "<<this->router_id<<endl;
        for(auto it:routing_table)
        {
            cout<<"destination:"<<it.first<<"---";
            cout<<" "<<"next hop:"<<it.second;
            auto it2=it.first;
            cout<<" ---"<<"distance to this node(destination) is:"<<findist[it2];
            cout<<endl;
        }
        cout<<"           all nodes which are not reachable are not printed,So,for any node not present here,it represents that node is not reachable"<<endl;
        cout << "-----------------------------------------------------------\n";
    }
};
//used to print the shortest paths between given two nodes
void simulatepath(unordered_map<int , router*> &idtorouter)
{
    int srcid;
    int dstid;
    for(int i=0;i<1;i++)
    {
    cout<<"Enter Source id:";
    cin>>srcid;
    if (idtorouter.find(srcid) == idtorouter.end())
    {
        cout << "Entered id is not found! Please enter again" << endl;
        i--;
        continue;
    }
    cout<<"Enter Destination id:";
    cin>>dstid;
    if (idtorouter.find(dstid) == idtorouter.end())
    {
        cout << "Entered id is not found! Please enter both node ids again" << endl;
        i--;
        continue;
    }
    }
    router* src=idtorouter[srcid];
    if(src->routing_table.find(dstid)==src->routing_table.end())
    {
        cout<<"No path exists between entered nodes"<<endl;
        return;
    }
    cout<<"Path :";
    src->update_routing_table();
    //src->print_routing_table();
    cout<<srcid<<" ";
    while(src->routing_table[dstid]!=dstid)
    {
        cout<<src->routing_table[dstid]<<" ";
        src=idtorouter[src->routing_table[dstid]];
        src->update_routing_table();
    }
    cout<<dstid<<endl;
}
void print_all_routing_tables(unordered_map<int, router*>&idtorouter)//printing all routing tables
{
    for(auto it:idtorouter)
    {
        it.second->update_routing_table();
        it.second->print_routing_table();
    }
}
bool isValid(int srcid,int dstid,unordered_map<int,router*>&idtorouter)
{
    router* src=idtorouter[srcid];
    for(auto it:src->neighbours)
    {
        if(it.first->router_id==dstid) return false;
    }
    return true;
}
int main()
{
    // router router1;
    // router router2;
    // router router3;
    // router1.router_id=1;
    // router2.router_id=2;
    // router3.router_id=3;
    // router* r1=&router1;
    // router* r2=&router2;
    // router* r3=&router3;
    // r1->neighbours.push_back(make_pair(r2,1));
    // r2->neighbours.push_back(make_pair(r1,1));
    // r2->neighbours.push_back(make_pair(r3,1));
    // r3->neighbours.push_back(make_pair(r2,1));
    // r1->update_routing_table();
    // r2->update_routing_table();
    // r3->update_routing_table();
    // r1->print_routing_table();
    // r2->print_routing_table();
    // r3->print_routing_table();
    int n,m;
    cout<<"Enter the number of nodes:";
    cin>>n;
    cout<<"Enter only unique ids"<<endl;
    unordered_map<int , router*> idtorouter;
    for(int i=0;i<n;i++)
    {
        int id;
        cout<<"Enter router_id:";
        cin>>id;
        router* pr=new router();
        if(idtorouter.find(id)!=idtorouter.end())
        {
            cout<<"You entered same id number again! Please enter unique id"<<endl;
            i--;
            continue;
        }
        pr->router_id=id;
        idtorouter[id]=pr;
        //cout<<idtorouter[id]->router_id<<endl;
    }
    //cout << idtorouter[1]->router_id << endl;
    cout<<"Enter number of edges:";
    cin>>m;
    cout<<"Enter two node ids and weight between them"<<endl;
    for(int i=0;i<m;i++)
    {
        int srcid,dstid,weight;
        cout<<"Node id:";
        cin>>srcid;
        if(idtorouter.find(srcid)==idtorouter.end())
        {
            cout<<"Entered id is not found! Please enter again"<<endl;
            i--;
            continue;
        }
        cout<<"Node id:";
        cin>>dstid;
        if(idtorouter.find(dstid)==idtorouter.end())
        {
            cout<<"Entered id is not found! Please enter both node ids again"<<endl;
            i--;
            continue;
        }
        if(srcid==dstid)
        {
            cout<<"Self loop is not allowed in networking entities,So,Please enter valid neighbouring nodes"<<endl;
            i--;
            continue;
        }
        if (!isValid(srcid, dstid, idtorouter))
        {
            cout << "They already have an edge!" << endl;
            i--;
            continue;
        }
        cout<<"Weight:";
        cin>>weight;
        router* src;
        router* dst;
        src=idtorouter[srcid];
        dst=idtorouter[dstid];
        src->add_neighbour(dst,weight);
        dst->add_neighbour(src,weight);
    }
    //cout<<idtorouter[1]->router_id<<endl;
    simulatepath(idtorouter);
    print_all_routing_tables(idtorouter);
    char cond;
    while(true)
    {
    cout<<"Do you want to add more nodes and edges(y/n)?"<<endl;
    cin>>cond;
    if(cond=='y')
    {
            int n, m;
            cout << "Enter the number of new nodes:";
            cin >> n;
            cout << "Enter only unique ids" << endl;
            for (int i = 0; i < n; i++)
            {
                int id;
                cout << "Enter router_id:";
                cin >> id;
                router *pr = new router();
                if (idtorouter.find(id) != idtorouter.end())
                {
                    cout << "You entered same id number again! Please enter unique id" << endl;
                    i--;
                    continue;
                }
                pr->router_id = id;
                idtorouter[id] = pr;
                // cout<<idtorouter[id]->router_id<<endl;
            }
            // cout << idtorouter[1]->router_id << endl;
            cout << "Enter number of edges:";
            cin >> m;
            cout << "Enter two node ids and weight between them" << endl;
            for (int i = 0; i < m; i++)
            {
                int srcid, dstid, weight;
                cout << "Node id:";
                cin >> srcid;
                if (idtorouter.find(srcid) == idtorouter.end())
                {
                    cout << "Entered id is not found! Please enter again" << endl;
                    i--;
                    continue;
                }
                cout << "Node id:";
                cin >> dstid;
                if (idtorouter.find(dstid) == idtorouter.end())
                {
                    cout << "Entered id is not found! Please enter both node ids again" << endl;
                    i--;
                    continue;
                }
                if (srcid == dstid)
                {
                    cout << "Self loop is not allowed in networking entities,So,Please enter valid neighbouring nodes" << endl;
                    i--;
                    continue;
                }
                if(!isValid(srcid,dstid,idtorouter))
                {
                    cout<<"They already have an edge!"<<endl;
                    i--;
                    continue;
                }
                cout << "Weight:";
                cin >> weight;
                router *src;
                router *dst;
                src = idtorouter[srcid];
                dst = idtorouter[dstid];
                src->add_neighbour(dst, weight);
                dst->add_neighbour(src, weight);
            }
        char cond2;
        cout<<"Do you want to simulate new path between two nodes and print all new routing tables(y/n)?"<<endl;
        cin>>cond2;
        if(cond2=='y')
        {
            simulatepath(idtorouter);
            print_all_routing_tables(idtorouter);
        }
        char cond3;
        cout<<"Do you want to end the program(y/n)?"<<endl;
        cin>>cond3;
        if(cond3=='y') break;
    }
    else
    {
        char cond4;
        cout<<"Do you want to end the program(y/n)?"<<endl;
        cin>>cond4;
        if(cond4=='y') break;
    }
    }
}
