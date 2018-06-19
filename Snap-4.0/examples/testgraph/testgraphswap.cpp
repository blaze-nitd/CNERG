#include "stdafx.h"
#include<bits/stdc++.h>
using namespace std;


int main(int argc, char* argv[]) {
  srand(time(0));
  if(argc<4)
  {
    cout<<"Error"<<"\n";
    exit(1);
   }
  /*PUNGraph G=TSnap::LoadEdgeList<PUNGraph>("as2.txt",0,1);
  for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
    printf("  %d: ", NI.GetId());
      printf(" %d", NI.GetDeg()); 
    printf("\n"); }
  for(TUNGraph::TEdgeI EI=G->BegEI(); EI<G->EndEI(); EI++){
       printf("%d------ %d", EI.GetSrcNId(),EI.GetDstNId());
       printf("\n"); }*/


  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Node Centrality. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  TExeTm ExeTm;


  Try
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", argv[1], "Input un/directed graph");
  string si(argv[1]);
   string ri(argv[3]);
   string qi(argv[2]);
  string y="node_centrality"+si.substr(0,si.find('.'))+qi+"swap"+ri+".tab";
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", y.c_str(), "Output file");

  printf("Loading %s...", InFNm.CStr());


  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  PUNGraph UGraph = TSnap::ConvertGraph<PUNGraph>(Graph);


  TIntFltH BtwH, CloseH;
  printf("nodes:%d  edges:%d\n", UGraph->GetNodes(), UGraph->GetEdges());

  int no_of_edges=Graph->GetEdges();
  //int rno=rand(no_of_edges);


  vector<pair<TInt, TInt> > Ea;
  //int coun=0;


  for(TUNGraph::TEdgeI EI=UGraph->BegEI(); EI<UGraph->EndEI(); EI++)
  {
     Ea.push_back(make_pair(EI.GetSrcNId(),EI.GetDstNId()));
     //cout<< EI.GetSrcNId()<<" "<<EI.GetDstNId()<<"\n";
  }


  vector<pair<int,int > > fin;
  TIntPrV EdgeV;
  TSnap::GetEdgeBridges(UGraph, EdgeV);

  for (int i = 0; i < EdgeV.Len(); i++) {
    fin.push_back(make_pair(EdgeV[i].Val1(), EdgeV[i].Val2()));}
  

  int z= atoi(argv[2]);
  int dp=z*no_of_edges*1.0/100;
  int no_of_try=4000;
  int cned=0;


  while(no_of_try--)
  {
    if(cned>=dp)
    break;
    vector<pair<TInt, TInt> > Ea;
  //int coun=0;
  for(TUNGraph::TEdgeI EI=UGraph->BegEI(); EI<UGraph->EndEI(); EI++)
  {
     Ea.push_back(make_pair(EI.GetSrcNId(),EI.GetDstNId()));
  }
    int rno1=rand()%(Ea.size());
    int rno2=rand()%(Ea.size());
//    int test = rand()%10;
//    cout<<rno1<<" "<<rno2<<" "<<test<<endl;
    int x=Ea[rno1].first.Val;
    int y=Ea[rno1].second.Val;

    int w =Ea[rno2].first.Val;
    int z=Ea[rno2].second.Val;
    

    /*UGraph->DelEdge(x,y);
    UGraph->DelEdge(y,x);
    UGraph->DelEdge(w,z);
    UGraph->DelEdge(z,w);*/
    int flag1=0,flag4=0;

    if(UGraph->AddEdge(x,z)==-2) flag1=1;
    //if(UGraph->AddEdge(z,x)==-2) flag1=1;
    if(UGraph->AddEdge(y,w)==-2) flag4=1;
    //if(UGraph->AddEdge(w,y)==-2) flag4=1;
     
    if(flag1==0&&flag4==0)
    {
      UGraph->DelEdge(x,y);
    //UGraph->DelEdge(y,x);
    UGraph->DelEdge(w,z);
    //UGraph->DelEdge(z,w);
    }
    else
    {
      if(flag1==0)
        UGraph->DelEdge(x,z);
      /*if(flag1==0)
        UGraph->DelEdge(z,x);*/
      else if(flag4==0)
        UGraph->DelEdge(y,w);
      /*if(flag4==0)
        UGraph->DelEdge(w,y);*/
    }


    
    
    TCnComV CnComV;
  // weakly connected components
    PNGraph UG = TSnap::ConvertGraph<PNGraph>(UGraph);
     TSnap::GetWccs(UG, CnComV);
    if(CnComV.Len()!=1)
    {
      if(flag1==0&&flag4==0)
      {
      
    UGraph->AddEdge(x,y);
    //UGraph->AddEdge(y,x);
    UGraph->AddEdge(w,z);
    //UGraph->AddEdge(z,w);

    //UGraph->DelEdge(z,x);
    UGraph->DelEdge(x,z);
    UGraph->DelEdge(y,w);
    //UGraph->DelEdge(w,y);
      }
    }
    else
    {
      cned++;
    }
   
  }

    printf("After Deletion\n");
    printf("nodes:%d  edges:%d\n", UGraph->GetNodes(), UGraph->GetEdges());
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  printf(" Betweenness (SLOW!)...");   TSnap::GetBetweennessCentr(UGraph, BtwH, 1);
  printf(" Closeness (SLOW!)...");
  for (TUNGraph::TNodeI NI = UGraph->BegNI(); NI < UGraph->EndNI(); NI++) {
    const int NId = NI.GetId();
    CloseH.AddDat(NId, TSnap::GetClosenessCentr<PUNGraph>(UGraph, NId, false));
  }
   printf("\nDONE! saving...");
  FILE *F = fopen(OutFNm.CStr(), "wt");
  /*fprintf(F,"#Network: %s\n", InFNm.CStr());
  fprintf(F,"#Nodes: %d\tEdges: %d\n", UGraph->GetNodes(), UGraph->GetEdges());
  fprintf(F,"#NodeId\tDegree\tCloseness\tBetweennes\n");*/
  for (TUNGraph::TNodeI NI = UGraph->BegNI(); NI < UGraph->EndNI(); NI++) {
    const int NId = NI.GetId();
    const double DegCentr = UGraph->GetNI(NId).GetDeg();
    const double CloCentr = CloseH.GetDat(NId);
    const double BtwCentr = BtwH.GetDat(NId);
     fprintf(F, "%d\t%f\t%f\t%f\n", NId, 
      DegCentr, CloCentr, BtwCentr); }
  fclose(F);
  Catch
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;}
