#pragma once

#include "Engine/API.h"
#include "Engine/Service/Wrap/EventManagerUserWrap.h"
#include "Render/IGraphicsContext.h"
#include "Window/WindowEvents.h"

namespace FlatFormer {

	struct WindowProperties
	{
		std::string title;
		int width;
		int height;

		WindowProperties(const std::string& title = "FlatFormer",
						 int width = 1280,
						 int height = 720) :
			title(title),
			width(width),
			height(height)
		{}
	};


	class IWindow : public Service::EventManagerUserWrap
	{
	public:
		virtual ~IWindow() {};

		virtual void Update() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetTitle(const std::string& newTitle) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual IGraphicsContext* GetContext() const = 0;

		static IWindow* Create(const WindowProperties& properties = WindowProperties());
	};

}