#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

TEST(Main, LinsolveLowerTriangular)
{
	// currently already tested by LinsolveChol
	// TODO: add a specific test later
}
