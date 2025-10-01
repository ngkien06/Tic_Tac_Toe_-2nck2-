#include "Config.h"

float ScreenS::ScreenWidth = 800;
float ScreenS::ScreenHeight = 800;

Color ScreenC::C[6] = {
	// Soft Blues
		{ 25, 55, 92, 255 },
		{ 187, 222, 251, 255 },
		{ 144, 192, 249, 255 },
		{ 100, 181, 246, 255 },
		{ 226, 242, 253, 255 },
		{ 66, 165, 245, 255 }
};

/*
C[0] : Text
C[1] : Node, Second menu
C[2] : Button
C[3] : Edge
C[4] : Background
C[5] : Active node
*/