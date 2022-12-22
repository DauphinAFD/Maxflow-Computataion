#include"Matrix.h"
using namespace std;

Matrix::Matrix(){
    grid = vector< vector<float> > (4,vector<float>(4,0));
    rows = grid.size();
    cols = grid[0].size();
}
Matrix::Matrix(vector<vector<float>> mat){
    grid = mat;
    rows = grid.size();
    cols = grid[0].size();
}
void Matrix::setGrid(vector<vector<float>> mat){
    grid = mat;
    rows = grid.size();
    cols = grid[0].size();
}
vector<vector<float>> Matrix::getGrid(){
    return grid;
} 
int Matrix::getRows(){
    return rows;
}
int Matrix::getCols(){
    return cols;
}

Matrix Matrix::add(Matrix A){
    vector<vector<float>> mat;
    vector<float> new_row;
    vector<vector<float>> mat1 = A.getGrid();
    for (int i=0; i<rows; i++){
        new_row.clear();
        for (int j=0; j<cols; j++){
            new_row.push_back(grid[i][j]+mat1[i][j]);
        }
        mat.push_back(new_row);
    }
    return Matrix(mat);
}

Matrix Matrix::T(){
    vector<vector<float>> mat;
    vector<float> new_row;
    
    for (int i=0; i<cols; i++){
        new_row.clear();
        for (int j=0; j<rows; j++){
            new_row.push_back(grid[j][i]);
        }
        mat.push_back(new_row);
    }
    return Matrix(mat);
}

void Matrix::Print(){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cout<<grid[i][j]<<" ";
        }
        cout<<endl;
    }
}