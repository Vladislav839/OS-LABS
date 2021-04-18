#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std::chrono;

std::vector<std::vector<double>> A;
std::vector<std::vector<double>> B;
std::vector<std::vector<double>> result;
int k; 

void ReadMatrixes(std::string fileName)
{
	std::ifstream fin(fileName);
	int n1, m1, n2, m2;
	fin >> k >> n1 >> m1;
	A.resize(n1, std::vector<double>(m1));
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m1; j++) {
			fin >> A[i][j];
		}
	}
	fin >> n2 >> m2;
	if (m1 != n2) {
		std::cout << "Invalid matrixes dimension\n";
		exit(-1);
	}
	B.resize(n2, std::vector<double>(m2));
	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < m2; j++) {
			fin >> B[i][j];
		}
	}
}

void PrintMatrix(std::vector<std::vector<double>> m) {
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[0].size(); j++) {
			std::cout << m[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void SimpleMultiplication() {
	for (int i = 0; i < A.size(); i++) {
		for (int j = 0; j < B[0].size(); j++) {
			for (int k = 0; k < A[0].size(); k++) {
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}


int main()
{
	ReadMatrixes("input.txt");
	result.resize(A.size(), std::vector<double>(B[0].size()));
	auto start = high_resolution_clock::now();
	SimpleMultiplication();
	auto stop = high_resolution_clock::now();
	PrintMatrix(result);
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "Time taken by simple multiplication: "
		<< duration.count() << " microseconds" << std::endl;
	return 0;
}