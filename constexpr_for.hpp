#pragma once

#include <type_traits>

template<auto Start, auto End, int Inc = 1, typename Func1, typename Func2 = std::nullptr_t>
constexpr decltype(auto) for_constexpr(Func1&& main_expr, Func2&& fin_expr = nullptr);

class ConstexprLoop {
	template<auto Current, auto End, int Inc, typename Func1, typename Func2>
	static constexpr decltype(auto) Recursion(Func1&& main_expr, Func2&& fin_expr) {
		constexpr bool NotLast = Inc > 0 ? Current < End : Current >= End;
		using ExprRes = std::invoke_result_t<decltype(main_expr.template operator()<Current>())()>;
		if constexpr (std::is_same_v<ExprRes, RunTimeAction>) {
			if (main_expr.template operator()<Current>() == Break) {
				return;
			}
			if constexpr (NotLast) {
				Recursion<Current + Inc, End, Inc>(main_expr, fin_expr);
			} else if constexpr (!std::is_null_pointer_v<Func2>) {
				fin_expr();
			}
		} else if constexpr (std::is_void_v<ExprRes>) {
			main_expr.template operator()<Current>();
			if constexpr (NotLast) {
				return Recursion<Current + Inc, End, Inc>(main_expr, fin_expr);
			} else if constexpr (!std::is_null_pointer_v<Func2>) {
				return fin_expr();
			}
		} else {
			return main_expr.template operator()<Current>();
		}
	}

	template<auto Start, auto End, int Inc, typename Func1, typename Func2>
	friend constexpr decltype(auto) for_constexpr(Func1&&, Func2&&);

public:
	enum RunTimeAction : bool {Continue, Break};
};

template<auto Start, auto End, int Inc, typename Func1, typename Func2>
constexpr decltype(auto) for_constexpr(Func1&& main_expr, Func2&& fin_expr) {
	if constexpr (Start != End) {
		return ConstexprLoop::Recursion<Start, End - Inc, Inc>(main_expr, fin_expr);
	}
}
