#pragma once

#include <type_traits>

template<auto Start, auto End, int Inc = 1, typename Func>
constexpr decltype(auto) for_constexpr(Func&&);

class ConstexprLoop {
	template<auto Current, auto End, int Inc, typename Func>
	static constexpr decltype(auto) Recursion(Func&& expr) {
		constexpr bool Cond = Inc > 0 ? Current < End : Current >= End;
		using ExprRes = std::invoke_result_t<decltype(expr.template operator()<Current>())()>;
		if constexpr (std::is_same_v<ExprRes, RunTimeAction>) {
			if (expr.template operator()<Current>() == Break) {
				return;
			}
			if constexpr (Cond) {
				Recursion<Current + Inc, End, Inc>(expr);
			}
		} else if constexpr (std::is_same_v<ExprRes, void>) {
			expr.template operator()<Current>();
			if constexpr (Cond) {
				if constexpr (std::is_same_v<ResType<Current + Inc, End, Inc, Func>, void>) {
					Recursion<Current + Inc, End, Inc>(expr);
				} else {
					return Recursion<Current + Inc, End, Inc>(expr);
				}
			}
		} else {
			return expr.template operator()<Current>();
		}
	}

	template<auto Start, auto End, int Inc, typename Func>
	friend constexpr decltype(auto) for_constexpr(Func&&);

private:
	template<auto Current, auto End, int Inc, typename Func>
	using ResType = std::invoke_result_t<decltype(Recursion<Current, End, Inc, Func>), Func&&>;

public:
	enum RunTimeAction : bool {Continue, Break};
};

template<auto Start, auto End, int Inc, typename Func>
constexpr decltype(auto) for_constexpr(Func&& expr) {
	if constexpr (Start != End) {
		static_assert(Start < End || Inc < 0, "This will lead to an endless loop");
		static_assert(Start > End || Inc > 0, "This will lead to an endless loop");
		if constexpr (std::is_same_v<ConstexprLoop::ResType<Start, End - Inc, Inc, Func>, void>){
			ConstexprLoop::Recursion<Start, End - Inc, Inc>(expr);
		} else {
			return ConstexprLoop::Recursion<Start, End - Inc, Inc>(expr);
		}
	}
}
