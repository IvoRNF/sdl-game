#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <memory.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
      explicit Matrix4(glm::mat4 &mat4){
        float * matinput = glm::value_ptr(mat4); 
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
      Matrix4& operator*=(Matrix4&& right)
      {
        *this = Matrix4::Multiply(*this,right);
        return *this;
      }
      float * AsFloatPtr()  {
        return &mat[0][0];
      }
      static const Matrix4 Instance;
      static Matrix4 CreateSimpleViewProj(float width, float height)
      {
        float temp[4][4] =
        {
          { 2.0f/width, 0.0f, 0.0f, 0.0f },
          { 0.0f, 2.0f/height, 0.0f, 0.0f },
          { 0.0f, 0.0f, 1.0f, 0.0f },
          { 0.0f, 0.0f, 1.0f, 1.0f }
        };
        return Matrix4(temp);
      }
      static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
      {
        float yScale = 1 / tanf(fovY / 2.0f);
        float xScale = yScale * height / width;
        float temp[4][4] =
        {
          { xScale, 0.0f, 0.0f, 0.0f },
          { 0.0f, yScale, 0.0f, 0.0f },
          { 0.0f, 0.0f, far / (far - near), 1.0f },
          { 0.0f, 0.0f, -near * far / (far - near), 0.0f }
        };
        return Matrix4(temp);
      }
      void print();
      /*static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
      {
        Vector3 zaxis = Vector3::Normalize(target - eye);
        Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
        Vector3 yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
        Vector3 trans;
        trans.x = -Vector3::Dot(xaxis, eye);
        trans.y = -Vector3::Dot(yaxis, eye);
        trans.z = -Vector3::Dot(zaxis, eye);

        float temp[4][4] =
        {
          { xaxis.x, yaxis.x, zaxis.x, 0.0f },
          { xaxis.y, yaxis.y, zaxis.y, 0.0f },
          { xaxis.z, yaxis.z, zaxis.z, 0.0f },
          { trans.x, trans.y, trans.z, 1.0f }
        };
        return Matrix4(temp);
      }*/
};
#endif