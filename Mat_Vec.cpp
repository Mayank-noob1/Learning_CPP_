#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

// Vector class.
template <typename T>
class Vec{
private:
int len;     //Current length.
bool type;  //True -> Row vector, False -> Column vector.
std::vector<T> vector;     //Field Elements.
public:
Vec(int len, const std::vector<T> &arr){
    if (len < 1){
        throw(std::invalid_argument("Definition of Vector violated. Atleast one field is required."));
    }
    if (arr.size() == len){
        this->len = len;
        this->type = true;  //Default set to row vector.
        this->vector = *(new std::vector<T>(arr));
    }
    else{
    throw(std::invalid_argument("Error."));
    }
}
Vec(int len){
    if (len < 1){
        throw(std::invalid_argument("Definition of Vector violated. Atleast one field is required."));
    }
    // Generates null vector.
    std::vector<T> vect;
    for (int i = 0;i < len;i++){
        vect.push_back(T(0));
    }
    this->len = len;
    this->type = true;
    (this->vector) = *(new std::vector<T>(vect));
    // std::cout<<this->vector[0]<<std::endl;
}
Vec(const std::vector<T> &arr){
    if (arr.size() < 1){
        throw(std::invalid_argument("Definition of Vector violated. Atleast one field is required."));
    }
    // Vector of field elements. No length constraints.
    this->len = arr.size();
    this->type = true;  //Default set to row vector.
    this->vector = *(new std::vector<T>(arr));
}
Vec(const Vec<T> &vec){
    this->len = vec.len;
    this->type = vec.type;
    this->vector = *(new std::vector<T>(vec.vector));
    for (int i = 0;i<vec.len;i++){
        this->vector.push_back(vec.vector[i]);
    }
}
// Copy Constructor
// Vec(const Vec &vect):len(vect.len),type(vect.type),vector(vect.vector){};
// ~Vec(){
//     delete(&(this->len));
//     delete(&(this->type));
//     delete(&(this->vector));        //Not sure about this. Will it delete the vector or to delete individual element.
// }
Vec &operator+=(const Vec<T> &right){
    if (this->len != right.len){
        throw(std::invalid_argument("Vector lies in different dimensions."));
    }
    for (int i = 0; i< this->len;i++){
        this->vector[i] += right.vector[i];
    }
    return (*this);
};
Vec &operator-=(const Vec<T> &right){
    if (this->len != right.len){
        throw(std::invalid_argument("Vector lies in different dimensions."));
    }
    for (int i = 0; i< this->len;i++){
        this->vector[i] -= right.vector[i];
    }
    return (*this);
    }
Vec &operator/=(double num){
    for (int i = 0; i< this->len;i++){
        this->vector[i] /= num;
    }
    return (*this);
    }
double operator*(const Vec<T> &right){
    // Dot product.
    if (this->len != right.len){
        throw(std::invalid_argument("Vector dimensional error."));
    }
    double acc;
    for (int i = 0; i< this->len;i++){
        acc += this->vector[i] * right.vector[i];
    }
    return acc;
    }
Vec &operator*=(double scalar){
    // Scaling vector.
    for (int i = 0; i< this->len;i++){
        this->vector[i] *=  scalar;
    }
    return (*this);
    }
T &operator[](int i) {
    return this->vector[i];
}
Vec &assign(int i,double data){
    this->vector[i] = data;
    return *this;
}
void Transpose(){
    // Row -> Column or Column -> Row.
    this->type ^= true;
}
int get_length() const{
    return this->len;
}
bool is_row() const{
    return this->type;
}
bool is_col() const{
    return !(this->type);
}
};
template <typename T>
Vec<T> &operator+(const Vec<T> &left, const Vec<T> &right){
    Vec<T>* newVec = new Vec<T>(left);
    return (*newVec += right);
}
template <typename T>
Vec<T> &operator-(const Vec<T> &left, const Vec<T> &right){
    Vec<T>* newVec = new Vec<T>(left);
    return (*newVec -= right);
}
template <typename T>
Vec<T> &operator*(const Vec<T> &left, const double scalar){
    Vec<T>* newVec = new Vec<T>(left);
    *newVec *= scalar;
    return *newVec;
}
template <typename T>
std::ostream& operator<<(std::ostream&os,Vec<T> vector){
    // Output
    for (int i =0;i<vector.get_length();i++)
    os<<vector[i]<<" ";
    os<<'\n';
    return os;
};
// Cross Product not implemented as it requires the information of basis vectors or the next dimension.

template <typename T>
class Matrix        // n X m
{
private:
    int n;
    int m;
    int swaps;
    double det;
    bool type; //True -> Row space, False -> Column space.
    std::vector<Vec<T>*>matrix;

    void Swap(int i,int j){
            Vec<T>* temp = (*this).matrix[j];
            (*this).matrix[j] = (*this).matrix[i];
            (*this).matrix[i] = temp;
        return;
    }
    void pivot(Matrix& I,bool determ){
        int cnt=0;
        this->swaps = 0;
        for (int i = 0; i < this->n; i++) {
            int pivot_row = i;
            for (int j = i+1; j < this->n; j++) {
                if (abs((*this)[j][i]) > abs((*this)[pivot_row][i])) {
                    pivot_row = j;
                }
            }
            if (pivot_row != i) {
                this->Swap(i,pivot_row);
                I.Swap(i,pivot_row);
                this->swaps++;
            }
            for (int j = i+1; j < this->n; j++) {
                if ((*this)[i][i] == 0){
                    throw(std::invalid_argument("Not reducible, Rank issue."));
                }
                double factor = (*this)[j][i] / (*this)[i][i];
                for (int k = 0; k < n; k++) {
                    double temp = (*this)[j][k] - factor * (*this)[i][k];
                    (*this)[j][k] = temp;
                    temp = I[j][k] - factor * I[i][k];
                    I[j][k] = temp;
                }
            }
        }
        if (determ){
        this->det = Product(*this)*((this->swaps % 2) ? -1: 1);
        return;
        }
        for (int i=0; i < this->n;i++){
            if ((*this)[i][i] == 0){
                throw(std::invalid_argument("Not reducible, Rank issue."));
            }
            if ((*this)[i][i] != 1){
                double factor = 1 / (*(this->matrix[i]))[i];
                for (int h = 0; h < this->m; h++)
                {
                    double temp = ((*this)[i])[h] * factor;
                    (*this)[i][h] = temp;
                    temp = I[i][h] * factor;
                    I[i][h] = temp;
                }
            }
        }
        for(int i =this->n-1;i >= 0;i--){
            for (int j=i-1;j >=0; j--){
                double factor = (*this)[j][i] / (*this)[i][i];
                for (int k = this->n-1; k >=0 ; k--) {
                    if (factor != 0){
                    double temp = I[j][k] - factor * I[i][k];
                    I[j][k] = temp;
                    }
                }        
                (*this)[j][i]  = 0;
            }
        }
    }
    Matrix<T> back_substitute(bool determ=true){
        Matrix<T> Identity = Matrix<T>(this->n,this->m,true);
        this->pivot(Identity,determ);
        return Identity;
    }

public:
Matrix<T>(const int &n, const int &m,bool iden = false)
    {
        if (n < 1 || m < 1){
            throw(std::invalid_argument("Definition of Matrix violated. Atleast one dimension is undefined."));
        }
        // Null matrix. Default Row space.
        this->n = n;
        this->m = m;
        this->type = true;
        for (int i = 0; i < this->n; i++)
        {
            Vec<T>* vector = new Vec<T>(m);
            this->matrix.push_back(vector);
        }
        if(iden){
            // Identity matrix.
            for (int i = 0;i < this->n;i++){
                (*this)[i][i]= 1;
            }
        }
    }
Matrix<T>(const int &n, const int &m, const std::vector<double> &arr)
    {
        if (n < 1 || m < 1){
            throw(std::invalid_argument("Definition of Matrix violated. Atleast one dimension is undefined."));
        }
        // Creates a matrix based on array of data. Default row space.
        if (arr.size() < n*m){
            throw(std::invalid_argument("Less field elements given"));
        }
        this->n = n;
        this->m = m;
        this->type = true;
        for (int i = 0; i < n; i++)
        {
            std::vector <T> dummy;
            for (int j = 0; j < m; j++)
            {
                dummy.push_back(arr[i*m + j]);
            }
            Vec<T>* vector= new Vec<T>(m,dummy);
            this->matrix.push_back(vector);
        }
    }
// void Add_vec(const Vec &arr,bool where=true){
//     if (where){     //Row addition.
//         if (arr.get_length() != this->m){
//             throw(std::invalid_argument("Dimensional error. Vector and matrix not suitable."));
//         }
//         else{
//             Vec* newVec = new Vec(arr);
//             this->n++;
//             this->matrix.push_back(newVec);
//             if (newVec->is_col()){
//                 newVec->Transpose();
//             }
//         }
//     }
//     else{       //Column addition.
//         if (arr.get_length() != this->n){
//             throw(std::invalid_argument("Dimensional error. Vector and matrix not suitable."));
//         }
//         else{
//             Vec* newVec = new Vec(arr);
//             this->n++;
//             this->matrix.push_back(newVec);
//             if (newVec->is_row()){
//                 newVec->Transpose();
//             }
//         }
//     }
// }
// Copy constructor
Matrix<T>(const Matrix &mat){
    this->n = mat.n;
    this->m = mat.m;
    for (int i = 0; i < this->n; i++){
        Vec<T>* sub = new Vec<T>(*(mat.matrix[i]));
        this->matrix.push_back(sub);
    }
}
// ~Matrix(){
//     for (int i =0;i< this->n;i++){delete(this->matrix[i]);}
//     delete(&matrix);
//     delete(&n);
//     delete(&m);
// };
Vec<T> &operator[](int i){
    return *(this->matrix[i]);
}
Matrix<T> &operator+=(const Matrix &right)
    {
        if ((this->n == right.n) && (this->m == right.m))
        {
            for (int i = 0; i < this->n; i++)
            {
                *(this->matrix[i]) += (*right.matrix[i]);
            }
            return *this;
        }
        throw(std::invalid_argument("Matrix are not compatible."));
    }
Matrix<T> &operator-=(const Matrix &right)
{
            std::cout<<"\n\n";
    if ((this->n == right.n) && (this->m == right.m))
    {
        for (int i = 0; i < this->n; i++)
        {
            *(this->matrix[i]) -= (*right.matrix[i]);
        }
        return *this;
    }
    throw(std::invalid_argument("Matrix are not compatible."));
}
Matrix<T> &operator/=(double &num)
    {
        for (int i = 0; i < this->n; i++)
        {
            *(this->matrix[i]) /= num;
        }
        return *this;
    }
Matrix<T> &operator*=(double &num){
    if (num == 1){
        return *this;
    }
    for (int i = 0; i < this->n; i++)
        {
            *(this->matrix[i]) *= num;
        }
    return *this;
}
Matrix<T> &operator*=(const Matrix &righ){
    Matrix right = Matrix(righ);
    if (this->m == right.n)
    {
        Matrix newmatrix = (Matrix(this->n,right.m));
        for (int i = 0; i < this->n; i++)
        {
            for (int j = 0; j < right.m; j++)
            {
                double temp = 0;
                for (int k =0; k < this->m;k++){
                    temp += (*this)[i][k]*right[k][j];
                }
                newmatrix[i][j] = temp;
            }
        }
        for (int i = 0; i < this->n; i++)
        {
            for (int j = 0; j < right.m; j++)
            {
                (*this)[i][j] = (newmatrix)[i][j];
            }
        }
        return (*this);
    }
    throw(std::invalid_argument("Matrix are not compatible."));
}
double Product(const Matrix& mat){
    if (mat.n != mat.m){
        throw(std::invalid_argument("Matrix is not regular."));
    }
    double product = 1;
    for (int i = 0; i < mat.n; i++)
            product *= (*mat.matrix[i])[i];
    return product;
}
Matrix<T> Inverse(){
    if(this->n != this->m){
        throw(std::invalid_argument("Matrix not regular."));
    }
    Matrix mat = Matrix(*this);
    Matrix Inver = mat.back_substitute(false);
    return (Inver);
}
double Determinant(){
    if (this->n != this->m){
        throw(std::invalid_argument("Matrix not regular."));
    }
    Matrix mat = *this;
    try{
        Matrix augment = mat.back_substitute();
        return mat.det;
    }
    catch(std::exception &e){
        return 0;
    }
}
Matrix<T> Adjoint(){
    if (this->n != this->m){
        throw(std::invalid_argument("Matrix not regular."));
    }
    double determ = this->Determinant();
    Matrix inv = this->Inverse();
    inv *= determ;
    return inv;
}
Matrix<T> Transpose(){
    Matrix newMatrix = Matrix(this->m,this->n);
    for( int i = 0; i < this->m;i++){
        for ( int j = 0;j < this->n;j++){
            newMatrix[i][j]  = (*this)[j][i];
            }
    }
    return newMatrix;
}
int get_n() const{
    return this->n;
}
int get_m() const{
    return this->m;
}
};
template <typename T>
Matrix<T> &operator+(const Matrix<T> &left,const Matrix<T> &right){
    Matrix<T> *mat = new Matrix<T>(left.get_n(),left.get_m());
    *mat = left;
    return (*mat += right);
    }
template <typename T>
Matrix<T> &operator-(const Matrix<T> &left,const Matrix<T> &right){
    Matrix<T> *mat = new Matrix<T>(left.get_n(),left.get_m());
    *mat = left;
    return (*mat -= right);
    }
template <typename T>
Matrix<T> &operator*(const Matrix<T> &left,const Matrix<T> &right){
    Matrix<T>* ret=  new Matrix<T>(left);
    // *ret 
    return (*ret *= right);
}
template <typename T>
const std::ostream &operator<<(std::ostream&os,Matrix<T> &mat){
    // Output
    os<<"Matrix:\n";
    for (int i =0;i<mat.get_n();i++)
    os<<mat[i];
    return os;
};

int main(){
    return 0;
}