#include "game.h"
#include <limits.h>

int const numSixTuples = int(pow(maxTileValue, 6));
int const numFourTuples = int(pow(maxTileValue, 4));
int const numTypes = 2;

int extractSixTuple(Grid grid, int type) {
	if (type < 0 || type > 1) {
		assert(false);
	}
	else {
		int value = 0;
		value = value * maxTileValue + grid(0, 0 + type);
		value = value * maxTileValue + grid(1, 0 + type);
		value = value * maxTileValue + grid(2, 0 + type);
		value = value * maxTileValue + grid(2, 1 + type);
		value = value * maxTileValue + grid(1, 1 + type);
		value = value * maxTileValue + grid(0, 1 + type);
		return value;
	}
}

int extractFourTuple(Grid grid, int type) {
	if (type < 0 || type > 1) {
		assert(false);
	}
	else {
		int value = 0;
		for (int i = 0; i < gridWidth; i++) {
			value = value * maxTileValue + grid(i, type);
		}
		return value;
	}
}

std::vector<std::vector<float> > lookUpTables = {std::vector<float>(numSixTuples, 0.0f), std::vector<float>(numSixTuples, 0.0f),
												std::vector<float>(numFourTuples, 0.0f), std::vector<float>(numFourTuples, 0.0f)};

// six tuples first then four tuples

float valueFunction(Grid grid){

	float totalValue = 0.0f;

	for (int k = 0; k < 2; k++) {
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < numTypes; i++) {
				totalValue += lookUpTables[i][extractSixTuple(grid, i)];
				totalValue += lookUpTables[i + numTypes][extractFourTuple(grid, i)];
			}
			grid.cwRotate90();
		}
		grid.hFlip();
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

	for (int k = 0; k < 2; k++) {
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < numTypes; i++) {
				lookUpTables[i][extractSixTuple(prevAfterstate, i)] += delta;
				lookUpTables[i + numTypes][extractFourTuple(prevAfterstate, i)] += delta;
			}
			prevAfterstate.cwRotate90();
		}
		prevAfterstate.hFlip();
	}
}
