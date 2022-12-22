#ifndef MATRIX_H
#define MATRIX_H

#include<vector>
#include<iostream>
#include <stdexcept>
class Matrix{
    private :
        std::vector< std::vector<float> > grid;
        int rows;
        int cols;
    public :
        //constructor
        Matrix();
        Matrix(std::vector< std::vector<float> >);
        
        //set and get Functions
        void setGrid(std::vector< std::vector<float> >);
        
        std::vector< std::vector<float> >getGrid();
        int getRows();
        int getCols();

        // Matrix Functions Declarations
        Matrix add(Matrix);
        Matrix T();
        void Print();
};

#endif /*MATRIX_H*/
