#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

TEST(Main, LinsolveUpperTriangular)
{
	// currently already tested by LinsolveChol
	// TODO: add a specific test later
}
