
#include "Demi.h"
#include "DemoFrame.h"

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
}

void UpdateScene()
{
}

int main()
{
	DiApp app;
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open(800, 600, "Demi3D Test - GUI");

	while (app.IsOpen())
	{
		app.Update();
	}

	return 0;
}