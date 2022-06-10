# Compile-time-for-loop
## Intro
There are occasions when plain C++ loops are not enough. For example, the counter of such loop is not a constant, therefore it cannot be used in a constant expression or as a template argument. Or one may simply want to unroll a loop in order to avoid redundant run-time checks and arithmetic. Of course, these goals could be achieved with a plain template recursion, but it often requires an additional template function and overall less readable. So, the goal of this work is to provide an opportunity to organize the recursive code into an expression resembling a classical for loop.
## Syntax Info
The general syntax of the constexpr for function is following:
```cpp
template<auto Start, auto End, int Inc = 1, typename Func1, typename Func2  = std::nullptr_t>
constexpr void for_constexpr(Func1 main_expr, Func2 fin_expr = nullptr)
```
### Parameters
`Start` – the beginning of the loop.  
`End` – the end of the loop.  
`Inc` – the increment (1 by default).  
`main_expr` – the main expression: a function object, usually passed as a lambda, that has one templated argument of an integral type (C++ 20 generic lambda), like the following:  
```cpp
[/*capture something*/]<IntegralType counter>() {
		//do something
	});
```
(optional) `fin_expr` - the expression that should be executed after the loop.
### Types Of Loops
Reversed loops are also supported. In non-reversed loops the End value does not enter the loop, in reversed – both Start and End enter. In general, the behavior of the loop could be described as following:  
Non-reversed:  
```cpp
for (auto i = Start;  i < End; ++i) { 
//do something 
};
```
Reversed:
```cpp
for (auto i = End;  i >= Start; ++i) { 
//do something 
};
```
### Termination
#### Run time execution abortion
There are also situations when the loop should be aborted in run time anyway (I personally encountered with such a situation). For this I added `break` and `continue`-like statements. In order to abort the execution of the loop you should specify in the main expression:
```cpp
return ConstexprLoop::Break;
```
After this the loop will be unrolled and all its recursive templates will be instantiated, but none of the following instantiations will be called. The execution of the loop will stop here. Since the return type of the `expr` has changed, it will require to specify `return ConstexprLoop::Continue` at the end of the main expression, where the iteration shouldn’t be the last.  
#### Compile time termination. Non-void loops
This kind of loops could also be terminated by a more complex condition then a simple upper bound value. For this you need to have the main expression return a value of a type distinct from ConstexprLoop::RunTimeAction. The same approach could be used if you need to initialize a constexpr value using this loop.  
See `examples.cpp` file for more information.
## Advantages of C++ 20 approach
Before the emergence of generic lambdas it was more difficult to pass the counter into a constant expression. It was impossible to do this directly and it usually required to use instruments like std::integral_constant. Such approach is working, but makes the editor features (like code completion, error checking, etc.) less responsible.
