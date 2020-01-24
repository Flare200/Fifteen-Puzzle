//Elijah Ratner, CS 482, HW 2 

#include <iostream>
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <string>
#include "stlastar.h"//Library used for handling the nodes in the program

#define GET_LISTS 1   //Used for when I want to display the open and closed lists on each
                      //iteration, but also for getting the number of nodes on each list

#define GET_SOLUTION 1//Used for when I want to display the solution path

using namespace std;

class puzzleNode
  {
  public:
    static int start[16];//Static variable for start state
    int state[16];//Current state of the node
    static int goal[16];//Static variable of the goal state
    string solPath;//String of characters for each move of the solution path
    int pathMove;//The cost of the moves up to the current state

    puzzleNode()//Default Constructor
    {
      for(int i=0; i<16; i++)
	      state[i]=start[i];

      solPath="";
      pathMove=0;
      }

    puzzleNode(int curstate[], string path, int move)
    {
      for(int i=0; i<16; i++)
	      state[i]=curstate[i];

      solPath=path;
      pathMove=move;
    }

    /*These functions were required for the header "stlastar.h" to work properly.  The names
      and params could not be changed as they are for a general A star program, but the 
      functions are my own.  Since we are doing the 15 puzzle, these functions work 
      corresponding to the given assignment.
    */
    int GoalDistanceEstimate(puzzleNode &goal);//Calculates the heuristic
    bool IsSameState(puzzleNode &rhs);//Checks if one node is similar to another
    bool IsGoal(puzzleNode &goal);//Checks if the current node is the goal state
    bool GetSuccessors(AStarSearch<puzzleNode> *astarsearch, puzzleNode *parent);//Gets all
    //possible successors that can be generated from the current node
    int GetCost(puzzleNode &successor);//Returns the cost to move to the next node

    
    /*These functions were not required for the "stlastar.h" library.  These only served to 
    print the current state of each node, and act as operators for each move.
    */
    void PrintNodeInfo();
    bool moveUp(int state[], int cstate[], string path, string &cpath, int move, int &cmove);
    bool moveDown(int state[], int cstate[], string path, string &cpath, int move, int &cmove);
    bool moveLeft(int state[], int cstate[], string path, string &cpath, int move, int &cmove);
    bool moveRight(int state[], int cstate[], string path, string &cpath, int move, int &cmove);
 
  };

int puzzleNode::start[16]={1,2,0,3,6,7,11,4,5,9,12,8,13,10,14,15};//1
int puzzleNode::goal[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};

int puzzleNode::GoalDistanceEstimate(puzzleNode &goal)
{
  int h=0,num,dest,row1,col1,row2,col2;
  for(int i=0;i<16;i++)
    {
      if(state[i]==0)//If blank tile
	      continue;

      num=state[i];//actual number of the tile 
      dest=num-1;  //index of the destination for the tile      

      row1=i/4;//row and col for actual tile place   
      col1=i%4;

      row2=dest/4;//row and col of tile dest
      col2=dest%4;

      if(row1!=row2)//Tile not in correct row
        h+=abs(row1-row2);

      if(col1!=col2)//Tile not in correct col
        h+=abs(col1-col2);
      
      if(row1==row2)//Tile is in correct row
	{
	  for(int j=(i/4)*4; j<i; j++)//Starting from the leftmost tile in row
	    {
        if(i%4==0)
          break; //If leftmost tile

        if(state[j]==0 || ((state[j]-1)/4!=(state[i]-1)/4))
          continue; //If blank tile, or other tile not in correct row

        if(state[j]>state[i]) //Linear conflict in row
          h+=2;
      }
	}

      if(col1==col2)//Tile is in correct column      
        {
          for(int j=i/4; j<i; j++)//Starting from the uppermost tile in col 
            {
              if(i<4)
                break; //If uppermost tile

              if(j%4!=i%4 || state[j]==0 || ((state[j]-1)%4!=(state[i]-1)%4))
                continue; //If blank tile, wrong col, or other tile not in correct col

              if(state[j]>state[i]) //Linear conflict in col
                h+=2;
            }
        }

    }

  return h*10;//Adds weight to the heuristic to decrease time to find path
}

bool puzzleNode::IsSameState(puzzleNode &rhs)//Returns true if all tiles on lhs match all tiles
{                                            //on rhs
  for(int i=0; i<16; i++)
    if(state[i]!=rhs.state[i])
      return false;
  return true;
}

bool puzzleNode::IsGoal(puzzleNode &goal)//Returns true if goal state
{
  return IsSameState(goal);
}

bool puzzleNode::GetSuccessors(AStarSearch<puzzleNode> *astarsearch, puzzleNode *parent)
{
  puzzleNode newNode(state, solPath, pathMove);//Node based on current state

  bool ret;

  if(moveUp(state,newNode.state,solPath,newNode.solPath,pathMove,newNode.pathMove))
    {
      ret=astarsearch->AddSuccessor(newNode);//Add successor if up is possible
      newNode.solPath=solPath;

      if(!ret)
	      return false;
    }

  if(moveDown(state,newNode.state,solPath,newNode.solPath,pathMove,newNode.pathMove))
    {
      ret=astarsearch->AddSuccessor(newNode);//Add successor if down is possible
      newNode.solPath=solPath;

      if(!ret)
	      return false;
    }

  if(moveLeft(state,newNode.state,solPath,newNode.solPath,pathMove,newNode.pathMove))
    {
      ret=astarsearch->AddSuccessor(newNode);//Add successor if left is possible
      newNode.solPath=solPath;

      if(!ret)
	      return false;
    }
  
  if(moveRight(state,newNode.state,solPath,newNode.solPath,pathMove,newNode.pathMove))
    {
      ret=astarsearch->AddSuccessor(newNode);//Add successor if right is possible
      newNode.solPath=solPath;

      if(!ret)
	      return false;
    }

  return true;
  
}

int puzzleNode::GetCost(puzzleNode &successor)//The cost to the next node is always one
{
  return 1;
}

void puzzleNode::PrintNodeInfo()
{
  if(pathMove==0)//If initial state
    {}
  else//Print move
    {
      if(solPath[pathMove-1]=='U')
	      cout << setw(10) << "Up" << endl << endl;
      else if(solPath[pathMove-1]=='D')
	      cout << setw(11) << "Down" << endl << endl;
      else if(solPath[pathMove-1]=='L')
	      cout << setw(11) << "Left" << endl << endl;
      else if(solPath[pathMove-1]=='R')
	      cout << setw(11) << "Right" << endl << endl;
    }

  for(int i=0; i<4; i++)//Print state of tiles
    {
      for(int j=0; j<4; j++)
        cout << setw(4) << state[j+(i*4)];
      cout << endl;
    }
}


bool puzzleNode::moveUp(int state[], int cstate[], string path, string &cpath, int move, int &cmove)
{
  cpath=path;
  cmove=move;
  cmove++;

  for(int i=0; i<16; i++)
    cstate[i]=state[i];

  for(int i=0; i<16; i++)
    {
      if(cstate[i]==0)
        {
          if(i<=3)
            return false;//Return false if in first row
          else
            {
              int temp=cstate[i];
              cstate[i]=cstate[i-4];
              cstate[i-4]=temp;

	            cpath=cpath+"U";//Add U to solution path
              return true;
            }
        }
    }
}

bool puzzleNode::moveDown(int state[], int cstate[], string path, string &cpath, int move, int &cmove)
{
  cpath=path;
  cmove=move;
  cmove++;

  for(int i=0; i<16; i++)
    cstate[i]=state[i];

  for(int i=0; i<16; i++)
    {
      if(cstate[i]==0)
        {
          if(i>=12)
            return false;//Return false if in last row 
          else
            {
              int temp=cstate[i];
              cstate[i]=cstate[i+4];
              cstate[i+4]=temp;

	            cpath=cpath+"D";//Add D to solution path
              return true;
            }
        }
    }
}

bool puzzleNode::moveLeft(int state[], int cstate[], string path, string &cpath, int move, int &cmove)
{
  cpath=path;
  cmove=move;
  cmove++;

  for(int i=0; i<16; i++)
    cstate[i]=state[i];

  for(int i=0; i<16; i++)
    {
      if(cstate[i]==0)
        {
          if(i%4==0)
            return false;//Return false if in leftmost column
          else
            {
              int temp=cstate[i];
              cstate[i]=cstate[i-1];
              cstate[i-1]=temp;

	            cpath=cpath+"L";//Add L to solution path
              return true;
            }
        }
    }
}

bool puzzleNode::moveRight(int state[], int cstate[], string path, string &cpath, int move, int &cmove)
{
  cpath=path;
  cmove=move;
  cmove++;

  for(int i=0; i<16; i++)
    cstate[i]=state[i];

  for(int i=0; i<16; i++)
    {
      if(cstate[i]==0)
        {
          if((i+1)%4==0)//Return false if in rightmost column
            return false;
          else
            {
              int temp=cstate[i];
              cstate[i]=cstate[i+1];
              cstate[i+1]=temp;

	            cpath=cpath+"R";//Add R to solution path
              return true;
            }
        }
    }
}

//All three initial states, plus the goal state
int start[16]={1,2,0,3,6,7,11,4,5,9,12,8,13,10,14,15};//1
//int start[16]={1,2,3,4,12,13,14,5,11,0,15,6,10,9,8,7};//2
//int start[16]={0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};//3
int goal[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};



int main()
{
  AStarSearch<puzzleNode> astarsearch(1000000);//The template class to control the nodes
  int searchCount=0, numSearches=1;

  while(searchCount<numSearches)
    {
      puzzleNode nodeStart(start,"",0);//Start node
      puzzleNode nodeEnd(goal,"",0);//Goal node

      astarsearch.SetStartAndGoalStates(nodeStart,nodeEnd);//Creates the start and goal states
      
      unsigned int searchState, searchSteps=0;//Search state determines if astarseach is either:
      //searching, succeeeded, failed, or out of memory; search steps is the # of iterations
      float f,g,h;//g=cost, h=heuristic, f=g+h
      int oN=0, cN=0;//oN=open nodes; cN=closed nodes

      do {
	searchState=astarsearch.SearchStep();//Goes through an iteration

#if GET_LISTS 
	//cout << "-------------------------------------\nSearch Step: " << searchSteps << endl;

	//cout << "Open List:\n";
	puzzleNode *p=astarsearch.GetOpenListStart(f,g,h);//Beginning of open list
	int openNodes=0, closedNodes=0;
	
	while(p)
	  {
	    //((puzzleNode *)p)->PrintNodeInfo();
	    //cout << "f=" << f << ", g=" << g << ", h=" << h << endl << endl;
	    
	    p=astarsearch.GetOpenListNext(f,g,h);//Next on open list
	    openNodes++;
	  }
	//cout << "# of Open List Nodes: " << openNodes << endl;

	if(openNodes!=0)
	  oN=openNodes;

	//cout << "--------\nClosed List:\n";
	p=astarsearch.GetClosedListStart(f,g,h);//Beginning of closed list
	while(p)
	  {
	    //p->PrintNodeInfo();
	    //cout << "f=" << f << ", g="<< g <<", h=" << h << endl << endl;

            p=astarsearch.GetClosedListNext(f,g,h);//Next on closed list
	    closedNodes++;
	  }
	//cout <<"# of Closed List Nodes: " << closedNodes << endl;
	
	if(closedNodes!=0)
	  cN=closedNodes;

	searchSteps++;
	
#endif

      }while(searchState==AStarSearch<puzzleNode>::SEARCH_STATE_SEARCHING);//Goal state not found yet

      
      if(searchState==AStarSearch<puzzleNode>::SEARCH_STATE_SUCCEEDED)//Goal state found
	{
#if GET_SOLUTION

	  cout << "Displaying Solution:\n";
	  
	  puzzleNode *node=astarsearch.GetSolutionStart();//Start of solution list
	  int steps=0;
	  
	  node->PrintNodeInfo();
	  cout << endl;

	  while(true)
	    {
	      node=astarsearch.GetSolutionNext();//Next on solution list
	      
	      if(!node)
		break;

	      node->PrintNodeInfo();
	      cout << endl;
	      
	      steps++;
	    }
	  cout << "Length of Solution Path: " << steps << endl;
	  cout << "# of Open Nodes: " << oN << endl;
	  cout << "# of Closed Nodes: " << cN << endl;
	  cout << "# of Total Nodes: " << oN+cN << endl;
#endif

	}
      else if(searchState==AStarSearch<puzzleNode>::SEARCH_STATE_FAILED)//Goal state not reached
	      cout << "Search Failed" << endl;
      else if(searchState==AStarSearch<puzzleNode>::SEARCH_STATE_OUT_OF_MEMORY)//Ran through max nodes
	      cout <<"Out of Memory"<< endl;

      //cout << "Search Steps: " << searchSteps << endl;

      searchCount++;
      }
  
  return 0;
}
