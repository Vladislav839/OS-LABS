#include "buffered_channel.h"

#include <iostream>
#include <random>
#include <chrono>

BufferedChannel<int32_t> buffered_channel(10);
const int max_counter = 10;
int counter = 0;
bool channel_is_closed = false;


void prepare_data() {
	std::default_random_engine engine(rand() % 10);
	std::uniform_int_distribution<int32_t> int_distribution(10, 100);
	auto value = int_distribution(engine);
	buffered_channel.Send(value);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	++counter;

	if (counter == max_counter) {
		buffered_channel.Close();
		channel_is_closed = true;
		return;
	}

	prepare_data();
}

void extract_data() {
	if (channel_is_closed && buffered_channel.Size() == 0) return;
	auto value = buffered_channel.Recv();
	std::cout << "( " << value.first << ", " << value.second << " )\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	extract_data();
}

int main() {
	srand(time(NULL));
	std::thread th1(prepare_data);
	std::thread th2(extract_data);

	th2.join();
	th1.join();

	return 0;
}