#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class CSquare;
const char gPound = 156;

int randomSelection()
{
	return static_cast<int>( static_cast<double> (rand()) / (RAND_MAX + 1) * 6.0f + 1 );
}

class CPlayer
{
private:
	int mPlayer;
	string mName;
	int mMoney;
	vector<CSquare*>::iterator mPosition;
public:
	CPlayer(string,int,vector<CSquare*>::iterator);
	int getMoney(){return mMoney;}
	int getPlayer(){return mPlayer;}
	void addMoney(int x){mMoney = mMoney + x;}
	void removeMoney(int x){mMoney = mMoney - x;}
	string getName(){return mName;}
	void movePosition(int,vector<CSquare*>*);
	vector<CSquare*>::iterator getPosition(){return mPosition;}
	
};

void CPlayer::movePosition(int distance, vector<CSquare*>* pSquares)
{
			for (int i=1; i<=distance;i++)
			{
				mPosition++;
				if (mPosition == (*pSquares).end())
				{
					mPosition = (*pSquares).begin();
					mMoney = mMoney+200;
				}
			}
}

CPlayer::CPlayer(string name,int player,vector<CSquare*>::iterator startPosition)
{
	mMoney = 1500; 
	mPosition = startPosition; 
	mName = name; 
	mPlayer = player;
}


class CSquare
{
private:
	string mName;
public:
	virtual void Land(CPlayer*,vector<CSquare*>*)=0;
	virtual bool PropertyGroupCheck(int,int)=0;
	string getName(){return mName;}
	CSquare(string name){mName = name;}
};

class CPropertySquare: public CSquare
{
private:
	int mOwner;
	CPlayer* pOwner;
	int mCost;
	int mRent;
	int mID;
public:
	void Land(CPlayer*,vector<CSquare*>*);
	CPropertySquare(string,int,int,int);
	bool PropertyGroupCheck(int,int);
	bool OwnerGroupCheck(vector<CSquare*>*);
};

CPropertySquare::CPropertySquare(string name, int cost, int rent, int id):CSquare(name)
{
	mOwner = -1;
	pOwner = 0;
	mCost = cost;
	mRent = rent;
	mID = id;
}

void CPropertySquare::Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares)
{
	if (mOwner == -1)
	{
		if (landingPlayer->getMoney() > 0)
			 {
				 mOwner = landingPlayer->getPlayer();
				 pOwner = landingPlayer;
				 landingPlayer->removeMoney(mCost);
				 cout << landingPlayer->getName() << " buys " << getName() << " for " << gPound << mCost << endl;
			 }
	}

	if (mOwner != landingPlayer->getPlayer()  && mOwner != -1)
	{
		//<Player> pays <rent>’
		if (OwnerGroupCheck(pSquares))
		{
			landingPlayer->removeMoney(mRent*2);
			pOwner->addMoney(mRent*2);
			cout << landingPlayer->getName() << " pays " << mRent*2 << endl;
		}
		else
		{
			landingPlayer->removeMoney(mRent);
			pOwner->addMoney(mRent);
			cout << landingPlayer->getName() << " pays " << mRent << endl;
		}
	}
	 
}
bool CPropertySquare::PropertyGroupCheck(int id,int player)
{
	if (id != mID)
	{
		return true;
	}
	if (id == mID && player == mOwner)
	{
		return true;
	}
	return false;
}
bool CPropertySquare::OwnerGroupCheck(vector<CSquare*>* pSquares)
{
	bool groupOwner=true;
	vector<CSquare*>::iterator itSquares;
	for (itSquares = pSquares->begin();  itSquares != pSquares->end(); itSquares++)
	{
		groupOwner= (*itSquares)->PropertyGroupCheck(mID,mOwner);
		if (groupOwner==false){return false;}
	}
	return true;
}

class CBonusSquare: public CSquare
{
private:
	string mBonus[6];
	int mAmount[6];
public:
	CBonusSquare(string);
	void Land(CPlayer*,vector<CSquare*>*);
	bool PropertyGroupCheck(int,int){return true;}
};

CBonusSquare::CBonusSquare(string name) : CSquare(name)
{
	mBonus[0] = "Find some money. ";
	mBonus[1] = "Win competition.";
	mBonus[2] = "Tax rebate.";
	mBonus[3] = "Win lottery.";
	mBonus[4] = "Bequest.";
	mBonus[5] = "Birthday.";
	mAmount[0] = 20;
	mAmount[1] = 50;
	mAmount[2] = 100;
	mAmount[3] = 150;
	mAmount[4] = 200;
	mAmount[5] = 300;
}

void CBonusSquare::Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares)
{
	int selection =  randomSelection()-1; //needs random function filled out
	cout << mBonus[selection] << " Player gains " << gPound  << mAmount[selection] << endl;
	landingPlayer->addMoney(mAmount[selection]);
}

class CPenaltySquare: public CSquare
{
private:
	string mPenalty[6];
	int mAmount[6];
public:
	CPenaltySquare(string);
	void Land(CPlayer*,vector<CSquare*>*);
	bool PropertyGroupCheck(int,int){return true;}
};

CPenaltySquare::CPenaltySquare(string name): CSquare(name)
{
	mPenalty[0] = "Pay food bill.";
	mPenalty[1] = "Pay phone bill.";
	mPenalty[2] = "Pay heating bill.";
	mPenalty[3] = "Pay vehicle tax.";
	mPenalty[4] = "Pay fuel bill.";
	mPenalty[5] = "Pay windfall tax.";
	mAmount[0] = 20;
	mAmount[1] = 50;
	mAmount[2] = 100;
	mAmount[3] = 150;
	mAmount[4] = 200;
	mAmount[5] = 300;
}

void CPenaltySquare::Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares)
{
	int selection =  randomSelection()-1; //needs random function filled out
	cout << mPenalty[selection] << " Player loses " << gPound  << mAmount[selection] << endl;
	landingPlayer->removeMoney(mAmount[selection]);
}

class CGoToJailSquare:public CSquare
{
public:
	CGoToJailSquare(string name):CSquare(name){};
	void Land(CPlayer*,vector<CSquare*>*);
	bool PropertyGroupCheck(int,int){return true;}
};

void CGoToJailSquare::Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares)
{
	landingPlayer->removeMoney(250);
	landingPlayer->movePosition(13,pSquares);
	cout << landingPlayer->getName() << " goes to Jail" << endl;
	cout << landingPlayer->getName() << " pays "<< gPound << "50" << endl;
}

class CGoSquare: public CSquare
{
private:
public:
	CGoSquare(string name):CSquare(name){};
	void Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares){cout << landingPlayer->getName() << " is resting" << endl;}
	bool PropertyGroupCheck(int,int){return true;}
};

class CFreeParkingSquare: public CSquare
{
public:
	CFreeParkingSquare(string name):CSquare(name){};
	void Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares){cout << landingPlayer->getName() << " is resting" << endl;}
	bool PropertyGroupCheck(int,int){return true;}
};

class CJailSquare: public CSquare
{
public:
	CJailSquare(string name):CSquare(name){};
	void Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares){cout << landingPlayer->getName() << " is just visiting" << endl;}
	bool PropertyGroupCheck(int,int){return true;}
};


class CAirportSquare: public CSquare
{
private:
	int mOwner;
	CPlayer* pOwner;
	int mCost;
	int mRent;
public:
	CAirportSquare(string);
	void Land(CPlayer*,vector<CSquare*>*);
	bool PropertyGroupCheck(int,int){return true;}
};

CAirportSquare::CAirportSquare(string name):CSquare(name)
{
	mOwner = -1;
	pOwner = 0;
	mCost = 200;
	mRent = 10;
}

void CAirportSquare::Land(CPlayer* landingPlayer, vector<CSquare*>* pSquares)
{
	if (mOwner == -1)
	{
		if (landingPlayer->getMoney() > 0)
			 {
				 mOwner = landingPlayer->getPlayer();
				 pOwner = landingPlayer;
				 landingPlayer->removeMoney(mCost);
				 cout << landingPlayer->getName() << " buys " << getName() << " for " << gPound << mCost << endl;
			 }
	}

	if (mOwner != landingPlayer->getPlayer() && mOwner != -1)
	{
		//<Player> pays <rent>’
		landingPlayer->removeMoney(mRent);
		pOwner->addMoney(mRent);
		cout << landingPlayer->getName() << " pays " << mRent << " flying cost" << endl;
	}
}




class CMonopoly
{
private:
	int mNumPlayers;
	vector <CPlayer*> pPlayers;
	vector <CSquare*> pSquares;
	int mMaxRounds;
public:
	CMonopoly();
	void Play();
	~CMonopoly();
};

CMonopoly::CMonopoly()
{
	string name1 = "";
	string name2 = "";
	string name3 = "";
	int rent = 0;
	int cost = 0;
	int id = 0;	
	mMaxRounds = 20;
	string playerNames[6] = {"Dog","Car","Top Hat", "Boot", "Battleship","Thimble"};

	int numPlayers;
	bool correctNum;
	do
	{
		cout << "Input number of players from 2 - 6:" << endl;
		cin >> numPlayers;
		correctNum = true;

		if (numPlayers < 2)
		{
			cout << "There must be more than 2 Players" << endl;
			correctNum = false;
		}
		if (numPlayers > 6)
		{
			cout << "There must be less than 6 Players" << endl;
			correctNum = false;
		}
	}
	while (!correctNum);

	mNumPlayers = numPlayers;

	ifstream infile;
	infile.open("Monopoly.txt");
	if( !infile )
	{
		cout << "Can't open input file Monopoly.txt\n";
		exit(1);
	}

	infile >> name1;
	pSquares.push_back(new CGoSquare(name1));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1;
	pSquares.push_back(new CBonusSquare(name1));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1;
	pSquares.push_back(new CJailSquare(name1));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2;
	pSquares.push_back(new CAirportSquare(name1 + " " + name2));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2;
	pSquares.push_back(new CFreeParkingSquare(name1 + " " + name2));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1;
	pSquares.push_back(new CPenaltySquare(name1));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> name3;
	pSquares.push_back(new CGoToJailSquare(name1 + " " +  name2 +  " " + name3));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2;
	pSquares.push_back(new CAirportSquare(name1 + " " + name2));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile >> name1 >> name2 >> cost >> rent >> id;
	pSquares.push_back(new CPropertySquare(name1 + " " + name2,cost,rent,id));

	infile.close();

	vector<CSquare*>::iterator itStart = pSquares.begin();
	for (int i=0;i<numPlayers;i++)
	{
		pPlayers.push_back(new CPlayer(playerNames[i],i,itStart));
	}

	int randomSeed=0;
	infile.open("seed.txt");
	if( !infile )
	{
		cout << "Can't open input file seed.txt\n";
		exit(1);
	}
	infile >> randomSeed;
	infile.close();
	srand(randomSeed);

	cout << "Welcome to Monopoly" << endl << endl;
	Play();
}

void CMonopoly::Play()
{
	int roll = 0;
	vector<CPlayer*>::iterator itPlayers;
	vector<CSquare*>::iterator itPosition;
	for (int i = 1;i<=mMaxRounds;i++)
	{
		cout << " ROUND: " << i << endl;
		for( itPlayers = pPlayers.begin();itPlayers != pPlayers.end(); itPlayers++)
		{
			roll = randomSelection();
			cout << (*itPlayers)->getName() << " rolls " << roll << endl;
			(*itPlayers)->movePosition(roll,&pSquares);
			itPosition = (*itPlayers)->getPosition();

			cout << (*itPlayers)->getName() << " has landed on " << (*itPosition)->getName()<<endl;
			(*itPosition)->Land(*itPlayers,&pSquares);
			cout << (*itPlayers)->getName() << " has " << gPound  << (*itPlayers)->getMoney() << endl << endl;
		}
	}
}

CMonopoly::~CMonopoly()
{
	vector <CSquare*>::iterator itSquares;
	for (itSquares=pSquares.begin();itSquares!=pSquares.end();itSquares++)
	{
		delete *itSquares;
	}
	vector <CPlayer*>::iterator itPlayers;
	for (itPlayers=pPlayers.begin();itPlayers!=pPlayers.end();itPlayers++)
	{
		delete *itPlayers;
	}
}


int main()
{
	
	CMonopoly* myMonopoly = new CMonopoly();
	delete myMonopoly;

	system("pause");
	return 0;
}