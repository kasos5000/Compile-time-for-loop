#include <iostream>
#include <tuple>
#include "constexpr_for.hpp"

using namespace std;

int main() {
	tuple<int, double, string, size_t> my_tuple{1, 1.5, "hello, constexpr for", 2};
	constexpr size_t Size1 = tuple_size_v<decltype(my_tuple)>;
	cout << "Strait loop:\n";
	for_constexpr<0, Size1>([&my_tuple]<size_t I>() {
		cout << get<I>(my_tuple) << "; ";
	});

	cout << "\nReversed loop:\n";
	for_constexpr<Size1 - 1, 0, -1>([&my_tuple]<size_t I>() {
		cout << get<I>(my_tuple) << "; ";
	});

	cout << "\nSkip loop:\n";
	for_constexpr<0, Size1, 2>([&my_tuple]<size_t I>() {
		cout << get<I>(my_tuple) << "; ";
	});

	tuple<int*, double*, string*, size_t*, int*, double*, size_t*> ptr_tuple;
	constexpr size_t Size2 = tuple_size_v<decltype(ptr_tuple)>;
	for_constexpr<0, Size1>([&my_tuple, &ptr_tuple]<size_t I>() {
		get<I>(ptr_tuple) = &get<I>(my_tuple);
	});
	for_constexpr<Size1, Size2>([&ptr_tuple]<size_t I>() {
		get<I>(ptr_tuple) = nullptr;
	});
	cout << "\nTerminated loop:\n";
	for_constexpr<0, Size2>([&ptr_tuple]<size_t I>() {
		if (!get<I>(ptr_tuple)) {
			return ConstexprLoop::Break;
		}
		cout << *get<I>(ptr_tuple) << "; ";
		return ConstexprLoop::Continue;
	});
	cout << "\nDone!";
}
