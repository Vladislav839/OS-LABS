#include <pthread.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std::chrono;

std::vector<std::vector<double>> A;
std::vector<std::vector<double>> B;
std::vector<std::vector<double>> result;
pthread_t* threads;
pthread_mutex_t mt;
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

struct Data {
	int a_block;
	int b_block;
	int thread_index;
	int total_threads;
	Data() = default;
	Data(int a_block, int b_block, int thread_index, int total_threads) {
		this->a_block = a_block;
		this->b_block = b_block;
		this->thread_index = thread_index;
		this->total_threads = total_threads;
	}
};

struct MatrixInfo
{
	int start_row;
	int start_col;
	int end_row;
	int end_col;
	MatrixInfo() = default;

	MatrixInfo(int start_row, int start_col, int end_row, int end_col)
	{
		this->start_row = start_row;
		this->start_col = start_col;
		this->end_row = end_row;
		this->end_col = end_col;
	}
};

std::vector<MatrixInfo> blocks_A;
std::vector<MatrixInfo> blocks_B;

void PrintBlock(std::vector<MatrixInfo> block) {
	for (int i = 0; i < block.size(); i++) {
		std::cout << "start_row: " << block[i].start_row << '\n';
		std::cout << "start_col: " << block[i].start_col << '\n';
		std::cout << "end_row: " << block[i].end_row << '\n';
		std::cout << "end_col: " << block[i].end_col << '\n';
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

void SimpleMultiplication(MatrixInfo a_info, MatrixInfo b_info) {
	for (int i = a_info.start_row; i < a_info.end_row; i++) {
		for (int j = b_info.start_col; j < b_info.end_col; j++) {
			for (int k = a_info.start_col; k < a_info.end_col; k++) {
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void* ThreadProc(void* lpParametr)
{
	pthread_mutex_lock(&mt);
	Data* data = (Data*)lpParametr;
	MatrixInfo A_info = blocks_A[data->a_block];
	MatrixInfo B_info = blocks_B[data->b_block];
	SimpleMultiplication(A_info, B_info);
	pthread_mutex_unlock(&mt);
    return 0;
}

void InitBlocks(int k) {
	int n = A.size();
	int l = A[0].size();
	int m = B[0].size();
	int height = (int)n / k;
	blocks_A.resize(k);
	blocks_B.resize(k);
	if (n % k == 0) {
		blocks_B[k - 1] = MatrixInfo(m - height, 0, m, l);
		blocks_A[k - 1] = MatrixInfo(0, n - height, l, n);
	}
	else {
		blocks_B[k - 1] = MatrixInfo(m - height - 1, 0, m, l);
		blocks_A[k - 1] = MatrixInfo(0, n - height - 1, l, n);
	}
	for (size_t i = 0; i < k - 1; i++) {
		blocks_B[i] = MatrixInfo(i * height, 0, height + i * height, l);
		blocks_A[i] = MatrixInfo(0, i * height, l, height + i * height);
	}
}


void ColToRowMultiplication(int k) {
	InitBlocks(k);
	threads = new pthread_t[k];
	for (int i = 0; i < k; i++) {
		int a_block = i;
		int b_block = i;
		int thread_index = i;
		Data* data = new Data(a_block, b_block, thread_index, k);
		pthread_create(&threads[i], NULL, &ThreadProc, (void*)data);
	}
}

int main()
{
	ReadMatrixes("input.txt");
	result.resize(A.size(), std::vector<double>(B[0].size()));
	auto start = high_resolution_clock::now();
	ColToRowMultiplication(k);
	for (int i = 0; i < k; i++) {
		pthread_join(threads[i], NULL);
	}
	auto stop = high_resolution_clock::now();
	PrintMatrix(result);
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "Time taken by ColToRowMultiplication: "
		<< duration.count() << " microseconds" << std::endl;

	delete[] threads;
	return 0;
}