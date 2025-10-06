#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

namespace ScreenS {
	extern float ScreenWidth;
	extern float ScreenHeight;
}

/// <summary>
/// C[0] : Text |
/// C[1] : Node, Second menu |
/// C[2] : Button |
/// C[3] : Edge |
/// C[4] : Background |
/// C[5] : Active node |
/// </summary>
namespace ScreenC {
	extern Color C[6];
}

#endif // !CONFIG_H

