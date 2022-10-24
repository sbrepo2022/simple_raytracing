#include "sbgeometry.h"

bool isSimSign(float a, float b) {
    if ((a < 0.0f && b < 0.0f) || (a > 0.0f && b > 0.0f))
        return true;
    else
        return false;
}

SBVector2 SBVector2::add(const SBVector2 &ov) const {
    return SBVector2(this->getX() + ov.getX(), this->getY() + ov.getY());
}

SBVector2 SBVector2::add(float scalar) const {
    return SBVector2(this->getX() + scalar, this->getY() + scalar);
}

SBVector2 SBVector2::sub(const SBVector2 &ov) const {
    return SBVector2(this->getX() - ov.getX(), this->getY() - ov.getY());
}

SBVector2 SBVector2::sub(float scalar) const {
    return SBVector2(this->getX() - scalar, this->getY() - scalar);
}

SBVector2 SBVector2::mult(float scalar) const {
    return SBVector2(this->getX() * scalar, this->getY() * scalar);
}

SBVector2 SBVector2::div(float scalar) const {
    return SBVector2(this->getX() / scalar, this->getY() / scalar);
}

SBVector3 SBVector3::add(const SBVector3 &ov) const {
    return SBVector3(this->getX() + ov.getX(), this->getY() + ov.getY(), this->getZ() + ov.getZ());
}

SBVector3 SBVector3::add(float scalar) const {
    return SBVector3(this->getX() + scalar, this->getY() + scalar, this->getZ() + scalar);
}

SBVector3 SBVector3::sub(const SBVector3 &ov) const {
    return SBVector3(this->getX() - ov.getX(), this->getY() - ov.getY(), this->getZ() - ov.getZ());
}

SBVector3 SBVector3::sub(float scalar) const {
    return SBVector3(this->getX() - scalar, this->getY() - scalar, this->getZ() - scalar);
}

SBVector3 SBVector3::mult(float scalar) const {
    return SBVector3(this->getX() * scalar, this->getY() * scalar, this->getZ() * scalar);
}

SBVector3 SBVector3::div(float scalar) const {
    return SBVector3(this->getX() / scalar, this->getY() / scalar, this->getZ() / scalar);
}

SBVector3 SBVector3::cross(const SBVector3 &ov) const {
    return SBVector3(this->getY() * ov.getZ() - ov.getY() * this->getZ(), this->getX() * ov.getZ() - ov.getX() * this->getZ(), this->getX() * ov.getY() - ov.getX() * this->getY());
}

float SBVector3::scalar(const SBVector3 &ov) const {
    return this->getX() * ov.getX() + this->getY() * ov.getY() + this->getZ() * ov.getZ();
}

float SBVector3::length() const {
    return sqrtf(this->getX() * this->getX() + this->getY() * this->getY() + this->getZ() * this->getZ());
}

SBVector3 SBVector3::normalize() const {
    float l = this->length();
    return SBVector3(this->getX() / l, this->getY() / l, this->getZ() / l);
}

SBVector3 SBLine::lineEquation() const {
    return SBVector3(p1.getY() - p2.getY(), p2.getX() - p1.getX(), p1.getX() * p2.getY() - p2.getX() * p1.getY());
}

SBVector2 SBLine::lineIntersect(const SBLine &ol) const {
    SBVector3 le1 = this->lineEquation();
    SBVector3 le2 = ol.lineEquation();

    SBMatrix A(2, 2);
    A.setVal(0, 0, le1.getX());
    A.setVal(1, 0, le1.getY());
    A.setVal(0, 1, le2.getX());
    A.setVal(1, 1, le2.getY());

    SBMatrix B(1, 2);
    B.setVal(0, 0, -1 * le1.getZ());
    B.setVal(0, 1, -1 * le2.getZ());

    SBMatrix C = A.reverse().mult(B);
    return SBVector2(C.getVal(0, 0), C.getVal(0, 1));
}

int SBLine::isSegmentIntersect(const SBLine &ol) const {
    SBVector2 vec2l1 = this->p2.sub(this->p1);
    SBVector2 vec2left1 = ol.p1.sub(this->p1);
    SBVector2 vec2right1 = ol.p2.sub(this->p1);
    SBVector2 vec2l2 = ol.p2.sub(ol.p1);
    SBVector2 vec2left2 = this->p2.sub(ol.p1);
    SBVector2 vec2right2 = this->p1.sub(ol.p1);

    SBVector3 vec3l1(vec2l1.getX(), vec2l1.getY(), 0);
    SBVector3 vec3left1(vec2left1.getX(), vec2left1.getY(), 0);
    SBVector3 vec3right1(vec2right1.getX(), vec2right1.getY(), 0);
    SBVector3 vec3l2(vec2l2.getX(), vec2l2.getY(), 0);
    SBVector3 vec3left2(vec2left2.getX(), vec2left2.getY(), 0);
    SBVector3 vec3right2(vec2right2.getX(), vec2right2.getY(), 0);

    if (vec3l1.cross(vec3left1).getZ() == 0.0f && vec3l1.cross(vec3right1).getZ() == 0.0f) {
        return 2;
    }
    else if (! isSimSign(vec3l1.cross(vec3left1).getZ(), vec3l1.cross(vec3right1).getZ()) && ! isSimSign(vec3l2.cross(vec3left2).getZ(), vec3l2.cross(vec3right2).getZ())) {
        return 1;
    }
    else {
        return 0;
    }
}

void SBMatrix::create(int width, int height) {
    if (matrix != nullptr)
        destroy();

    this->width = width;
    this->height = height;
    matrix = new float*[height];
    for (int y = 0; y < height; y++) {
        matrix[y] = new float[width];
    }
}

void SBMatrix::destroy() {
    for (int y = 0; y < height; y++) {
        delete [] matrix[y];
    }
    delete [] matrix;
}

void SBMatrix::copy(const SBMatrix &obj) {
    this->width = obj.width;
    this->height = obj.height;
    this->create(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            this->matrix[y][x] = obj.getVal(x, y);
        }
    }
}

float SBMatrix::det() const {
    if (width == 1 && height == 1) {
        return matrix[0][0];
    }
    else {
        float sum = 0.0f;
        for (int i = 0; i < width; i++) {
            sum += matrix[0][i] * this->add(i, 0);
        }
        return sum;
    }
}

float SBMatrix::add(int x, int y) const {
    if ((x + y) % 2 == 0) {
        return minor(x, y);
    }
    else {
        return  -1 * minor(x, y);
    }
}

float SBMatrix::minor(int x, int y) const {
    int xPos = 0, yPos = 0;
    SBMatrix mMatrix(width - 1, height - 1);
    for (int j = 0; j < height; j++) {
        if (j == y) {
            continue;
        }
        for (int i = 0; i < width; i++) {
            if (i == x) {
                continue;
            }
            mMatrix.setVal(xPos, yPos, this->getVal(i, j));
            xPos++;
        }
        xPos = 0;
        yPos++;
    }
    yPos = 0;
    return mMatrix.det();
}

SBMatrix SBMatrix::mult(float scalar) const {
    SBMatrix nMatrix(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nMatrix.setVal(x, y, this->getVal(x, y) * scalar);
        }
    }
    return nMatrix;
}

SBMatrix SBMatrix::mult(const SBMatrix &om) const {
    SBMatrix nMatrix(om.getWidth(), this->getHeight());
    float sum = 0.0f;
    int max = 0;
    this->getWidth() < om.getHeight() ? max = this->getWidth() : max = om.getHeight();
    for (int y = 0; y < nMatrix.getHeight(); y++) {
        for (int x = 0; x < nMatrix.getWidth(); x++) {
            for (int m = 0; m < max; m++) {
                sum += this->getVal(m, y) * om.getVal(x, m);
            }
            nMatrix.setVal(x, y, sum);
            sum = 0;
        }
    }
    return nMatrix;
}

SBMatrix SBMatrix::reverse() const {
    SBMatrix nMatrix(width, height);
    float dt = this->det();
    if (dt != 0.0f) {
        nMatrix = this->uni().mult((1.0f / dt));
    }
    return nMatrix;
}

SBMatrix SBMatrix::uni() const {
    SBMatrix nMatrix(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nMatrix.setVal(x, y, this->add(x, y));
        }
    }
    return nMatrix.transp();
}

SBMatrix SBMatrix::transp() const {
    SBMatrix nMatrix(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nMatrix.setVal(x, y, this->getVal(y, x));
        }
    }
    return nMatrix;
}

std::string SBMatrix::toString() const {
    std::string data;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            data += std::to_string(matrix[y][x]);
            data += " ";
        }
        data += "\n";
    }
    return data;
}
