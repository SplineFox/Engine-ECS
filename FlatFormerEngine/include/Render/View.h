#pragma once

#include "ECS/Render/CameraComponent.h"
#include "ECS/Common/TransformComponent.h"
#include "Math/Transform2D.h"

#include "Render/RenderStructs.h"

#include "Event/IEventListener.h"
#include "Window/KeyCodes.h"
#include "Window/InputEvents.h"


namespace FlatFormer { namespace Render {

	class View : public Event::IEventListener
	{
	public:
		Transform2D*		m_DefaultTransform;
		Transform2D*		m_CurrentTransform;

		CameraComponent*	m_DefaultCamera;
		CameraComponent*	m_CurrentCamera;


	public:
		View(unsigned defualtWidth, unsigned defaultHeight);

		~View();

		Point ConvertToScreenPoint(vec2& worldPosition);

		void OnKeyPressed(const KeyDownEvent* event);
	};

}}