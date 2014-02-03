
#include "Demi.h"
#include "DemoFrame.h"

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app(DemoConfig("Demi test: Gui"));
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open();

	return 0;
}