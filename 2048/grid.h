#include "initialize.h"

class Grid {

	private:

		int values[gridSize]{};
		int score{};
		int numEmptyTiles{};
		int reward;

		bool swipeLeft() {
		//If possible, grid values updated to after swipe left move
		//Adds points earned from swipe left move to current score
		//No tile generation yet
		//Return: true/false, whether or not swipe left move possible
			int oldScore = score;
			bool possible = false;
			int currentCol{};
			int currentTile{};
			int mergeCol{};
			int newRow[gridWidth]{};
			for (int row = 0; row < (gridSize / gridWidth); row++) {
				currentCol = mergeCol = 0;
				std::fill(std::begin(newRow), std::end(newRow), emptyTile);
				for (int col = 0; col < gridWidth; col++) {
					currentTile = values[row * gridWidth + col];
					if (currentTile == emptyTile) {
						continue;
					}
					if (currentCol > mergeCol && currentTile == newRow[currentCol - 1]) {
						newRow[currentCol - 1] += 1;
						mergeCol = currentCol;
						score += ((1 << currentTile) & -2) * 2;
						possible = true;
						numEmptyTiles++;
					}
					else {
						if (currentCol != col) {
							possible = true;
						}
						newRow[currentCol++] = currentTile;
					}
				}
				for (int col = 0; col < gridWidth; col++) {
					values[row * gridWidth + col] = newRow[col];
				}
			}
			reward = score - oldScore;
			return possible;
		}

	public:

		Grid() {
		//Initializes to empty grid
			clear();
		}

		int operator[](int index) {
		//Parameters: 
		//	index - index of tile (int)
		//Return: tile value at index, error value when index out of range
			if (index < 0 || index > gridSize - 1) {
				assert(false);
				std::cout << "Index out of range!" << std::endl;
				return errorValue;
			}
			return values[index];
		}

		int operator()(int row, int col) {
		//Parameters: 
		//	row - row number of tile (int)
		//	col - column number of tile (int)
		//Return: tile value at (row,col), error value when index out of range
			int index = row * gridWidth + col;
			if (index < 0 || index > gridSize - 1) {
				assert(false);
				std::cout << "Index out of range!" << std::endl;
				return errorValue;
			}
			return values[index];
		}


		void clear() {
		//Sets all tiles to empty
		//Reset score to zero
			std::fill(std::begin(values), std::end(values), emptyTile);
			numEmptyTiles = gridSize;
			score = 0;
			reward = 0;
		}

		void copy(Grid& tempGrid) {
		//Copies contents of provided grid into this grid
		//Parameters: 
		//	tempGrid - grid to be copied
			if (this == &tempGrid)  return;
			memcpy(values, tempGrid.values, sizeof(values));
			score = tempGrid.score;
			numEmptyTiles = tempGrid.numEmptyTiles;
			reward = tempGrid.reward;
		}

		void setValue(int index, int value) {
		//Assigns value of specified tile
		//Updates number of empty tiles
		//Parameters: 
		//	index - index of tile (int)
		//  value - value to be assigned (int)
			if (index < 0 || index > gridSize - 1) {
				assert(false);
				std::cout << "Index out of range!" << std::endl;
				return;
			}
			if (values[index] != emptyTile && value == emptyTile) {
				numEmptyTiles--;
			}
			else if (values[index] == emptyTile && value != emptyTile) {
				numEmptyTiles++;
			}
			values[index] = value;
		}

		int getScore() const {
		//Return: current score
			return score;
		}

		int getNumEmptyTiles() const {
			//Return: number of empty tiles
			return numEmptyTiles;
		}

		int getReward() const {
			//Return: current score
			return reward;
		}

		int getMaxTile() const {
		//Return: value of tile with maximum value
			int maxValue = 0;
			for (int i = 0;i < gridSize;i++) {
				if (maxValue < values[i])
					maxValue = values[i];
			}
			return maxValue;
		}

		bool swipe(moveDir dir) {
		//Generalized version of swipeLeft method function
		//Parameters: 
		//	dir - direction of swipe (moveDir)
		//Return: true/false, whether or not move is possible
			bool possible{};
			assert(dir == moveDir::Up || dir == moveDir::Right ||
				dir == moveDir::Down || dir == moveDir::Left);
			switch (dir) {
				case moveDir::Left :
					possible = swipeLeft();
					break;
				case moveDir::Right :
					hFlip();
					possible = swipeLeft();
					hFlip();
					break;
				case moveDir::Up :
					ccwRotate90();
					possible = swipeLeft();
					cwRotate90();
					break;
				case moveDir::Down:
					cwRotate90();
					possible = swipeLeft();
					ccwRotate90();
					break;
			}
			return possible;
		}

		void makeNewTile() {
			//Adds a new normal or special tile on a random empty tile
			int counter = rand() % numEmptyTiles + 1;
			for (int i = 0; i < gridSize; i++) {
				if (values[i] == emptyTile) {
					counter--;
				}
				if (counter == 0) {
					values[i] = newTileValue();
					numEmptyTiles--;
					return;
				}
			}
		}

		void print() {
		//Displays grid on terminal
			std::cout << "-----------------" << std::endl;
			for (int i = 0; i < gridSize; i++) {
				if (i % gridWidth == 0) {
					std::cout << "| ";
				}
				if (values[i] != emptyTile) {
					std::cout << values[i] << " | ";
				}
				else {
					std::cout << "  | ";
				}
				if ((i + 1) % gridWidth == 0) {
					std::cout << std::endl << "-----------------" << std::endl;
				}
			}
			std::cout << std::endl;
		}

		void hFlip() {
			//Flips along vertical axis, effectively swapping left and right
			int tempValues[gridSize]{};
			for (int i = 0; i < gridSize; i++) {
				tempValues[((i / gridWidth + 1) * gridWidth - i % gridWidth - 1)] = values[i];
			}
			memcpy(values, tempValues, sizeof(values));
		}

		void cwRotate90() {
			//Rotates grid 90 degrees clockwise
			int tempValues[gridSize]{};
			for (int i = 0; i < gridSize; i++) {
				int row = i / gridWidth;
				int col = i % gridWidth;
				tempValues[col * gridWidth + gridSize / gridWidth - row - 1] = values[i];
			}
			memcpy(values, tempValues, sizeof(values));
		}

		void ccwRotate90() {
			//Rotates grid 90 degrees counterclockwise
			int tempValues[gridSize]{};
			for (int i = 0; i < gridSize; i++) {
				int row = i / gridWidth;
				int col = i % gridWidth;
				tempValues[(gridSize / gridWidth - col - 1) * gridWidth + row] = values[i];
			}
			memcpy(values, tempValues, sizeof(values));
		}

};


