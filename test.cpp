#include "include/cptest/cptest.h"

#include <string>

using namespace org::iatoki::cptest;
using std::string;

struct Problem : BaseProblem {
	int A, B;

	void inputFormat() {
		line() % A % B;
	}
};

struct Generator : BaseGenerator<Problem> {

	void testSet1() {
		addTestCase(TestCase([this] { A = 3; B = 4;}, ""));
		addTestCase(TestCase([this] { A = 7; B = 4;}, ""));
	}

	void testSet2() {
		addTestCase(TestCase([this] { A = 1; B = 1;}, ""));
		addTestCase(TestCase([this] { A = 2; B = 2;}, ""));
	}
};

int main() {

	Generator gen;
	gen.generate();
}