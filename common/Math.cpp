#include "./Math.h"
#include <cmath>
using namespace std;

const float PI = 3.14159;

Matrix4 Matrix4::Identity()
{
    float result[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};
    return Matrix4(result);
}

void Matrix4::print(){
     //std::cout << "\x1B[2J\x1B[H";
    for(int i = 0 ; i < 4 ; i++){
       for(int j = 0 ; j < 4 ; j++){
           
           printf(" %f ",this->mat[i][j]);   
       } 
       std::cout << std::endl;
    }
}
Matrix4 Matrix4::Multiply(Matrix4& first, Matrix4& second){
    auto result = Matrix4::Identity();  
    auto sumRowByCol = [&](int row,int col){
       float sum = 0.0f;
       for(int i = 0 ; i < 4 ;i++){
         sum += first.mat[row][i] * second.mat[i][col];
       }
       return sum;
    };
    for(int i = 0 ; i < 4 ; i++){
       for(int j = 0 ; j < 4 ; j++){
           float v = sumRowByCol(i,j);
           result.mat[i][j] = v;   
       } 
    }
    return result;
}

Matrix4 Matrix4::RotateAxis(VecAxis axis, float degrees, Matrix4 &mat4)
{
    auto result = mat4.mat;
    float radians = degrees * PI / 180;
    if (axis == Y)
    {
        result[0][0] = std::cos(radians);
        result[0][2] = -std::sin(radians);
        result[2][0] = std::sin(radians);
        result[2][2] = std::cos(radians);
    }
    if (axis == X)
    {
        result[1][1] = std::cos(radians);
        result[1][2] = std::sin(radians);
        result[2][1] = -std::sin(radians);
        result[2][2] = std::cos(radians);
    }

    if (axis == Z)
    {
        result[0][0] = std::cos(radians);
        result[0][1] = std::sin(radians);
        result[1][0] = -std::sin(radians);
        result[1][1] = std::cos(radians);
    }
    return mat4;
}



Matrix4 Matrix4::RotationMatrix(VecAxis axis, float degrees){
   Matrix4 result = Matrix4::Identity();
   Matrix4::RotateAxis(axis,degrees,result);
   return result;
}
Matrix4 Matrix4::Translate(VecAxis axis, float value, Matrix4 &mat4)
{
    auto result = mat4.mat;
    if (axis == Y)
    {
        result[3][1] += value;
    }
    if (axis == X)
    {
        result[3][0] += value;
    }

    if (axis == Z)
    {
        result[3][2] += value;
    }
    return mat4;
}
Matrix4 Matrix4::Scale(float value, Matrix4 &mat4)
{
    auto result = mat4.mat;
    result[0][0] *= value;
    result[1][1] *= value;
    result[2][2] *= value;
    return mat4;
}

