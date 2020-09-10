#include "game.h"
#include <limits.h>

int const numTupleValues = (1 << (maxTileValue + 1)) & -2;
int const numTuples = 17;
int const numBoxTuples = (gridSize / gridWidth - 1) * (gridWidth - 1);


int extractRowTuple(Grid grid, int row) {
	if (row < 0 || row > gridSize / gridWidth) {
		assert(false);
	}
	else {
		int rowTuple = 0;
		for (int i = 0; i < gridWidth; i++) {
			rowTuple = rowTuple* 16 + grid(row, i);
		}
		return rowTuple;
	}
}

int extractColTuple(Grid grid, int col) {
	if (col < 0 || col > gridWidth) {
		assert(false);
	}
	else {
		int colTuple = 0;
		for (int i = 0; i < gridWidth; i++) {
			colTuple = colTuple * 16 + grid(i, col);
		}
		return colTuple;
	}
}

int extractBoxTuple(Grid grid, int box) {
	if (box < 0 || box > numBoxTuples) {
		assert(false);
	}
	else {
		int boxTuple = 0;
		int boxRow = box / (gridSize / gridWidth - 1);
		int boxCol = box % (gridWidth - 1);
		boxTuple = boxTuple * 16 + grid(boxRow, boxCol);
		boxTuple = boxTuple * 16 + grid(boxRow, boxCol + 1);
		boxTuple = boxTuple * 16 + grid(boxRow + 1, boxCol);
		boxTuple = boxTuple * 16 + grid(boxRow + 1, boxCol + 1);
		return boxTuple;
	}
}

std::vector<std::vector<float> > lookUpTables(numTuples, std::vector<float>(numTupleValues, 0.0f));

float valueFunction(Grid grid){

	float totalValue = 0.0f;

	for (int row = 0; row < gridSize / gridWidth; row++) {
		totalValue += lookUpTables[row][extractRowTuple(grid, row)];
	}
	for (int col = 0; col < gridWidth; col++) {
		totalValue += lookUpTables[gridSize / gridWidth + col][extractColTuple(grid, col)];
	}
	for (int box = 0; box < numBoxTuples; box++) {
		totalValue += lookUpTables[gridSize / gridWidth + gridWidth + box][extractBoxTuple(grid, box)];
	}
	return totalValue;
}

moveDir nextMove(Grid grid) {
	//does not change grid, tries all directions and returns one with highest evaluate

	Grid temp;
	moveDir bestMove = moveDir::Default;
	float maxValue = -INFINITY;

	for (int i = 0; i < 4; i++) {
		temp.clear();
		temp.copy(grid);
		moveDir dir = (moveDir)i;
		if (temp.swipe(dir)) {
			float newValue = ((float)temp.getReward() + valueFunction(temp));
			if (newValue > maxValue) {
				maxValue = newValue;
				bestMove = dir;
			}
		}
	}
	return bestMove;
}

void trainNetwork(Grid prevAfterstate, float delta) {
	for (int row = 0; row < gridSize / gridWidth; row++) {
		lookUpTables[row][extractRowTuple(prevAfterstate, row)] += delta;
	}
	for (int col = 0; col < gridWidth; col++) {
		lookUpTables[gridSize / gridWidth + col][extractColTuple(prevAfterstate, col)] += delta;
	}
	for (int box = 0; box < numBoxTuples; box++) {
		lookUpTables[gridSize / gridWidth + gridWidth + box][extractBoxTuple(prevAfterstate, box)] += delta;
	}
}