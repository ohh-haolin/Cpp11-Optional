#include "Optional.hpp"

struct A
{
	int age;
	void fun()
	{
		::std::cout << "hello" << '\n';
	}
};

int main()
{
	using namespace std;

	auto a = Optional<A>(new A{100});

	// 测试->，*，和get, 输出100和100和100
	cout << a->age << '\t' << (*a).age << '\t' << a.get().age << '\n';
	// 测试empty
	auto c = Optional<A>::empty();
	// 测试Of，支持传入指针和引用
	auto d = Optional<A>::of(new A{10});
	auto e = Optional<A>::of(A{10});

	// 测试ofNullable，支持传入指针，引用，空指针，
	// 测试抛出异常
	auto f = Optional<A>::ofNullable(nullptr);
	try
	{
		cout << f.get().age << '\n';
	}catch(Optional<A>::NullptrException E)
	{
		cout << E.what() << '\n';
	}

	// 测试isPresent和ifPresent
	if(a.isPresent())
	{
		// 输出hello
		a.ifPresent([](A* x){x->fun();});
	}

	// 测试filter
	// 输出100
	cout << "filter:\t" << a.filter([](A* x){return x->age > 10;}).get().age << '\n';


	// 测试map
	// 输出100
	cout << "map:\t" << (a.map([](A* x){return x->age;})).get() << '\n';

	// 测试orElseGet
	// 输出100
	f.orElse(a).ifPresent([](A* x){cout << "orElse:\t" << x->age << '\n';});

	// 测试orElseGet
	// 输出100
	cout << "orElseGet:\t" << f.orElseGet(a).age << '\n';

	// 测试orElseThrow
	// 输出myee
	NullptrException myee("myee");
	try{
		f.orElseThrow(myee);
	}catch(Optional<A>::NullptrException E){
		cout << "orElseThrow:\t" << E.what() << '\n';
	}
	return 0;
}