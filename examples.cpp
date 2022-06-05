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

	cout << "\nNon-void (compile time terminated) loop:";
	constexpr array<int, 5> Nums{3, 4, 10, 7, 4};
	constexpr size_t WhereIsTen = for_constexpr<0, Nums.size()>([&Nums]<size_t I>() {
		static_assert(I < 3, "");
		if constexpr (Nums[I] == 10) { // More complex stop condition
			return I;
		}
	});
	cout << "\n10 is at the position " << WhereIsTen;

	// constexpr size_t WhereIsOne = for_constexpr<0, Nums.size()>([&Nums]<size_t I>() {
	// 	if constexpr (Nums[I] == 1) {
	// 		return I;
	// 	}
	// });
	// Error: there's no 1 in Nums, so the return type will be void

	cout << "\nDone!";
}
