#include "Application/Application.h"

int main(int argc, char *argv[])
{
	using namespace FlatFormer;
	using namespace ECS;
	
	FFE_DECLARE_LOGGER
	FFE_DEFINE_LOGGER("main")

	FFE_LOG_INFO("Something")

	FlatFormer::Application* app = new FlatFormer::Application();
	app->Run();

	delete app;

	FFE_SPDLOG_DROP_ALL
	return 0;
}