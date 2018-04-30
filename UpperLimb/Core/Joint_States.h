#include <yarp\os\Portable.h>
#include <yarp\os\ConnectionWriter.h>
#include <yarp\os\ConnectionReader.h>

#include <vector>

class Joint_States : public yarp::os::Portable
{
public:
	/*
	enum EndEffector
	{
		Grasping,
		Moving,
		Releasing
	};
	*/
	std::vector<float> position;
	std::vector<float> velocity;
	//EndEffector end_effector_state;

	Joint_States();

	//copy constructor
	Joint_States(const Joint_States& other);

	//move semantics
	Joint_States(Joint_States && other);
	Joint_States& operator=(Joint_States other);
	friend void swap(Joint_States& first, Joint_States& second)
	{
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;

		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.position, second.position);
		swap(first.velocity, second.velocity);
		//swap(first.end_effector_state, second.end_effector_state);
	}

	virtual bool write(yarp::os::ConnectionWriter& connection);
	virtual bool read(yarp::os::ConnectionReader& connection);
};