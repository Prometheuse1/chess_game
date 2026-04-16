#include <iostream>
#include "plateau.h"

using namespace std;

void afficher(plateau p)
{
	for(int i=0;i<=7;i++)
	{
		for(int j=0;j<=7;j++)
		{
			switch(p.get_Piece(i,j))
			{
				case 0: cout<<"  "; break;	
				case 1: cout<<"PB"; break;	
				case 2: cout<<"TB"; break;	
				case 3: cout<<"CB"; break;	
				case 4: cout<<"FB"; break;	
				case 5: cout<<"DB"; break;	
				case 6: cout<<"RB"; break;	

				case -1: cout<<"PN"; break;	
				case -2: cout<<"TN"; break;	
				case -3: cout<<"CN"; break;	
				case -4: cout<<"FN"; break;	
				case -5: cout<<"DN"; break;	
				case -6: cout<<"RN"; break;	

			} 
			cout<<" | ";
		}
		cout<<endl;
		cout<<"----------------------------------------"<<endl;
	}	
}

void deplacer_Piece(plateau &p)
{
	int ld,cd,la,ca;
	do{
		cout<<"Coord de depart : (L/C) "<<endl;
		cin>>ld;
		cin>>cd;
	}while( (ld<0) || (ld>7) || (cd<0) || (cd>7) || (p.get_Piece(ld,cd)==0) || ((p.get_Tour()%2==0)&&(p.get_Piece(ld,cd)<0) ) || ((p.get_Tour()%2!=0)&&(p.get_Piece(ld,cd)>0) ));	
	cout<<"Coord d arrivee : (L/C) "<<endl;
	cin>>la;
	cin>>ca;
	
	p.mov_Piece(ld,cd,la,ca);
}

int menu()
{
	int a;
	cout<<"MENU "<<endl<<"1- J vs J"<<endl<<"2- J vs IA"<<endl;
	cin>>a;
	return a;
}

main()
{
	int choix;
	plateau p;
	choix=menu();
	if(choix==1)
	{
		do{
			if(p.get_Tour()%2==0)
			{
				cout<<"Joueur Blanc"<<endl;
				afficher(p);
				deplacer_Piece(p);			
			}
			else
			{
				cout<<"Joueur Noir"<<endl;			
				afficher(p);
				deplacer_Piece(p);			
			}
	
		}while(p.fin_Partie()==0);
	}
	else
	{
		do{
			if(p.get_Tour()%2==0)
			{
				cout<<"Joueur Blanc"<<endl;
				afficher(p);
				deplacer_Piece(p);			
			}
			else
			{
				cout<<"Joueur Noir"<<endl;			
				p.deplacer_IA();
			}
	
		}while(p.fin_Partie()==0);		
	}
	
}
