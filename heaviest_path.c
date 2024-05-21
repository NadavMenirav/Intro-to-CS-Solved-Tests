#include <stdio.h>

typedef enum { false, true } boolean;

int heaviest_path(unsigned int** matrix, int height, int width);
int heaviest_path_help(unsigned int** matrix, int height, int width, int rowIndex, int colIndex, int sum);

int main() {
	return 0;
}

int heaviest_path(unsigned int** matrix, int height, int width) {
	heaviest_path_help(matrix, height, width, 0, 0, matrix[0,0]);
}

int heaviest_path_help(unsigned int** matrix, int height, int width, int rowIndex, int colIndex, int sum) {
	boolean isDownLegal = false, isRightLegal = false;
	if ((rowIndex == height - 1) && (colIndex == width - 1))
		return sum;

	int down = heaviest_path_help(matrix, height, width, rowIndex + 1, colIndex, sum + matrix[rowIndex + 1, colIndex]);
	int	right = heaviest_path_help(matrix, height, width, rowIndex, colIndex + 1, sum + matrix[rowIndex, colIndex + 1]);

	if (matrix[rowIndex, colIndex] >= matrix[rowIndex + 1, colIndex]) //moving down is illegal		
		isDownLegal = true;

	if (matrix[rowIndex, colIndex] >= matrix[rowIndex, colIndex + 1])
		isRightLegal = true;

	if (!isDownLegal && isRightLegal)
		return right;
	else if (isDownLegal && !isRightLegal)
		return down;

	else {
		if (down >= right)
			return down;
		else
			return right;
	}
}
