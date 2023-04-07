#pragma once

#include "ECS/Component.h"
#include "Render/RenderStructs.h"

#include "Event/IEventListener.h"
#include "Window/WindowEvents.h"

namespace FlatFormer {

	class CameraComponent : public ECS::Component<CameraComponent>,
							public Event::IEventListener
	{
	public:
		unsigned int		width;
		unsigned int		height;
		ColorRGBA			backgroundColor;

	private:
		float				scale;

	public:
		CameraComponent(unsigned int width = 0u, unsigned int height = 0u) :
			width(width),
			height(height),
			backgroundColor(),
			scale(1.f)
		{
			this->RegisterEventCallback(&CameraComponent::OnWindowResized);
		}


		void Scale(float scaleIncrement)
		{
			scale += scaleIncrement;
			if (scale < 0.f)
			{
				scale = 0.f;
			}
		}


		void ScaleAt(float newScale)
		{
			if (newScale < 0.f)
			{
				scale = 0.f;
				return;
			}
			scale = newScale;
		}


		float GetScale()
		{
			return scale;
		}


		void OnWindowResized(const WindowResizedEvent* event)
		{
			width = event->width;
			height = event->height;
		}

	};

}