#include <iostream>
#include <time.h>
#include<iomanip>
using namespace std;

const int IDLE = 0;
const int MISSILE = 1;
const int FIREBALL = 2;
const int DAGGER = 3;
const int STAFF = 4;
const int GLOBE = 5;
const int STONESKIN = 6;
const int HEAL = 7;
const int STRENGTH = 8;

const int numAction = 9; // 9 actions 
const int numFeature = 7; // 1 feature: alive

float alpha = 0.001;
float gamma = 0.3;
float epsilon = 0.9;

float weights[numAction][numFeature]; // the weight matrix used for functon approximation

int features[numFeature]; // boolean features with values 1 or 0

int selectAction();
void printWeights();
int rounds, action, damagedealt, damagetaken, oppB50, oppB10, GoIActive, GOIoppActive, SSActive, SSoppActive, StrActive, StroppActive, endroundsig, turn = 0;
float reward = 0;
float maxQSA = -100;
float nextQSA;
float maxValue = -100;
int healthcount = 0;
int strengthcount = 0;
int GOIturncount = 0;
int SSturncount = 0;



int main()
{
	srand(time(NULL));

	// random initialization of weights
	for (int i = 0; i < numAction; i++) {
		for (int j = 0; j < numFeature; j++) {
			weights[i][j] =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}
	}
	features[0] = 1;
	cin >> rounds;
	
	for (int i = 0; i < rounds; i++)
	{
		turn = 0;
		while (turn == 0)
		{
			//State input
			cout << selectAction() << endl;
			if (selectAction() == 7)
			{
				healthcount++;
			}
			else if (selectAction() == 8)
			{
				strengthcount++;
			}
			else if (selectAction() == 5)
			{
				GOIturncount++;
			}
			else if (selectAction() == 6)
			{
				SSturncount++;
			}

			cin >> action;
			cin >> damagedealt;
			cin >> damagetaken;
			cin >> oppB50;
			cin >> oppB10;
			cin >> GoIActive;
			features[1] = GoIActive;
			cin >> SSActive;
			features[2] = SSActive;
			cin >> StrActive;
			features[3] = StrActive;
			cin >> GOIoppActive;
			features[4] = GOIoppActive;
			cin >> SSoppActive;
			features[5] = SSoppActive;
			cin >> StroppActive;
			features[6] = StroppActive;
			cin >> endroundsig;

			printWeights();

			//Reward determination and endroundsig
			if (endroundsig != 0)
			{
				turn = 1;
				strengthcount = 0;
				GOIturncount = 0;
				SSturncount = 0;
				healthcount = 0;
				break;
			}
			//Rewards
			if (damagedealt > 0 || damagedealt > damagetaken || damagetaken == 0)
			{
				reward = 0.5;
			}
			else if (damagetaken > 0 || damagedealt < damagetaken)
			{
				reward = -0.1;
			}
			//Calculate Q(s',a')
			for (int i = 0; i < numAction; i++)
			{
				nextQSA = 0;
				for (int j = 0; j < numFeature; j++)
				{
					nextQSA += weights[i][j] * features[j];
				}
				if (nextQSA > maxQSA)
				{
					maxQSA = nextQSA;
				}
			}
			for (int i = 0; i < numAction; i++)
			{
				for (int j = 0; j < numFeature; j++)
				{
					//Update Weights
					weights[i][j] = weights[i][j] + alpha * (reward + (gamma * (maxQSA - maxValue)));
				}
			}
			
		}	
	}
	cerr << endl;
	cout.flush();
	return 0;

}
// selects an action based on reinforcement learning
int selectAction()
{
	//Calculates max Q(s,a) and uses that action that is associated with variable I
	int action = 0;
	for (int i = 0; i < numAction; i++)
	{
		float qsa = 0;
		for (int j = 0; j < numFeature; j++)
		{
			qsa += weights[i][j] * features[j];
			
		}
		if (qsa > maxValue)
		{
			maxValue = qsa;		
			action = i;
			if (action == 7)
			{
				damagetaken = damagetaken - 30;
				healthcount++;
			}
			else if (action == 8)
			{
				strengthcount++;
			}
		}
		if (action == 0)
		{
			action = (int)(rand() % 8 + 1);
		}
		if (action == 7 && healthcount >= 1)
		{
			action = (int)(rand() % 8 + 1);
		}
		if (action == 8 && strengthcount >= 1)
		{
			action = (int)(rand() % 8 + 1);
		}
		if (action == 5 && GOIturncount % 5 == 0)
		{
			action = 5;
		}
		if (action == 6 && SSturncount % 5 == 0)
		{
			action = 6;
		}
	}
	return action;

}


// prints the weight matrix and the features in the current state
void printWeights()
{
	cerr << "Weight matrix: " << endl;
	for (int i = 0; i < numAction; i++) {
		for (int j = 0; j < numFeature; j++) {
			cerr << weights[i][j] << " ";
		}
		cerr << endl;
	}

	cerr << "Features in the state: " << endl;
	for (int i = 0; i < numFeature; i++) {
		cerr << features[i] << " ";
	}
	cerr << endl;
}