#include "Core/Settings.hpp"
#include "Core/Window.hpp"
#include "Util/CommonVersionFormat.hpp"
#include "Invasion.hpp"

using namespace Invasion;
using namespace Invasion::Util;
using namespace Invasion::Core;

int main()
{
	Settings::GetInstance().Set<String>("defaultDomain", "Invasion");
	
	InvasionGame::GetInstance().PreInitialize();

	Window::GetInstance().Initialize(Settings::GetInstance().Get<String>("defaultApplicationTitle") + "* " + Settings::GetInstance().Get<CommonVersionFormat>("defaultApplicationVersion"), Settings::GetInstance().Get<Vector2i>("defaultWindowDimensions"));

	InvasionGame::GetInstance().Initialize();
	
	Window::GetInstance().AddUpdateFunction([] { InvasionGame::GetInstance().Update(); });
	Window::GetInstance().AddRenderFunction([] { InvasionGame::GetInstance().Render(); });
		
	Window::GetInstance().Run();

	InvasionGame::GetInstance().CleanUp();
	Window::GetInstance().CleanUp();

	return 0;
}