#pragma once

#include <type_traits>

template<auto Start, auto End, int Inc = 1, typename Func>
constexpr void for_constexpr(Func&&);

class ConstexprLoop {
	template<auto Current, auto End, int Inc, typename Func>
	constexpr static void Recursion(Func&& expr) {
		if constexpr (std::is_same_v<std::invoke_result_t<Func&&()>, Action>) {
			if (expr.template operator()<Current>() == Break) {
				return;
			}
		} else {
			expr.template operator()<Current>();
		}
		constexpr bool cond = Inc > 0 ? Current < End : Current >= End;
		if constexpr (cond) {
			Recursion<Current + Inc, End, Inc>(expr);
		}
	}

	template<auto Start, auto End, int Inc, typename Func>
	friend constexpr void for_constexpr(Func&&);

public:
	enum Action : bool {Continue, Break};
};

template<auto Start, auto End, int Inc, typename Func>
constexpr void for_constexpr(Func&& expr) {
	if constexpr (Start != End) {
		static_assert(Start < End || Inc < 0, "This will lead to an endless loop");
		static_assert(Start > End || Inc > 0, "This will lead to an endless loop");
		ConstexprLoop::Recursion<Start, End - Inc, Inc>(expr);
	}
}