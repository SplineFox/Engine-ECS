#pragma once

#include "ECS/System.h"
#include "Render/IGraphicsContext.h"

namespace FlatFormer {

	class RenderEndSystem : public ECS::System<RenderEndSystem>
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		RenderEndSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
		}


		~RenderEndSystem()
		{
		}


		void Update(float deltaTime) override
		{
			m_GraphicsContext->Present();
		}
	};
}