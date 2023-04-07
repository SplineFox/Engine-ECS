#pragma once

#include "ECS/System.h"
#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"

namespace FlatFormer {

	class RenderBeginSystem : public ECS::System<RenderBeginSystem>,
		Service::FFE_SERV_USER_View
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		RenderBeginSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
		}


		~RenderBeginSystem()
		{
		}


		void Update(float deltaTime) override
		{
			m_GraphicsContext->Clear(CLEAR_COLOR);
		}

	};
}