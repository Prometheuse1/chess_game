#include "plateau.h"

plateau::plateau()
{
	init_Plateau();
	//TODO
}
plateau::~plateau()
{
	
}

int plateau::check_arival_space(int ld,int cd,int la,int ca)
{
	if((ech[ld][cd]>0 && ech[la][ca]<0) ||	(ech[ld][cd]<0 && ech[la][ca]>0) || ech[la][ca]==0)
	{
		return 1;
	}
	
	return 0;
}

int plateau::eval_pion(int ld,int cd,int la,int ca)
{
	if(ech[ld][cd]==1)
	{
		// 1 case
		if(la==ld+1 && ca==cd && ech[la][ca]==0)
            return 1;

		// 2 case
        if(ld==1 && la==ld+2 && ech[ld+1][cd]==0 && ech[la][ca]==0)
        	return 1;
        
        // attack
        if(la==ld+1 && (ca==cd+1 || ca==cd-1) && ech[la][ca]<0)
        	return 1;
	}
	
	if(ech[ld][cd]==-1)
	{
		// 1 case
		if(la==ld-1 && ca==cd && ech[la][ca]==0)
            return 1;

		// 2 case
        if(ld==6 && la==ld-2 && ech[ld-1][cd]==0 && ech[la][ca]==0)
        	return 1;
        
        // attack
        if(la==ld-1 && (ca==cd+1 || ca==cd-1) && ech[la][ca]>0)
        	return 1;
	}
	
	return 0;
}

int plateau::eval_cavalier(int ld,int cd,int la,int ca)
{
	if( ((ld+2==la)&&(cd+1==ca)) || ((ld+2==la)&&(cd-1==ca)) || ((ld+1==la)&&(cd+2==ca)) || ((ld-1==la)&&(cd+2==ca)) 
	|| ((ld-2==la)&&(cd+1==ca)) || ((ld-2==la)&&(cd-1==ca)) || ((ld+1==la)&&(cd-2==ca)) || ((ld-1==la)&&(cd-2==ca)) )
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else
	{
		return 0;
	}
}

int plateau::eval_tour(int ld,int cd,int la,int ca)
{
	bool chemin_vide;
	int i;
	if(ld==la)
	{
		chemin_vide=true;
		if(cd<ca)
		{
			i=cd+1;
			while(i<ca && chemin_vide==true)
			{
				if(ech[ld][i]!=0)
				{
					chemin_vide=false;
				}
				i++;
			}
		}
		else if(cd>ca)
		{
			i=cd-1;
			while(i>ca && chemin_vide==true)
			{
				if(ech[ld][i]!=0)
				{
					chemin_vide=false;
				}
				i--;
			}
		}	
	}
	if(cd==ca)
	{
		chemin_vide=true;
		if(ld<la)
		{
			i=ld+1;
			while(i<la && chemin_vide==true)
			{
				if(ech[i][cd]!=0)
				{
					chemin_vide=false;
				}
				i++;
			}
		}
		if(ld>la)
		{
			i=ld-1;
			while(i>la && chemin_vide==true)
			{
				if(ech[i][cd]!=0)
				{
					chemin_vide=false;
				}
				i--;
			}
		}
	}
	
	if(chemin_vide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::eval_fou(int ld,int cd,int la,int ca)
{
    bool chemin_vide=false;
    int i,j;
    if(ld!=la && cd!=ca )
    {chemin_vide=true;
        if(la>ld && ca>cd)
        {  i=ld+1;
            j=cd+1;
            while(i<la && chemin_vide==true)
            {
                if(ech[i][j]!=0) chemin_vide=false;
                i++; j++;
            }
        }
        else if(la>ld && ca<cd)
        {
            i=ld+1;
            j=cd-1;
            while(i<la && chemin_vide==true)
            {
                if(ech[i][j]!=0) chemin_vide=false;
                i++; j--;
            }
        }
        else if(la<ld && ca>cd)
        {
            i=ld-1;
            j=cd+1;
            while(i>la && chemin_vide==true)
            {
                if(ech[i][j]!=0) chemin_vide=false;
                i--; j++;
            }
        }
        else if(la<ld && ca<cd)
        {
            i=ld-1;
            j=cd-1;
            while(i>la && chemin_vide==true)
            {
                if(ech[i][j]!=0) chemin_vide=false;
                i--; j--;
            }
        }
    }
   	if(chemin_vide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::evaluation(int ld,int cd,int la,int ca)
{
	switch(ech[ld][cd])
	{
		case  1: return eval_pion(ld,cd,la,ca);		break;
		case -1: return eval_pion(ld,cd,la,ca);		break;
		case  2: return eval_tour(ld,cd,la,ca); 	break;
		case -2: return eval_tour(ld,cd,la,ca);		break;
		case  3: return eval_cavalier(ld,cd,la,ca); break;
		case -3: return eval_cavalier(ld,cd,la,ca);	break;
		case  4: return eval_fou(ld,cd,la,ca);		break;
		case -4: return eval_fou(ld,cd,la,ca);		break;
		/*case  5: break;
		case -5: break;
		case  6: break;
		case -6: break;*/
	}
}


plateau::mov_Piece(int ld,int cd,int la,int ca)
{
	if(evaluation(ld,cd,la,ca)==1)
	{
		ech[la][ca]=ech[ld][cd];
		ech[ld][cd]=0;		
	}
	else
	{
		//TODO
	}
}


int plateau::get_Piece(int i,int j)
{
	return ech[i][j];
}

plateau::init_Plateau()
{
	for(int i=2;i<=5;i++)
	{
		for(int j=0;j<=7;j++)
		{
			ech[i][j]=0; // Position Vide
		}
	}
	
	for(int j=0;j<=7;j++)
	{
		ech[1][j]=1; // Pion Blanc
		ech[6][j]=-1;// Pion Noir
	}
	
	ech[0][0]=2; // Tour
	ech[0][1]=3; // Cavalier
	ech[0][2]=4; // Fou
	ech[0][3]=6; // Roi
	ech[0][4]=5; // Reine/Dame
	ech[0][5]=4; // Fou
	ech[0][6]=3; // Cavalier
	ech[0][7]=2; // Tour

	ech[7][0]=-2; // Tour
	ech[7][1]=-3; // Cavalier
	ech[7][2]=-4; // Fou
	ech[7][3]=-6; // Roi
	ech[7][4]=-5; // Reine/Dame
	ech[7][5]=-4; // Fou
	ech[7][6]=-3; // Cavalier
	ech[7][7]=-2; // Tour
		
	
}
