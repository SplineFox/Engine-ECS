#pragma once

#include "Engine/API.h"
#include "Render/IGraphicsContext.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace FlatFormer {

	class SDLContext : public IGraphicsContext
	{
	private:
		SDL_Renderer* m_Renderer;
		SDL_Window* m_WindowHandle;

		FFE_DECLARE_LOGGER

	public:
		SDLContext(SDL_Window* windowHandle);
		~SDLContext();

		inline void* GetNativeContext() const override { return m_Renderer; };

		void Clear() override;
		void Clear(ColorRGBA color) override;
		void Present() override;
		void SetDrawColor(uint8 r, uint8 g, uint8 b, uint8 a) override;
		void SetDrawColor(ColorRGBA color) override;

		void DrawPoint(int x, int y) override;
		void DrawLine(int x1, int y1, int x2, int y2) override;

		void DrawRectangle(int x, int y, int width, int height) override;
		void DrawFilledRectangle(int x, int y, int width, int height) override;

		void DrawImage(std::shared_ptr<Asset::ImageAssetPayloadData> assetPayload, Rect destRect, float rotation, Point rotationCenter, FlipType flipType, ColorRGBA color) override;
		void DrawFont(std::shared_ptr<Asset::FontAssetPayloadData> assetPayload, Rect destRect, std::string text, ColorRGBA color) override;

		void DrawCircle(int x, int y, int radius) override;

	private:
		void Init();
		void Shutdown();
		void SDL_RenderDrawCircle(SDL_Renderer *Renderer, int x, int y, int radius);
	};

}