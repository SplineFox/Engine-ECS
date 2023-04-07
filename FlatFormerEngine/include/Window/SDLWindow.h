#pragma once

#include "Engine/API.h"
#include "Window/IWindow.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace FlatFormer {

	class SDLWindow : public IWindow
	{
	private:
		SDL_Window*	m_Window;
		IGraphicsContext* m_Context;

		std::string m_Title;

		int m_Width, m_Height;
		int m_PosX,	m_PosY;

		bool m_VSync;
		bool m_Fullscreen;

		FFE_DECLARE_LOGGER

	public:
		SDLWindow(const WindowProperties& properties);
		~SDLWindow();

		inline void* GetNativeWindow()			const override { return m_Window; }
		inline IGraphicsContext* GetContext()	const override { return m_Context; }

		void Update() override;

		inline int GetWidth()	const override { return m_Width; }
		inline int GetHeight()	const override { return m_Height; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void SetTitle(const std::string& newTitle) override;

	private:
		void Init();
		void Shutdown();

		void ProcessSDLWindowEvents();

	};

}