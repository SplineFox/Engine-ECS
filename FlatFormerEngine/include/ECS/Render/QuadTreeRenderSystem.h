#pragma once



#include "ECS/System.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/ComponentManagerUser.h"

#include "ECS/Collision/QuadTreeDebugComponent.h"

namespace FlatFormer {

	class QuadTreeRenderSystem :
		public ECS::System<QuadTreeRenderSystem>,
		public Service::FFE_SERV_USER_View,
		public Service::FFE_SERV_USER_ComponentManager
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		QuadTreeRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
			this->m_SystemFilter.SetItemByType<Collision::QuadTreeDebugComponent>(true);
		}


		~QuadTreeRenderSystem()
		{
		}


		void PreUpdate(float deltaTime) override
		{
		}


		void Update(float deltaTime) override
		{
			auto& entities = this->GetEntities();
			auto& componentManager = this->GetComponentManagerService();

			for (auto& entity : entities)
			{
				auto quadTree = componentManager.GetComponent<Collision::QuadTreeDebugComponent>(entity);
				RenderQuadTree(quadTree);
			}
		}


		void PostUpdate(float deltaTime) override
		{
		}


		void RenderQuadTree(Collision::QuadTreeDebugComponent* treeInfo)
		{
			auto& view = this->GetViewService();

			m_GraphicsContext->SetDrawColor(ORANGE_COLOR);

			std::vector<Collision::FF_AABB> aabbList;

			treeInfo->m_quadTree->FillDebug(aabbList);

			Point points[4];
			vec2 worldPoints[4];
			for (size_t i = 0; i < aabbList.size(); i++)
			{
				worldPoints[0] = aabbList[i].minCorner;
				worldPoints[1] = vec2(aabbList[i].minCorner.x, aabbList[i].maxCorner.y);
				worldPoints[2] = vec2(aabbList[i].maxCorner);
				worldPoints[3] = vec2(aabbList[i].maxCorner.x, aabbList[i].minCorner.y);

				points[0] = view.ConvertToScreenPoint(worldPoints[0]);
				points[1] = view.ConvertToScreenPoint(worldPoints[1]);
				points[2] = view.ConvertToScreenPoint(worldPoints[2]);
				points[3] = view.ConvertToScreenPoint(worldPoints[3]);

				m_GraphicsContext->DrawLine(points[0].x, points[0].y, points[1].x, points[1].y);
				m_GraphicsContext->DrawLine(points[1].x, points[1].y, points[2].x, points[2].y);
				m_GraphicsContext->DrawLine(points[2].x, points[2].y, points[3].x, points[3].y);
				m_GraphicsContext->DrawLine(points[3].x, points[3].y, points[0].x, points[0].y);
			}
		}
	};
}