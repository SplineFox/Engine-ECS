#include "Render/SDLContext.h"

#include "Asset/Render/ImageAssetPayloadData.h"

namespace FlatFormer {

	SDLContext::SDLContext(SDL_Window* windowHandle) :
		m_WindowHandle(windowHandle)
	{
		FFE_DEFINE_LOGGER("SDLContext")
		Init();
		FFE_LOG_INFO("Initialized")
	}


	SDLContext::~SDLContext()
	{
		Shutdown();
		FFE_LOG_INFO("Released")
	}


	void SDLContext::Init()
	{
		int SDLflags = 0;
		SDLflags = SDL_RENDERER_SOFTWARE;

		m_Renderer = SDL_CreateRenderer(m_WindowHandle, -1, SDLflags);
		if (m_Renderer == nullptr)
		{
			FFE_LOG_ERROR("Unable to create SDL renderer! {}", SDL_GetError());
			exit(-1);
		}

		/// set the blending mode to be able to use translucent colors
		SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
	}
	

	void SDLContext::Shutdown()
	{
		if (m_Renderer)
		{
			SDL_DestroyRenderer(m_Renderer);
			m_Renderer = nullptr;
		}
	}


	void SDLContext::Clear()
	{
		SDL_RenderClear(m_Renderer);
	}


	void SDLContext::Clear(ColorRGBA color)
	{
		SetDrawColor(color);
		SDL_RenderClear(m_Renderer);
	}


	void SDLContext::Present()
	{
		SDL_RenderPresent(m_Renderer);
	}


	void SDLContext::SetDrawColor(uint8 r, uint8 g, uint8 b, uint8 a)
	{
		SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	}


	void SDLContext::SetDrawColor(ColorRGBA color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	}


	void SDLContext::DrawPoint(int x, int y)
	{
		SDL_RenderDrawPoint(m_Renderer, x, y);
	}


	void SDLContext::DrawLine(int x1, int y1, int x2, int y2)
	{
		SDL_RenderDrawLine(m_Renderer, x1, y1, x2, y2);
	}

	
	void SDLContext::DrawRectangle(int x, int y, int width, int height)
	{
		SDL_Rect rectangle = { x, y, width, height};
		SDL_RenderDrawRect(m_Renderer, &rectangle);
	}


	void SDLContext::DrawFilledRectangle(int x, int y, int width, int height)
	{
		SDL_Rect rectangle = { x, y, width, height };
		SDL_RenderFillRect(m_Renderer, &rectangle);
	}


	void SDLContext::DrawImage(std::shared_ptr<Asset::ImageAssetPayloadData> assetPayload, Rect destRect, float rotation, Point rotationCenter, FlipType flipType, ColorRGBA color)
	{
		/// convert surface to the texture (load texture to GPU)
		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, assetPayload->GetSurface());

		if (texture != nullptr)
		{
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture, color.a);

			SDL_RenderCopyEx(m_Renderer, texture, NULL, (SDL_Rect*)&destRect, rotation, (SDL_Point*)&rotationCenter, (SDL_RendererFlip)flipType);

			SDL_DestroyTexture(texture);
		}
	}

	void SDLContext::DrawFont(std::shared_ptr<Asset::FontAssetPayloadData> assetPayload, Rect destRect, std::string text, ColorRGBA color)
	{
		SDL_Surface* textSurface = TTF_RenderText_Solid(assetPayload->GetFont(), text.c_str(), (SDL_Color&)color);

		/// convert surface to the texture (load texture to GPU)
		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);

		SDL_FreeSurface(textSurface);

		if (texture != nullptr)
		{
			SDL_RenderCopy(m_Renderer, texture, NULL, (SDL_Rect*)&destRect);

			SDL_DestroyTexture(texture);
		}
	}


	void SDLContext::DrawCircle(int x, int y, int radius)
	{
		SDL_RenderDrawCircle(m_Renderer, x, y, radius);
	}


	void SDLContext::SDL_RenderDrawCircle(SDL_Renderer *Renderer, int _x, int _y, int radius)
	{
		int x = radius - 1;
		int y = 0;
		int tx = 1;
		int ty = 1;
		int err = tx - (radius << 1);

		while (x >= y)
		{
			SDL_RenderDrawPoint(Renderer, _x + x, _y - y);
			SDL_RenderDrawPoint(Renderer, _x + x, _y + y);
			SDL_RenderDrawPoint(Renderer, _x - x, _y - y);
			SDL_RenderDrawPoint(Renderer, _x - x, _y + y);
			SDL_RenderDrawPoint(Renderer, _x + y, _y - x);
			SDL_RenderDrawPoint(Renderer, _x + y, _y + x);
			SDL_RenderDrawPoint(Renderer, _x - y, _y - x);
			SDL_RenderDrawPoint(Renderer, _x - y, _y + x);

			if (err <= 0)
			{
				y++;
				err += ty;
				ty += 2;
			}
			else if (err > 0)
			{
				x--;
				tx += 2;
				err += tx - (radius << 1);
			}
		}
	}
}