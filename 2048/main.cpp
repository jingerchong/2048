#include "tuple2.h"

void saveNetwork() {
	std::ofstream network;
	network.open("network.txt", std::ofstream::trunc);
	for (int i = 0; i < numTypes; i++) {
		for (int j = 0; j < numSixTuples; j++) {
			network << lookUpTables[i][j] << ",";
		}
		network << std::endl;
	}
	for (int i = 0; i < numTypes; i++) {
		for (int k = 0; k < numFourTuples; k++) {
			network << lookUpTables[i + numTypes][k] << ",";
		}
		network << std::endl;
	}
	network.close();
	std::cout << "Network saved to file." << std::endl;
}

void loadNetwork() {
	std::ifstream network("network.txt");
	lookUpTables.clear();
	while (network) {
		std::string s;
		if (!std::getline(network, s)) break;
		std::istringstream ss(s);
		std::vector<float> lookUpTable;
		while (ss) {
			std::string s;
			if (!std::getline(ss, s, ',')) break;
			lookUpTable.push_back(std::stof(s));
		}
		lookUpTables.push_back(lookUpTable);
	};
	network.close();
	std::cout << "Network successfully loaded." << std::endl;
};

void playRounds(int n) {
	//Parameter: 
	//	n - number of rounds to be played (int)

	Game game;
	bool gameOver;

	std::ofstream log;
	log.open("log.txt", std::ios_base::app);
	log.close();

	for (int i = 1;i < n + 1;i++) {

		Grid prevState;
		Grid prevAfterstate;
		Grid currState;
		Grid currAfterstate;
		float tdTarget;
		float oldValue;
		float newValue;
		float delta;
		gameOver = false;

		game.copyGrid(prevState);
		moveDir a = nextMove(prevState);
		game.makeMove(a);
		game.copyGrid(prevAfterstate);

		oldValue = valueFunction(prevAfterstate);
		game.addTile();


		while (!gameOver) {

			game.copyGrid(currState);
			moveDir a = nextMove(currState);
			game.makeMove(a);
			game.copyGrid(currAfterstate);
			
			newValue = valueFunction(currAfterstate);
			tdTarget = (float) currAfterstate.getReward() + newValue;
			
			delta = alpha * (tdTarget - oldValue);
			trainNetwork(prevAfterstate, delta);
			
			prevAfterstate = currAfterstate;
			oldValue = newValue;

			game.addTile();
			gameOver = game.getGameOver();

		};

		tdTarget = 0.0f;
		delta = alpha * (tdTarget - oldValue);
		trainNetwork(prevAfterstate, delta);

		game.reset();
	};
};


int main() {
	if (doesFileExist("network.txt")) loadNetwork();
	else std::cout << "No existing network." << std::endl;
	playRounds(20000);
	saveNetwork();
	return 0;
};
	