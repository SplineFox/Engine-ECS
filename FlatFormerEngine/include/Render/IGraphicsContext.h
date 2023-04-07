#pragma once

#include "Engine/API.h"
#include "Asset/Render/ImageAssetPayloadData.h"
#include "Asset/Render/FontAssetPayloadData.h"
#include "Render/BasicColors.h"
#include "Render/RenderStructs.h"

namespace FlatFormer {

	class IGraphicsContext
	{
	public:
		virtual void* GetNativeContext() const = 0;

		virtual void Clear() = 0;
		virtual void Clear(ColorRGBA color) = 0;
		virtual void Present() = 0;
		virtual void SetDrawColor(uint8 r, uint8 g, uint8 b, uint8 a) = 0;
		virtual void SetDrawColor(ColorRGBA color) = 0;

		virtual void DrawPoint(int x, int y) = 0;
		virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;

		virtual void DrawRectangle(int x, int y, int width, int height) = 0;
		virtual void DrawFilledRectangle(int x, int y, int width, int height) = 0;

		virtual void DrawCircle(int x, int y, int radius) = 0;

		virtual void DrawImage(std::shared_ptr<Asset::ImageAssetPayloadData> assetPayload, Rect destRect, float rotation, Point rotationCenter, FlipType flipType, ColorRGBA color) = 0;
		virtual void DrawFont(std::shared_ptr<Asset::FontAssetPayloadData> assetPayload, Rect destRect, std::string text, ColorRGBA color) = 0;
	};

}