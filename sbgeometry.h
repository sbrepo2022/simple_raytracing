#ifndef SBGEOMETRY_H
#define SBGEOMETRY_H
#include <string>
#include <cmath>
#include <QDebug>

bool isSimSign(float a, float b);

class SBVector2 {
private:
  float x, y;

public:
  SBVector2() {}
  SBVector2(float x, float y) {this->x = x; this->y = y;}
  void set(float x, float y) {this->x = x; this->y = y;}
  void setX(float x) {this->x = x;}
  void setY(float y) {this->y = y;}
  float getX() const {return x;}
  float getY() const {return y;}

  // vector operations
  SBVector2 add(const SBVector2 &ov) const;
  SBVector2 add(float scalar) const;
  SBVector2 sub(const SBVector2 &ov) const;
  SBVector2 sub(float scalar) const;
  SBVector2 mult(float scalar) const;
  SBVector2 div(float scalar) const;
};

class SBVector3 {
private:
  float x, y, z;

public:
  SBVector3() {}
  SBVector3(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
  void set(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
  void setX(float x) {this->x = x;}
  void setY(float y) {this->y = y;}
  void setZ(float z) {this->z = z;}
  float getX() const {return x;}
  float getY() const  {return y;}
  float getZ() const {return z;}

  // vector operations
  SBVector3 add(const SBVector3 &ov) const;
  SBVector3 add(float scalar) const;
  SBVector3 sub(const SBVector3 &ov) const;
  SBVector3 sub(float scalar) const;
  SBVector3 mult(float scalar) const;
  SBVector3 div(float scalar) const;
  SBVector3 cross(const SBVector3 &ov) const;
  float scalar(const SBVector3 &ov) const;
  float length() const;
  SBVector3 normalize() const;

  // vector operators
  SBVector3 operator+(const SBVector3 &ov) const {return this->add(ov);}
  SBVector3 operator+(float scalar) const {return this->add(scalar);}
  SBVector3 operator-(const SBVector3 &ov) const {return this->sub(ov);}
  SBVector3 operator-(float scalar) const {return this->sub(scalar);}
  SBVector3 operator*(float scalar) const {return this->mult(scalar);}
  SBVector3 operator/(float scalar) const {return this->div(scalar);}
  float operator*(const SBVector3 &ov) const {return this->scalar(ov);}
};

class SBLine {
private:
    SBVector2 p1, p2;

public:
    SBLine() {}
    SBLine(SBVector2 p1, SBVector2 p2) {this->p1 = p1; this->p2 = p2;}
    void set(SBVector2 p1, SBVector2 p2) {this->p1 = p1; this->p2 = p2;}
    void setP1(SBVector2 p1) {this->p1 = p1;}
    void setP2(SBVector2 p2) {this->p2 = p2;}
    SBVector2 getP1() {return p1;}
    SBVector2 getP2() {return p2;}
    void setPoints(SBVector2 p1, SBVector2 p2) {this->p1 = p1; this->p2 = p2;}

    SBVector3 lineEquation() const;
    SBVector2 lineIntersect(const SBLine &ol) const;
    int isSegmentIntersect(const SBLine &ol) const;
};

class SBMatrix {
private:
    float **matrix;
    int width, height;

public:
    SBMatrix() {matrix = nullptr;}
    SBMatrix(int size) {matrix = nullptr; create(size);}
    SBMatrix(int width, int height) {matrix = nullptr; create(width, height);}
    SBMatrix(const SBMatrix &obj) {this->copy(obj);}
    ~SBMatrix() {destroy();}

    void create(int width, int height);
    void create(int size) {create(size, size);}
    void copy(const SBMatrix &obj);
    void destroy();
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    void setVal(int x, int y, float val) {matrix[y][x] = val;}
    float getVal(int x, int y) const {return matrix[y][x];}
    // matrix values
    float det() const;                          // matrix determinant
    float add(int x, int y) const;              // algebraic addition
    float minor(int x, int y) const;            // minor

    // matrix operations
    SBMatrix mult(float scalar) const;          // scalar multiply
    SBMatrix mult(const SBMatrix &om) const;    // matrix multiply
    SBMatrix reverse() const;                   // reverse matrix
    SBMatrix uni() const;                       // union matrix
    SBMatrix transp() const;                    // transposed matrix

    // operators
    SBMatrix & operator=(const SBMatrix &obj) {this->copy(obj); return *this;}

    // debug
    std::string toString() const;
};

#endif // SBGEOMETRY_H
