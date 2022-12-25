#include <Mathyw/vector.hpp>

int main(int argc, char** argv)
{
	
	Mathyw::Fvec2 v1 = { 2, 3 }, v2 = { 5, 6 };
	std::cout << (v1 + v2).Get(1, 1, 0) << '\n';

	
	return 0;
}