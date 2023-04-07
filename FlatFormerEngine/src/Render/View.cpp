#include "Render/View.h"

namespace FlatFormer { namespace Render {

	View::View(unsigned defaultWidth, unsigned defaultHeight)
	{
		m_DefaultTransform = new Transform2D();
		m_DefaultCamera = new CameraComponent(defaultWidth, defaultHeight);

		m_CurrentTransform = m_DefaultTransform;
		m_CurrentCamera = m_DefaultCamera;

		this->RegisterEventCallback(&View::OnKeyPressed);
	}


	View::~View()
	{
		delete m_DefaultTransform;
		delete m_DefaultCamera;
	}


	Point View::ConvertToScreenPoint(vec2& worldPosition)
	{
		/// calculate the position of the point in camera coordinates (observation transformation)
		vec2 viewPosition = m_CurrentTransform->ConvertToLocal(worldPosition, m_CurrentCamera->GetScale());

		Point screenPoint = { m_CurrentCamera->width / 2 + roundf(viewPosition.x),
							  m_CurrentCamera->height / 2 - roundf(viewPosition.y) };

		return screenPoint;
	}

	void View::OnKeyPressed(const KeyDownEvent* event)
	{
		switch (event->keyCode)
		{
		case FFE_KEY_W:
			m_CurrentTransform->Translate(vec2(0, 10));
			break;

		case FFE_KEY_S:
			m_CurrentTransform->Translate(vec2(0, -10));
			break;

		case FFE_KEY_A:
			m_CurrentTransform->Translate(vec2(-10, 0));
			break;

		case FFE_KEY_D:
			m_CurrentTransform->Translate(vec2(10, 0));
			break;

		case FFE_KEY_Q:
			m_CurrentTransform->Rotate(m_CurrentTransform->GetWorldRotation() - 10);
			break;

		case FFE_KEY_E:
			m_CurrentTransform->Rotate(m_CurrentTransform->GetWorldRotation() + 10);
			break;

		case FFE_KEY_KP_PLUS:
			m_CurrentCamera->Scale(0.01f);
			break;

		case FFE_KEY_KP_MINUS:
			m_CurrentCamera->Scale(-0.01f);
			break;
		}
	}

}}