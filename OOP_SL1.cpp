#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
class game{
    private:
        int player1,player2;
        const int size=100,nl=8,ns=6;
        int a[100];
        void Ini(){
            for(int i=0;i<100;i++)
                a[i]=i;
        }
        void Ladders(){
            a[1]=37;
            a[3]=13;
            a[7]=29;
            a[27]=75;
            a[20]=41;
            a[49]=66;
            a[79]=98;
            a[70]=91;
        }
        void Snakes(){
            a[33]=5;
            a[31]=9;
            a[61]=17;
            a[87]=23;
            a[96]=77;
            a[47]=25;
        }
        void Move(int x,int player){
            if(player==1){
                if(player1+x>=99)
                    player1=99;
                else
                    player1=a[player1+x];
            }
            if(player==2){
                if(player2+x>=99)
                    player2=99;
                else
                    player2=a[player2+x];
            }
        }
    public:
        game(){
            Ini();
            Ladders();
            Snakes();
            player1=player2=0;
        }
        game(int a,int b){
            Ini();
            for(int i=1;i<=a;i++){
                int k;
                std::cout<<"\nEnter the starting position of ladder "<<i<<": ";
                std::cin>>k;
                std::cout<<"\nEnter the ending position of ladder "<<i<<": ";
                std::cin>>a[k];
            }
            for(int i=1;i<=b;i++){
                int k;
                std::cout<<"\nEnter the starting position of snake "<<i<<": ";
                std::cin>>k;
                std::cout<<"\nEnter the ending position of snake "<<i<<": ";
                std::cin>>a[k];
            }
        }
        ~game(){
            std::cout<<"\nThank you for playing!!";
        }
        int Show(int player){
            if(player==1)
                return player1+1;
            else
                return player2+1;
        }
        int Throw_Dice(int player){
            //randomize();
            int x=random()%6+1;
            Move(x,player);
        }
};

int main(){
	game g;
	std::cout<<"\nPlayer 1\tPlayer 2\n----------------------------";
	while((g.Show(1)!=99)&&(g.Show(2)!=99)){
	    std::cout<<"\n  "<<g.Show(1)<<"\t\t"<<g.Show(2);
	    g.Throw_Dice(1);
	    g.Throw_Dice(2);
	}
	std::cout<<"\n  "<<g.Show(1)<<"\t  "<<g.Show(2);
	if((g.Show(1)==99)&&(g.Show(2)==99)){
	    std::cout<<"\nIt was a draw";
	}
	else if(g.Show(1)==100){
	    std::cout<<"\nPlayer 1 won!!";
	}
	else std::cout<<"\nPlayer 2 won!!";
	return 0;
}
