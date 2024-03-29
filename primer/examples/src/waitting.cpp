/**
 * @file waitting.cpp
 * @brief use clock in time-delay loop
 */
#include <iostream>
#include <ctime>

int main() {
	using namespace std;
	cout << "Enter the delay in seconds: ";
	float secs;
	cin >> secs;
	clock_t delay = secs * CLOCKS_PER_SEC;
	cout << "starting\a" << endl;
	clock_t start = clock();
	while (clock() - start < delay);
	cout << "done\a" << endl;
	return 0;
}
