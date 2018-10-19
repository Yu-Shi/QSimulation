#ifndef CORE_H
#define CORE_H
/**************************************************************************
***
*** Copyright (c) 2018 Regents of the Tsinghua University,
***               Hailong Yao, and Weiqing Ji
***
***  Contact author(s): jwq18@mails.tinghua.edu.cn, hailongyao@mail.tsinghua.edu.cn
***  Original Affiliation:   EDA Lab., Department of Computer Science and Technology, Tsinghua University.
***
***  Permission is hereby granted, free of charge, to any person obtaining
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation
***  the rights to use, copy, modify, merge, publish, distribute, sublicense,
***  and/or sell copies of the Software, and to permit persons to whom the
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/


/*
*Created on 2018-8-27
*Author:Weiqing_Ji
*Version 1.3.1
*Title: 流速计算程序
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <queue>
#include <stack>
using namespace std;
#define NAX 0.000000001

struct edge
{
   double v; //流速
   double c; // con
   double leng; //管道长度
   int n1,n2; //边所连接的两个节点，默认从n1流向n2
}edges[200], edges2[200];
struct node
{
   vector<int> elist;  //点所连接的边，按照顺时针的顺序存储下来

}nodes[200], nodes2[200];

vector< vector<double> >rect;//存储方程组(行列式)

bool fr[200]; //深度搜索时判断边是否遍历过

int n,EDGESUM,NODESUM;  //记录网格边长

//函数功能：确定管道中某条管道的长度
//参数含义：x，管道的编号；leng，管道的长度
void setedgelength(int x, double leng)
{
   edges[x].leng = leng;
}

//函数功能：将方程加入到方程组中
//参数含义：tmp，新的方程
void addrect(vector<double> &tmp){
   rect.push_back(tmp);
}

//函数功能：计算节点(x,y)的dir方向的管道编号。dir为0表示上方，1表示右侧，2表示下方，3表示左侧。若管道存在(不论长度是否为0)则返回对应管道编号，不存在(即不在原始网格内)则返回EDGESUM
//参数含义：x，节点的横坐标；y节点的纵坐标；dir，询问的节点方向。
int getdirline(int x, int y, int dir){
   int e;
   int sum = x*n+y;
   if (dir == 0){
       if (y==0){
           if (edges[EDGESUM-5].n2 == sum)
               e = EDGESUM-5;
           else if (edges[EDGESUM-4].n2 == sum)
               e = EDGESUM-4;
           else
               e = EDGESUM;
       }
       else
           e=(x*(n-1)+y-1);
   }else if (dir == 1){
       if (x==n-1)
           e=EDGESUM;
       else
           e=n*n-n+x*n+y;
   }else if (dir == 2){
       if (y==n-1){
           if (edges[EDGESUM-3].n1 == sum)
               e = EDGESUM-3;
           else if (edges[EDGESUM-2].n1 == sum)
               e = EDGESUM-2;
           else if (edges[EDGESUM-1].n1 == sum)
               e = EDGESUM-1;
           else
               e = EDGESUM;
       }
       else
           e=(x*(n-1)+y);
   }else if (dir == 3){
       if (x==0)
           e=EDGESUM;
       else
           e=n*n-n+(x-1)*n+y;
   }
   return e;
}

//函数功能：计算节点(x,y)的dir方向的管道是否合理，即是否在原始网络中存在。dir为0表示上方，1表示右侧，2表示下方，3表示左侧。
//参数含义：x，节点的横坐标；y节点的纵坐标；dir，询问的节点方向。
bool existdir(int x, int y, int dir)
{
   int e = getdirline(x,y,dir);
   if (e>=EDGESUM)
       return false;
   return (edges[e].leng!=0);
}

//函数功能：从(x,y)沿着dir方向遍历闭环，遍历到end时，遍历结束。
//参数含义：x，节点的横坐标；y节点的纵坐标；dir，下一步遍历的节点方向；tmp，存储的方程系数；end，遍历停止的管道。
void recursionline(int x, int y, int dir, vector<double> &tmp,int end){
   int e = getdirline(x,y,dir);
   if (e>EDGESUM-6)
       return;
   int nodeno = x*n+y;
   int x1;
   if (edges[e].n1 == nodeno){
       tmp[e] = edges[e].leng;
       x1 = edges[e].n2;
   }
   else{
       tmp[e] = -edges[e].leng;
       x1 = edges[e].n1;
   }

   if (e == end){
       addrect(tmp);
       return;
   }
   if (dir == 0){
       y--;
   }else if (dir == 1){
       x++;
   }else if (dir == 2){
       y++;
   }else if (dir == 3){
       x--;
   }
   for(int i=0; i<4; i++)
   {
       int newdir = dir+1-i;
       newdir = (newdir+4)%4;
       if (existdir(x,y,newdir))
       {
           recursionline(x,y,newdir,tmp,end);
           break;
       }
   }
}

//函数功能：从(x,y)位置进行深度优先搜索，直到到达边end结束
//参数含义：x，节点的横坐标；y节点的纵坐标；tmp，存储的方程系数；end，遍历停止的管道。
bool recursionrect(int x, int y, vector<double> &tmp,int end){
   int xx,yy;
   for(int i=1; i<5; i++)
   {
       int e =  getdirline(x,y,i%4);
       int dir = i%4;
       // cout<<"recursionrect  "<<x<<" "<<y<<" "<<e<<"  "<<i%4<<endl;
       if (e>=EDGESUM)
           continue;
       if (fr[e])
           continue;
       if (edges[e].leng==0)
           continue;
       fr[e] = true;
       int nodeno = x*n+y;
       int x1;
       if (edges[e].n1 == nodeno){
           tmp[e] = edges[e].leng;
           x1 = edges[e].n2;
       }
       else{
           tmp[e] = -edges[e].leng;
           x1 = edges[e].n1;
       }
       if (e == end){
           // cout<<"alsdhjkagjkdhasdhasjkld"<<endl;
           return true;
       }
       if (e>EDGESUM-6){
           tmp[e] = 0;
           continue;
       }

       if (dir == 0){
           xx=x;yy=y-1;
       }else if (dir == 1){
           xx=x+1;yy=y;
       }else if (dir == 2){
           xx=x;yy=y+1;
       }else if (dir == 3){
           xx=x-1;yy=y;
       }
       if (recursionrect(xx,yy,tmp,end))
           return true;
       tmp[e] = 0;
   }
   return false;
}

//函数功能：从(x,y)位置开始向右寻找闭环路径，直到到达(x,y)下方的边t结束
//参数含义：x，节点的横坐标；y节点的纵坐标；t，遍历停止的管道编号。
void findline(int x, int y,int t){
   vector<double> tmp(EDGESUM+1,0);
   recursionline(x,y,1,tmp,t);
}

//函数功能：计算从一个输入端口，到三个输出管道的“电压降”，从而计算得到输出管道两两之间的“电势差”，根据电势差为0，加入方程组
//参数含义：x1，输入管道编号。
void findrect(int x1){
   vector<double> tmp(EDGESUM+1,0);
   for (int i=0; i<EDGESUM; i++)
       fr[i] = false;
   recursionrect(edges[x1].n2/n,0,tmp,EDGESUM-1);
   vector<double> temp(EDGESUM+1,0);
   for (int i=0; i<EDGESUM; i++)
       fr[i] = false;
   recursionrect(edges[x1].n2/n,0,temp,EDGESUM-2);
   vector<double> tep(EDGESUM+1,0);
   for (int i=0; i<EDGESUM; i++)
       fr[i] = false;
   recursionrect(edges[x1].n2/n,0,tep,EDGESUM-3);
   vector<double> emp(EDGESUM+1,0);
   for (int i=0; i<EDGESUM+1; i++)
       emp[i] = tmp[i]-temp[i];
   rect.push_back(emp);
   vector<double> tem(EDGESUM+1,0);
   for (int i=0; i<EDGESUM+1; i++)
       tem[i] = tep[i]-temp[i];
   rect.push_back(tem);
}

//函数功能：初始化方程组（行列式）的值
void initrect(){
   for (int i=0;i<EDGESUM-5; i++)	//不存在的管道液体流速为0
       if (edges[i].leng == 0)
       {
           // cout<<"exist  "<<i<<endl;
           vector<double> tmp(EDGESUM+1,0);
           tmp[i]=1;
           addrect(tmp);
       }
   // cout<<rect.size()<<endl;
   for (int i=0; i<NODESUM-2; i++)  //首先根据基尔霍夫定律，统计所有的电流的相等关系
   {
       int number = 0;
       vector<double> tmp(EDGESUM+1,0);
       for (int j=0; j<nodes[i].elist.size(); j++)
       if (edges[nodes[i].elist[j]].leng!=0)
       {
           number++;
           if (edges[nodes[i].elist[j]].n1 == i)
               tmp[nodes[i].elist[j]] = 1;
           else
               tmp[nodes[i].elist[j]] = -1;
       }
       tmp[EDGESUM] = 0;
       if (number>0)
           addrect(tmp);
   }
   // cout<<rect.size()<<endl;
   for (int i=0; i<n-1; i++)	//寻找电路中的最小环,对于每个环路径电势差为0
       for (int j=0; j<n-1; j++)
       {
           int t = i*(n-1)+j;
           int m = n*n-n+i*n+j;
           if (edges[t].leng !=0 && edges[m].leng !=0)
                   findline(i,j,t);
       }
   // cout<<rect.size()<<endl;

   //三个输出端口之间的电势差为0
   findrect(EDGESUM-4);
   //两个输入端口的流速相同且已知，构成两个方程
   vector<double> tmp(EDGESUM+1,0);
   tmp[EDGESUM-5] = 1;
   tmp[EDGESUM] = 200;
   addrect(tmp);
   vector<double> temp(EDGESUM+1,0);
   temp[EDGESUM-4] = 1;
   temp[EDGESUM] = 200;
   addrect(temp);

   // cout<<rect.size()<<endl;
}

//函数功能：确定a和b之前的最小数。
//参数含义：。
int min (int a, int b)  {
   if (a>b)
       return b;
   else
       return a;
}

//函数功能：计算a和b的最大公约数。
//参数含义：。
double GreatestCommonDivisor(double a, double b)
{
   double t;
   if (a < b)
   {
       t = a;
       a = b;
       b = t;
   }
   if (b==1)
       return 1;
   while (abs(b) >= NAX)
   {
       t = fmod(a , b);
       a = b;
       b = t;
   }
   return a;
}

//函数功能：计算a和b的最小公倍数。
//参数含义：。
double LeastCommonMultiple (double a, double b)

{
   double c = GreatestCommonDivisor(a,b);
   double t = a  /c * b;
   return t;

}

//函数功能：使用行列式计算方程组rect的解。
void getans(bool getv)
{
   int n = rect.size();

   for (int i=0; i<n; i++)
       for (int j=0; j<EDGESUM+1; j++)
           if (abs(rect[i][j])<NAX)
               rect[i][j] = 0;

   int num = 0;
   for (int i=0; i<n; i++){
       if (abs(rect[i][num]) <NAX){
           int mjj = 0;
           for (int j=i+1; j<n; j++)
               if ( !(abs(rect[j][num])<NAX))
               {
                   mjj++;
                   for (int k=0; k<EDGESUM+1; k++)
                   {
                       double t = rect[j][k];
                       rect[j][k] = rect[i][k];
                       rect[i][k] = t;
                   }
                   break;
               }
           // if (mjj == 0)
           // 	cout<<"454354354354354"<<endl;
       }
       for (int j=0; j<n; j++)
           if (i!=j && (abs(rect[j][num])>NAX))
               {
                   double ml = LeastCommonMultiple(abs(rect[j][num]),abs(rect[i][num]));
                   double t = ml/rect[j][num];
                   double kt = ml/rect[i][num];
                   for (int k=EDGESUM; k>=0; k--){
                       rect[j][k] = rect[j][k]*t;
                       rect[j][k] = (abs(rect[j][k])<NAX)?0:rect[j][k];
                   }
                   for (int k=EDGESUM; k>=num; k--)
                   {
                       rect[j][k] -= kt*rect[i][k];
                       rect[j][k] = (abs(rect[j][k])<NAX)?0:rect[j][k];
                   }


               }


           num ++;
           for (int j=0; j<n; j++){
               double common = 0;
               for (int k=0; k<EDGESUM+1; k++)
               if ((abs(rect[j][k]) > NAX)){
                   if (abs(common) < NAX)
                       common = abs(rect[j][k]);
                   else
                       common = GreatestCommonDivisor(common,abs(rect[j][k]));
               }
               if (common!=0)
                   for (int k=0; k<EDGESUM+1; k++){
                       rect[j][k] = rect[j][k]/common;
                       rect[j][k] = (abs(rect[j][k])<NAX)?0:rect[j][k];
                   }
           }
       if (num==EDGESUM)
           break;
   }

   if (getv)
   {
       num = 0;
       for (int i=0; i<EDGESUM; i++)
       {
           edges[num].v = double(rect[i][EDGESUM])/double(rect[i][num]);
           if (edges[num].v<0)
           {
               edges[num].v = -edges[num].v;
               int tm = edges[num].n1;
               edges[num].n1 = edges[num].n2;
               edges[num].n2 = tm;
           }
           num++;
           if (num==EDGESUM)
               break;
       }
   }
    else
   {
       num = 0;
       for (int i=0; i<EDGESUM; i++)
       {
           edges[num].c = double(rect[i][EDGESUM])/double(rect[i][num]);
           num++;
           if (num==EDGESUM)
               break;
       }
   }

}


//函数功能：计算芯片所有管道的液体流速
//参数含义：num，正方形网格的边长（即网格一行的节点数量，比如8X8的网格，一行有8个节点，num为8）；length，存储网格中每个管道的长度，若管道不存在用0表示；i1,i2,o1,o2,o3
//				分别表示两个输入管道与三个输出管道在第几列。
vector<double> caluconspeed(int num, vector<double>&length, int i1, int i2, int o1, int o2, int o3)
{
   rect.clear();
   n = num;
   EDGESUM = 2*n*n-2*n+5;
   NODESUM = n*n+2;
   for (int i=0; i<NODESUM; i++)
       nodes[i].elist.clear();
   int n1 = 0;
   int n2 = 1;
   for (int i=0; i<n*n-n; i++)
   {
       edges[i].n1 = n1;
       edges[i].n2 = n2;
       nodes[n1].elist.push_back(i);
       nodes[n2].elist.push_back(i);
       n1++;n2++;
       if (n2%n==0)
       {
           n1++;n2++;
       }
   }
   n1 = 0;
   n2 = n1+n;
   for (int i=n*n-n; i<2*n*n-2*n; i++)
   {
       edges[i].n1 = n1;
       edges[i].n2 = n2;
       nodes[n1].elist.push_back(i);
       nodes[n2].elist.push_back(i);
       n1++;n2++;
   }

   edges[2*n*n-2*n+0].n1 = n*n;
   edges[2*n*n-2*n+0].n2 = (i1)*n;
   nodes[n*n].elist.push_back(2*n*n-2*n+0);
   nodes[(i1)*n].elist.push_back(2*n*n-2*n+0);

   edges[2*n*n-2*n+1].n1 = n*n;
   edges[2*n*n-2*n+1].n2 = (i2)*n;
   nodes[n*n].elist.push_back(2*n*n-2*n+1);
   nodes[(i2)*n].elist.push_back(2*n*n-2*n+1);

   edges[2*n*n-2*n+2].n1 = (o1+1)*n-1;
   edges[2*n*n-2*n+2].n2 = n*n+1;
   nodes[(o1+1)*n-1].elist.push_back(2*n*n-2*n+2);
   nodes[n*n+1].elist.push_back(2*n*n-2*n+2);

   edges[2*n*n-2*n+3].n1 = (o2+1)*n-1;
   edges[2*n*n-2*n+3].n2 = n*n+1;
   nodes[(o2+1)*n-1].elist.push_back(2*n*n-2*n+3);
   nodes[n*n+1].elist.push_back(2*n*n-2*n+3);

   edges[2*n*n-2*n+4].n1 = (o3+1)*n-1;
   edges[2*n*n-2*n+4].n2 = n*n+1;
   nodes[(o3+1)*n-1].elist.push_back(2*n*n-2*n+4);
   nodes[n*n+1].elist.push_back(2*n*n-2*n+4);

   for (int i=0; i<2*n*n-2*n+5; i++)
   {
       setedgelength(i,length[i]);
   }
   edges[EDGESUM-4].v = 200;
   edges[EDGESUM-5].v = 200;

   initrect();
   getans(true);
   vector<double> v(3,0);
   v[0] = edges[EDGESUM-3].v;
   v[1] = edges[EDGESUM-2].v;
   v[2] = edges[EDGESUM-1].v;
   return v;
}

bool is_vertical(int e)
{
    return (e < n * (n - 1)) || (e >= 2 * n * n - 2 * n);
}

void getCon()
{
    queue<int> q;
    q.push(0);
    for (int i = 0; i < EDGESUM; i++)
    {
        edges[i].c = 0;
    }
    edges[EDGESUM-4].c = 0;
    edges[EDGESUM-5].c = 1;
    /*while (!q.empty())
    {
        int f = q.front();
        vector<int> e = nodes[f].elist;
        int s = e.size();
        int in, out;
        in = out = 0;
        vector<int> c_in;
        vector<int> c_out;
        for (auto eg : e)
        {
            if (edges[eg].n2 == f)
            {
                in++;
                c_in.push_back(eg);
            }
            else
            {
                out++;
                c_out.push_back(eg);
                q.push(edges[eg].n2);
            }
        }
        if (out == 0)
        {
            q.pop();
            continue;
        }
        if (in == 1)
        {
            double v_sum = 0;
            for (auto i : c_out)
            {
                v_sum += edges[i].v;
            }
            double new_c;
            double v1 = edges[c_in[0]].v;
            double c1 = edges[c_in[0]].c;
            new_c = v1 / v_sum * c1;
            for (auto i : c_out)
            {
                edges[i].c = new_c;
            }
        }
        else if (in == 2)
        {
            if (out == 2)
            {
                double v1 = edges[c_in[0]].v;
                double v2 = edges[c_in[1]].v;
                double c1 = edges[c_in[0]].c;
                double c2 = edges[c_in[1]].c;
                double v3 = edges[c_out[0]].v;
                double v4 = edges[c_out[1]].v;
                double v11 = is_vertical(c_in[1]) ? v1 : v2;
                double c11 = is_vertical(c_in[1]) ? c1 : c2;
                double v22 = is_vertical(c_in[1]) ? v2 : v1;
                double c22 = is_vertical(c_in[1]) ? c2 : c1;
                double v33 = is_vertical(c_out[1]) ? v3 : v4;
                double v44 = is_vertical(c_out[1]) ? v4 : v3;
                if (v22 > v33)
                {
                    double c3, c4;
                    c3 = c22;
                    c4 = (v11*c11-v33*c3+v22*c22)/v44;
                    edges[c_out[0]].c = is_vertical(c_out[1]) ? c3 : c4;
                    edges[c_out[1]].c = is_vertical(c_out[1]) ? c4 : c3;
                }
                else//v1>v4
                {
                    double c3, c4;
                    c4 = c11;
                    c3 = (v11*c11-v44*c4+v22*c22)/v33;
                    edges[c_out[0]].c = is_vertical(c_out[1]) ? c3 : c4;
                    edges[c_out[1]].c = is_vertical(c_out[1]) ? c4 : c3;
                }
            }
            else if (out == 1)
            {
                double sum = 0;
                for (auto i : c_in)
                {
                    sum += edges[i].c * edges[i].v;
                }
                edges[c_out[0]].c = sum / edges[c_out[0]].v;
            }
        }
        else if (in == 3)
        {
            double sum = 0;
            for (auto i : c_in)
            {
                sum += edges[i].c * edges[i].v;
            }
            edges[c_out[0]].c = sum / edges[c_out[0]].v;
        }
        q.pop();
    }*/

    stack<int> s;


    vector<int> indegree;
    for (int i = 0; i < NODESUM; i++)
    {
        int sum = 0;
        for (auto e : nodes[i].elist)
        {
            if (edges[e].leng == 0)
                continue;
            if (edges[e].n2 == i)
            {
                sum++;
            }
        }
        indegree.push_back(sum);
    }
    for (int i = 0; i < NODESUM; i++)
    {
        if (indegree[i] == 0)
            s.push(i);
    }

    while (!s.empty())
    {
        int f = s.top();
        cout << f << " ";
        s.pop();
        vector<int> e = nodes[f].elist;
        //int s = e.size();
        int in, out;
        in = out = 0;
        vector<int> c_in;
        vector<int> c_out;
        for (auto eg : e)
        {
            if (edges[eg].leng == 0)
                continue;
            if (edges[eg].n2 == f)
            {
                in++;
                c_in.push_back(eg);
            }
            else
            {
                out++;
                c_out.push_back(eg);
                //q.push(edges[eg].n2);
            }
        }
        if (out == 0)
        {
            break;
        }
        if (in == 1)
        {
            double v_sum = 0;
            for (auto i : c_out)
            {
                v_sum += edges[i].v;
            }
            double new_c;
            double v1 = edges[c_in[0]].v;
            double c1 = edges[c_in[0]].c;
            new_c = v1 / v_sum * c1;
            for (auto i : c_out)
            {
                edges[i].c = new_c;
            }
        }
        else if (in == 2)
        {
            if (out == 2)
            {
                double v1 = edges[c_in[0]].v;
                double v2 = edges[c_in[1]].v;
                double c1 = edges[c_in[0]].c;
                double c2 = edges[c_in[1]].c;
                double v3 = edges[c_out[0]].v;
                double v4 = edges[c_out[1]].v;
                double v11 = is_vertical(c_in[1]) ? v1 : v2;
                double c11 = is_vertical(c_in[1]) ? c1 : c2;
                double v22 = is_vertical(c_in[1]) ? v2 : v1;
                double c22 = is_vertical(c_in[1]) ? c2 : c1;
                double v33 = is_vertical(c_out[1]) ? v3 : v4;
                double v44 = is_vertical(c_out[1]) ? v4 : v3;
                if (v22 > v33)
                {
                    double c3, c4;
                    c3 = c22;
                    c4 = (v11*c11-v33*c3+v22*c22)/v44;
                    edges[c_out[0]].c = is_vertical(c_out[1]) ? c3 : c4;
                    edges[c_out[1]].c = is_vertical(c_out[1]) ? c4 : c3;
                }
                else//v1>v4
                {
                    double c3, c4;
                    c4 = c11;
                    c3 = (v11*c11-v44*c4+v22*c22)/v33;
                    edges[c_out[0]].c = is_vertical(c_out[1]) ? c3 : c4;
                    edges[c_out[1]].c = is_vertical(c_out[1]) ? c4 : c3;
                }
            }
            else if (out == 1)
            {
                double sum = 0;
                for (auto i : c_in)
                {
                    sum += edges[i].c * edges[i].v;
                }
                edges[c_out[0]].c = sum / edges[c_out[0]].v;
            }
        }
        else if (in == 3)
        {
            double sum = 0;
            for (auto i : c_in)
            {
                sum += edges[i].c * edges[i].v;
            }
            edges[c_out[0]].c = sum / edges[c_out[0]].v;
        }
        for (auto a : nodes[f].elist)
        {
            if (edges[a].leng == 0)
                continue;
            if (edges[a].n2 != f)
            {
                indegree[edges[a].n2]--;
                if (indegree[edges[a].n2] == 0)
                {
                    s.push(edges[a].n2);
                }
            }
        }
    }

    for (int i = 0; i < EDGESUM; i++)
    {
        cout << edges[i].c << " ";
    }
    cout << endl;
}

vector<double> neighbour(vector<double> &o)
{
    vector<double> _n;
    int c = -1;
    for (auto i : o)
    {
        c++;
        if (c == n * n * 2 - 2 * n ||
            c == n * n * 2 - 2 * n + 1 ||
            c == n * n * 2 - 2 * n + 2 ||
            c == n * n * 2 - 2 * n + 3 ||
            c == n * n * 2 - 2 * n + 4)
        {
            _n.push_back(1.0);
            continue;
        }
/*        if (i < NAX)
        {
            double a = (double) rand() / RAND_MAX;
            if (a < 0.6)
            {
                double r = 1 + (double) rand() / RAND_MAX * 0.01 - 0.005;
                i = r;
                _n.push_back(i);
                continue;
            }
            else
            {
                _n.push_back(0);
            }
        }
        else
        {
            double b = (double) rand() / RAND_MAX;
            double r;
            if (b < 0.01)
                r = 0;
            else*/
        double a;
        a = rand() / RAND_MAX;
        if (a >= 0.05)
        {
            _n.push_back(i);
            continue;
        }
            double r;
                r = 1 + (double) rand() / RAND_MAX * 0.1 - 0.05;
            i *= r;
            _n.push_back(i);
        //}
        //c++;
    }
    return _n;
}


double annealing(int num, int a1, int a2, int a3, int i1, int i2, int o1, int o2 ,int o3, vector<double> &ori_len, vector<double> &ans)
{
    ans.clear();
    //int number = 5;
    n = num;
    int sum = 2 * n * n - 2 * n + 5;
    vector<double> len;
    len = ori_len;
    vector<double> r(3, 0);
    r = caluconspeed(n, len, i1, i2, o1, o2, o3);
    double e;
    int x, y, z;
    x = a1; y = a2; z = a3;
    e = fabs(r[0] - x) + fabs(r[1] - y) + fabs(r[2] - z);
    cout << "e: " << e << endl;
    int k = 0;
    int kmax = 1000;
    double emax = 2;
    double t0 = 500;
    double d = 0.98;
    double t = t0;
    double tk = 0.01;
    srand(time(0));
    double _min = 10000;
    vector<double> min_ans;
    while (t > tk && e > emax)
    {
        vector<double> sn = neighbour(len);
        /*for (auto i : sn)
        {
            cout << i << " ";
        }*/
        //cout << endl;
        //return 0;
        vector<double> rn = caluconspeed(n, sn, i1, i2, o1, o2, o3);
        double en = fabs(rn[0] - x) + fabs(rn[1] - y) + fabs(rn[2] - z);
        //cout << "en: " << en << endl;
        //return 0;
        //double dt = t - d * t;
        double rd = rand() * 1.0 / RAND_MAX;
        //cout << "rd: " << rd << endl;
        double P = exp((e - en) / t);
        //cout << "P: " << P << endl;
        if (rd < P)
        {
            len = sn;
            e = en;
        }
        t = d * t;
        if (en < _min)
        {
            _min = en;
            min_ans = sn;
        }
    }
    /*for (auto i : len)
    {
        cout << i << " ";
    }*/
    ans = min_ans;
    return _min;
    //cout << endl;
    //cout << "min: " << _min;
    //cout << endl;
}

#endif // CORE_H
