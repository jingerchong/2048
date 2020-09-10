#include "grid.h"

class Game {

	private:

		Grid grid;
		bool gameOver{};
		int totNumRounds{};
		int currNumRounds{};
		int maxScore{};
		int totalScore{};
		int numMoves{};
		int maxTile{};
		int numWins{};
		int maxTileArray[maxTileValue + 1]{};
		std::clock_t startTime{};
		std::clock_t endTime{};
		
		void setGameOver() {
		//Changes game over status to true if no possible moves remain
			if (grid.getNumEmptyTiles() > 0) {
				gameOver = false;
				return;
			}
			for (int row = 0; row < (gridSize / gridWidth); row++) {
				for (int col = 0; col < gridWidth; col++) {
					if ((row < gridSize / gridWidth - 1) && grid(row, col) == grid(row + 1, col)) {
						gameOver = false;
						return;
					}
					if ((col < gridWidth - 1) && grid(row, col) == grid(row, col + 1)) {
						gameOver = false;
						return;
					}
				}
			}
			gameOver = true;
		}

		void updateStats() {
		//Updates total score, max score, and max tile value
		//Called at the end of each round
		//Called at the end of each round
			totNumRounds++;
			currNumRounds++;
			int currScore = grid.getScore();
			totalScore += currScore;
			if (currScore > maxScore) {
				maxScore = currScore;
			}
			int currMaxTile = grid.getMaxTile();
			if (currMaxTile > maxTile)
				maxTile = currMaxTile;
			if (currMaxTile >= winTile)
				numWins++;
			endTime = std::clock();
			maxTileArray[currMaxTile] ++;
			if (totNumRounds % 1000 == 0) {
				dumpStats();
				currNumRounds = 0;
				maxScore = 0;
				totalScore = 0;
				numMoves = 0;
				maxTile = 0;
				numWins = 0;
				std::fill(std::begin(maxTileArray), std::end(maxTileArray), 0);
				startTime = std::clock();
			}
		}

	public:

		Game() {
			gameOver = false;
			maxScore = 0;
			totalScore = 0;
			numMoves = 0;
			maxTile = 0;
			numWins = 0;
			totNumRounds = 0;
			currNumRounds = 0;
			std::fill(std::begin(maxTileArray), std::end(maxTileArray), 0);
			for (int i = 0; i < numInitialTiles; i++) {
				grid.makeNewTile();
			}
			startTime = std::clock();
		}

		void reset() {
			updateStats();
			grid.clear();
			gameOver = false;
			for (int i = 0; i < numInitialTiles; i++) {
				grid.makeNewTile();
			}
		}

		bool getGameOver() const {
		//Returns true if game over, false otherwise
			return gameOver;
		}

		int getScore() const {
		//Returns current score of grid
			return grid.getScore();
		}

		void copyGrid(Grid& currentGrid) {
		//Copy contents of game grid into given grid
			currentGrid.copy(grid);
		}

		int getMaxScore() const {
		//Returns current max score reached
			return maxScore;
		}

		bool makeMove(moveDir dir) {
			if (grid.swipe(dir)) {
				numMoves++;
				return true;
			}
			else {
				return false;
			}
		}

		void addTile() {
			grid.makeNewTile();
			setGameOver();
		}

		void printGrid() {
		//Prints current state of grid
			grid.print();
		}

		void dumpStats() {
		//Saves current stats to file log.txt
			std::ofstream log;
			log.open("log.txt", std::ios_base::app);
			log << "Rounds: " << totNumRounds << std::endl;
			log << "Ave score: " << totalScore / currNumRounds << std::endl;
			log << "Max score: " << maxScore << std::endl;
			log << "Max tile: " << ((1 << maxTile) & -2) << std::endl;
			log << "% Max tiles: ";
			for (int i = 0; i < maxTileValue + 1; i++) {
				log << (float)maxTileArray[i] / (float)currNumRounds << " ";
			}
			log << std::endl;
			clock_t t = endTime - startTime;
			log << "Moves per sec: " << (double)numMoves / (((double)t) / CLOCKS_PER_SEC) << std::endl;
			log << "Win rate: " << (float)numWins / (float)currNumRounds << std::endl << std::endl;
			log.close();
			std::cout << "Rounds: " << totNumRounds << std::endl;

		}
		
};
