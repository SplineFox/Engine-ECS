#include "Window/SDLWindow.h"

#include "Render/SDLContext.h"

namespace FlatFormer {

	IWindow* IWindow::Create(const WindowProperties& properties)
	{
		return new SDLWindow(properties);
	}


	SDLWindow::SDLWindow(const WindowProperties & properties) :
		m_Window(nullptr),
		m_Context(nullptr),
		m_Title(properties.title),
		m_Width(properties.width),
		m_Height(properties.height),
		m_PosX(0),
		m_PosY(0),
		m_VSync(false),
		m_Fullscreen(false)
	{
		FFE_DEFINE_LOGGER("SDLWindow")
		Init();
		FFE_LOG_INFO("Initialized")
	}


	SDLWindow::~SDLWindow()
	{
		Shutdown();
		FFE_LOG_INFO("Released")
	}


	void SDLWindow::Init()
	{
		int SDLInitFlags = SDL_INIT_EVERYTHING;
		int SDLInitResult = SDL_Init(SDLInitFlags);
		if (SDLInitResult != 0)
		{
			FFE_LOG_CRITICAL("Unable to initialize SDL! {}", SDL_GetError());
			exit(-1);
		}

		int IMGInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		int IMGInitResult = IMG_Init(IMGInitFlags);
		if ((IMGInitResult & IMGInitFlags) != IMGInitFlags)
		{
			FFE_LOG_CRITICAL("Unable to initialize SDL_IMG! {}", IMG_GetError());
			exit(-1);
		}

		int TTFInitResult = TTF_Init();
		if (TTFInitResult != 0)
		{
			FFE_LOG_CRITICAL("Unable to initialize SDL_TTF! {}", IMG_GetError());
			exit(-1);
		}

		int SDLWindowFlags = SDL_WINDOW_RESIZABLE;
		if (m_Fullscreen)
		{
			SDLWindowFlags |= SDL_WINDOW_FULLSCREEN;
		}

		m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height, SDLWindowFlags);
		if (!m_Window)
		{
			FFE_LOG_CRITICAL("Unable to create SDL window! {}", SDL_GetError());
			exit(-1);
		}

		m_Context = new SDLContext(m_Window);

		SDL_GetWindowPosition(this->m_Window, &m_PosX, &m_PosY);
		SDL_GetWindowSize(this->m_Window, &m_Width, &m_Height);
	}


	void SDLWindow::Shutdown()
	{
		if (m_Window)
		{
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}

		IMG_Quit();
		SDL_Quit();
	}


	void SDLWindow::Update()
	{
		ProcessSDLWindowEvents();
	}


	void SDLWindow::SetVSync(bool enabled)
	{
	}


	bool SDLWindow::IsVSync() const
	{
		return false;
	}


	void SDLWindow::SetTitle(const std::string & newTitle)
	{
		SDL_SetWindowTitle(m_Window, newTitle.c_str());
	}


	void SDLWindow::ProcessSDLWindowEvents()
	{
		/// we collect events from states and devices, filling the queue with events
		SDL_PumpEvents();

		SDL_Event event;
		/// 1 means that we receive only one event from the queue,
		/// SDL_GETEVENT means that the event will be dequeued,
		/// SDL_FIRSTEVENT and SDL_SYSWMEVENT define the range of event types to retrieve
		while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_SYSWMEVENT))
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:
				break;

			case SDL_WINDOWEVENT_HIDDEN:
				break;

			case SDL_WINDOWEVENT_EXPOSED:
				break;

			case SDL_WINDOWEVENT_MOVED:
				break;

			case SDL_WINDOWEVENT_RESIZED:
				SendEventByQueue<WindowResizedEvent>(event.window.data1, event.window.data2);
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				SendEventByQueue<WindowMinimizedEvent>();
				break;

			case SDL_WINDOWEVENT_MAXIMIZED:
				SendEventByQueue<WindowMaximizedEvent>();
				break;

			case SDL_WINDOWEVENT_RESTORED:
				SendEventByQueue<WindowRestoredEvent>();
				break;

			case SDL_WINDOWEVENT_ENTER:
				break;

			case SDL_WINDOWEVENT_LEAVE:
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				break;

			case SDL_WINDOWEVENT_CLOSE:
				SendEventByQueue<WindowClosedEvent>();
				break;

			case SDL_WINDOWEVENT_TAKE_FOCUS:
				break;

			case SDL_WINDOWEVENT_HIT_TEST:
				break;
			}
		}
	}

}