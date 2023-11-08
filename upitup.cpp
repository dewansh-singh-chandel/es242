#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include<tuple>
#include<cmath>
#include<queue>
#include<assert.h>
// #include "queue.hpp"
using namespace std;

struct board{
    int arr[3][3];
};

enum {
    X=1,
    O=2
};
// enum{
//     L=1, R=2, U=3, D=4
// };
// int encode_configuration(int arr[]){
//     int i;
//     for(i=0;i<6;i++){
//         if(arr[i]==1){
//             break;
//         }
//     }
//     if(i==6){return 0;}
//     return i+1;
// }
void print_board(const board& board){
    for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
            cout<<board.arr[r][c]<<" ";
        }
        cout<<endl;
    }
    return;
}

pair<int,int> find_space(const board& board){
    int r=0,c=0;
    for(r=0;r<3;r++){
        for(c=0;c<3;c++){
            if(board.arr[r][c]==0){
                return make_pair(r,c);
            }
        }
    }
    assert(0);
}
bool checkvalid(const board& board ){           //Want 1's on the top

    // for(int r=0;r<3;r++){
    //     if(board.arr[r][0]!=2){return false;}
    // }
    // if(board.arr[0][1]!=0){return false;}
    // if(board.arr[1][1]!=4){return false;}
    // if(board.arr[2][1]!=3){return false;}
    // if(board.arr[0][2]!=3){return false;}
    // if(board.arr[1][2]!=2){return false;}
    // if(board.arr[2][2]!=2){return false;}
    // print_board(board);
    // return true;

    for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
            if(board.arr[r][c]>1){return false;}
            if((r*c==1)){
                if(board.arr[1][1]!=0){return false;}
            }
        }
    }
    return true;
    // for(int r=0;r<3;r++){
    //     for(int c=0;c<3;c++){
    //         if(board.arr[r][c]!=0){return false;}
    //     }
    // }
    // return true;
}

int ord(const board& board){
    int o=0;
    int i=0;
    for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
            o+=board.arr[r][c]*pow(7,i);
            i++;
        }
    }
    return o;
}

board up(board b, int r, int c){
    board bnew=b;
    int val=b.arr[r-1][c];
    if(val==1){val=6;}
    else if(val==2){val=5;}
    else if(val==5){val=1;}
    else if(val==6){val=2;}
    // cout<<"Up\n";
    // print_board(b);
    bnew.arr[r][c]=val;
    bnew.arr[r-1][c]=0;
    
    // cout<<endl;
    // print_board(bnew);
    return bnew;
}

board down(const board& b, int r, int c){
    board bnew=b;
    int val=b.arr[r+1][c];
    if(val==1){val=5;}
    else if(val==2){val=6;}
    else if(val==5){val=2;}
    else if(val==6){val=1;}
    bnew.arr[r][c]=val;
    bnew.arr[r+1][c]=0;
    return bnew;
}

board left(const board& b, int r, int c){
    board bnew=b;
    int val=b.arr[r][c-1];
    if(val==1){val=4;}
    else if(val==2){val=3;}
    else if(val==3){val=1;}
    else if(val==4){val=2;}
    bnew.arr[r][c]=val;
    bnew.arr[r][c-1]=0;
    return bnew;
}

board right(const board& b, int r, int c){
    board bnew=b;
    int val=b.arr[r][c+1];
    if(val==1){val=3;}
    else if(val==2){val=4;}
    else if(val==3){val=2;}
    else if(val==4){val=1;}
    bnew.arr[r][c]=val;
    bnew.arr[r][c+1]=0;
    return bnew;
}

uint8_t visited[40353600]={0};
uint64_t parent[40353600];

vector<char> solve(const board& b){
    // cout<<"Hello"<<endl;
    int count=0;
    queue<board> q;             
    q.push(b);
    visited[ord(b)] = 'S';          //S means start
    while(!q.empty()){
        board u=q.front();
        q.pop();
        if(checkvalid(u)){
            vector<char> moves;
            int o=ord(u);
            while(visited[o]!='S'){
                moves.push_back(visited[o]);
                o=parent[o];
            }
            std::reverse(moves.begin(), moves.end());
            print_board(u);
            return moves;
        }   

        auto p=find_space(u);
        int row= p.first;
        int col= p.second;
        // cout<<row<<col<<endl;
        board a,b,c,d;
        if(row!=0){
            a=up(u,row,col);              //move the empty box/space up i.e move move upper box down
            int aord = ord(a);
            if (!visited[aord]) {
                visited[aord] = 'U';
                parent[aord] = ord(u);
                q.push(a);count++;
            }
        }
        if(row!=2){
            b = down(u,row,col);  
            int bord = ord(b);
            if (!visited[bord]) {
                visited[bord] = 'D';
                parent[bord] = ord(u);
                q.push(b);count++;
            }
        }
        if(col!=0){
            c = left(u,row,col);
            int cord = ord(c);
            if (!visited[cord]) {
                visited[cord] = 'L';
                parent[cord] = ord(u);
                q.push(c);count++;
            }
        }
        if(col!=2){
            d = right(u,row,col);
            int dord = ord(d);
            if (!visited[dord]) {
                visited[dord] = 'R';
                parent[dord] = ord(u);
                q.push(d);count++;
            }
        }
    }
    vector<char> v;
    v.push_back(0);
    return v;
}

void read_board(board& b){                               //T B L R U D
    for(int r=0;r<3;r++){                                //1 2 3 4 5 6 0
        for(int c=0;c<3;c++){
            if(r*c==1){
                b.arr[r][c]=0;
            }
            else{
                int val=2;
                // cin>>val;
                assert(val!=0);
                b.arr[r][c]=val;
            }
        }
    }
    return;
}

int main(){
    board b;
    read_board(b);
    print_board(b);
    // cout<<"Hellobefore solve"<<endl;
    auto moves=solve(b);
    // cout<<"\nHelloENDof Move";
    if(moves[0]==0){
        cout<<"The board is unsolvable";
    }
    for(char c: moves){
        cout<<c<<endl;
    }
    return 0;
}