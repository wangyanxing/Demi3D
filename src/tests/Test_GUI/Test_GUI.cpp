
#include "DiPch.h"
#include "Application.h"
#include "Skybox.h"
#include "SimpleShape.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShaderManager.h"
#include "RenderWindow.h"
#include "GfxDriver.h"


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