#include <sstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>

#include "BTC.h"

template<typename T>
class Matrix {
    std::vector<T> data;
    size_t row;
    size_t col;

    Matrix() : data(), row(0), col(0) {};

  public:
    Matrix(size_t rows, size_t cols) : data(rows*cols), row(rows), col(cols) {}

    size_t getRowDim() const {
        return row;
    }

    size_t getColDim() const {
        return col;
    }

    const T& get(size_t i, size_t j) const {
        return data[i+row*j];
    }

    void set(const T& val, size_t i, size_t j) {
        data[i+row*j] = val;
    }

    T* getDataPtr() {
        return data.data();
    }

    const T* getDataPtr() const {
        return data.data();
    }

    void print(std::ostream& os) {
        for (size_t i=0; i<row; ++i) {
            for (size_t j=0; j<col; ++j) {
                os << std::setprecision(16) << get(i,j) << ' ';
            }
            os << '\n';
        }
        os << '\n';
    }
};

void serializeMatrix(std::ostream& os, const Matrix<float>& matrix) {
    BTC::BTagCompoundPtr serializer(new BTC::BTagCompound());
    // Set row- and col-dimension
    serializer->setInt("row",matrix.getRowDim());
    serializer->setInt("col",matrix.getRowDim());
    // Just set the array as the ownership remains with the std::vector!
    serializer->setFloatArray("data",matrix.getDataPtr(),
                               matrix.getRowDim()*matrix.getColDim());
    // Serialize to the outstream
    serializer->serialize(os);
}

Matrix<float> deserializeMatrix(std::istream& is) {
    BTC::BTagCompoundPtr serializer(new BTC::BTagCompound());
    serializer->deserialize(is);
    // Create new matrix
    Matrix<float> result(serializer->getValue<BTC::UINT32_T>("row"),
                          serializer->getValue<BTC::UINT32_T>("col"));
    BTC::SIZE_T len;
    float* data = result.getDataPtr();
    BTC::FLOAT_T* deserialized_data = serializer->getArray<BTC::FLOAT_T>("data",len);
    for (size_t i=0; i<len; ++i) {
        data[i] = deserialized_data[i];
    }
    return result;
}

int main() {
    // Set up matrix
    Matrix<float> mat(10,10);
    float* dataptr = mat.getDataPtr();
    for (size_t i=0, len=10*10; i<len; ++i) {
        dataptr[i] = (float(rand())/float(RAND_MAX)-0.5);
    }
    std::cout << "Original matrix" << std::endl;
    mat.print(std::cout);
    // Serialize the matrix to stream
    std::stringstream ss;
    serializeMatrix(ss,mat);
    // Deserialize the matrix from stream
    Matrix<float> new_mat = deserializeMatrix(ss);
    std::cout << "Deserialized matrix" << std::endl;
    new_mat.print(std::cout);

    return 0;
}

