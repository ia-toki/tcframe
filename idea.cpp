#include <bits/stdc++.h>
using namespace std;

class Variable {
public:
	virtual void print(ostream& out) = 0;
	virtual ~Variable() {};
};

template<typename T>
class Scalar : public Variable {
private:
	T* value;

public:

	Scalar(T& value) {
		this->value = &value;
	}

	void print(ostream& out) {
		out << *value;
	}
};

class IOSegment {
public:
	virtual void print(ostream& out) = 0;
};

class LineIOSegment : public IOSegment {
private:
	vector<Variable*> vars;

public:
	LineIOSegment& add(int& x) {
		vars.push_back(new Scalar<int>(x));
		return *this;
	}

	void print(ostream& out) {
		bool first = true;
		for (Variable* v : vars) {
			if (!first)
				out << " ";
			first = false;
			v->print(out);
		}
		out << "\n";
	}
};

typedef function<bool()> Predicate;
typedef function<void()> TestCase;

class Problem {
private:
	vector<Predicate> predicates;
	vector<IOSegment*> segments;
protected:
	void addPredicate(Predicate pred) {
		predicates.push_back(pred);
	}

	LineIOSegment& line() {
		LineIOSegment* segment = new LineIOSegment();
		segments.push_back(segment);
		return *segment;
	}

public:
	
	virtual void inputFormat() = 0;
	virtual void constraint() = 0;

	void printCase() {
		for (IOSegment* segment : segments) {
			segment->print(cout);
		}
	}
};

struct MyProblem : Problem
{
	int A, B;

	void inputFormat() {
		line().add(A).add(B);
	}

	void subtaskConstraints1() {
		addPredicate([this] { return A <= 100; });
	}
};


template<typename T>
class Generator : protected T
{
private:
	vector<TestCase> testCases;

protected:
	void addCase(TestCase tc)
	{
		testCases.push_back(tc);
	}

public:

	virtual void set1() = 0;

	void run() {
		T::inputFormat();
		set1();
		for (TestCase tc : testCases) {
			tc();
			T::printCase();
		}
	}
};

#define CASE(tc) addCase([this] { tc })

struct MyGenerator : Generator<MyProblem> {

	void init() {
		assignToSubtasks({0, 1, 4});
		assignToSubtasks({0, 1, 4});
	}

	void testSet1() {
		CASE(A = 4; B = 4;);
		CASE(A = 4; B = 5;);
	}

	void testSet2() {

	}
};

int main() {

	MyGenerator gen;
	gen.run();

}