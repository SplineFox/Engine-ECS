#pragma once

#ifdef  FFE_PLATFORM_WINDOWS

extern FlatFormer::Application* FlatFormer::CreateApplication();

int main(int argc, char** argv)
{
	auto application = FlatFormer::CreateApplication();
	application->Run();
	delete application;
}

#endif
