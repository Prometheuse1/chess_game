#include <iostream>
#include "plateau.h"

using namespace std;

void afficher(plateau p)
{
	for(int i=0;i<=7;i++)
	{
		for(int j=0;j<=7;j++)
		{
			switch(p.getPiece(i,j))
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

void deplacerPiece(plateau &p)
{
	int ld,cd,la,ca;
	do{
		cout<<"Coord de depart : (L/C) "<<endl;
		cin>>ld;
		cin>>cd;
	}while( (ld<0) || (ld>7) || (cd<0) || (cd>7) || (p.getPiece(ld,cd)==0));	
	cout<<"Coord d arrivee : (L/C) "<<endl;
	cin>>la;
	cin>>ca;
	
	p.movPiece(ld,cd,la,ca);
}

main()
{
	int choix=1;
	plateau p;
	while(choix!=0)
	{
		afficher(p);
		deplacerPiece(p);
		afficher(p);
		cout<<"0-exit 1-continue"<<endl;
		cin>>choix;
	}
	
}
