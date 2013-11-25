/*
 * Unit test for Timer
 */

#include <Common/Window.hpp>

#include <BlendInt/Timer.hpp>

#include <gtest/gtest.h>

// The fixture for testing class Foo.

using namespace BlendInt;

class TimerTest: public testing::Test
{

protected:

	TimerTest () {

	}

	virtual ~TimerTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(TimerTest, CheckTimeOut)
{
	Timer timer;

	timer.Start ();

	timer.Stop ();
}

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here
	// testing::InitGoogleTest (&argc, argv);

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS();
	return 0;
}