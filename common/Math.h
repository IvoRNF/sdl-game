#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <memory.h>

using namespace std;


enum VecAxis{X=0,Y=1,Z=2,None=-1};

class Matrix4 {
    public:
      float mat[4][4];
      Matrix4(){
        *this = Matrix4::Instance;
      }
      explicit Matrix4(float matinput[4][4]){
        memcpy(mat,matinput, 16 * sizeof(float));
      }
      static Matrix4 Identity();
      static Matrix4 RotateAxis(VecAxis axis,float degrees, Matrix4& mat4);
      static Matrix4 RotationMatrix(VecAxis axis, float degress);
      static Matrix4 Translate(VecAxis axis, float value,Matrix4& mat4);
      static Matrix4 Scale(float value,Matrix4& mat4);
      static Matrix4 Multiply(Matrix4& first, Matrix4& second);
      friend Matrix4 operator*(Matrix4& first, Matrix4& second){
        return Matrix4::Multiply(first,second);
      }
      Matrix4& operator*=(Matrix4& right)
      {
        *this = Matrix4::Multiply(*this,right);
        return *this;
      }
      float * AsFloatPtr()  {
        return &mat[0][0];
      }
      static const Matrix4 Instance;
};
#endif