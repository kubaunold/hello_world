#include "pamsi_tester/Tester.hpp"
#include "spdlog/spdlog.h"
#include <fstream>
#include <numeric>
#include <vector>

using Matrix = std::vector<float>;


std::ostream& operator<<(std::ostream& output, const Matrix& matrix)
{
  std::ostream_iterator<float> outputIterator (output,", ");
  std::copy(matrix.begin(), matrix.end(), outputIterator );

  // for(int i = 0; i < matrix.size(); ++i)
  //   {
  //     output << matrix[i] << " ";
  //   }

  return output;
}

struct MatrixSumInput
{
  Matrix matrix1;
  Matrix matrix2;
};

class MatrixSumTester: public Tester<Matrix, MatrixSumInput>
{
protected:
  Matrix runAlgorithm(const MatrixSumInput& inputData) override;
  MatrixSumInput readSingleInput(std::istream& inputStream) override;
};



MatrixSumInput MatrixSumTester::readSingleInput(std::istream& inputStream)
{
  int nRows = 0, nCols = 0;

  inputStream >> nRows >> nCols;

  MatrixSumInput input;

  for(int i = 0; i < nRows*nCols; ++i)
    {
      float matrixElement = 0.f;

      inputStream >> matrixElement;
      input.matrix1.push_back(matrixElement);
    }

  for(int i = 0; i < nRows*nCols; ++i)
    {
      float matrixElement = 0.f;

      inputStream >> matrixElement;
      input.matrix2.push_back(matrixElement);
    }

  return input;

}

Matrix MatrixSumTester::runAlgorithm(const MatrixSumInput& inputData)
{
  Matrix result;
  result.reserve(inputData.matrix1.size());

  for(std::size_t i = 0; i < inputData.matrix1.size(); ++i)
    {
      result.push_back(inputData.matrix1[i] + inputData.matrix2[i]);
    }

  return result;
}

class SumTester : public Tester<int, std::vector<int>>
{
  protected:
    int runAlgorithm(const std::vector<int>& inputData) override;
    std::vector<int> readSingleInput(std::istream& inputStream) override;
};

int SumTester::runAlgorithm(const std::vector<int>& inputData)
{
    return std::accumulate(std::begin(inputData), std::end(inputData), 0,
                           std::plus<int>{});
}

std::vector<int> SumTester::readSingleInput(std::istream& inputStream)
{
    int numOfData = 0;

    inputStream >> numOfData;

    std::vector<int> result;
    result.reserve(numOfData);

    for(auto i = 0; i < numOfData; ++i)
    {
        int dataElement;
        inputStream >> dataElement;

        result.push_back(dataElement);
    }

    return result;
}

int main(int /*argc*/, char* /*argv*/[])
{
    std::ifstream inputFile{"input.txt"};
    std::ofstream dataOutputFile{"output.txt"}, timeOutputFile{"times.csv"};


    if(!inputFile)
    {
        spdlog::error("input.txt cannot be opened!");
        return -1;
    }

    MatrixSumTester tester;

    tester.runAllTests(inputFile, dataOutputFile, timeOutputFile);
}
