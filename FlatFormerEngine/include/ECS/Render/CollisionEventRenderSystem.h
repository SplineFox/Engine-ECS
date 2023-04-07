#pragma once



#include "ECS/System.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/EntityManagerUser.h"

#include "ECS/Collision/CollisionEventComponent.h"

namespace FlatFormer {

	class CollisionEventRenderSystem :
		public ECS::System<CollisionEventRenderSystem>,
		public Service::FFE_SERV_USER_View,
		public Service::FFE_SERV_USER_ComponentManager,
		public Service::FFE_SERV_USER_EntityManager
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		CollisionEventRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
			this->m_SystemFilter.SetItemByType<Collision::CollisionEventComponent>(true);
		}


		~CollisionEventRenderSystem()
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
				auto collider = componentManager.GetComponent<Collision::CollisionEventComponent>(entity);
				RenderCollisionEvent(collider);
			}
		}


		void PostUpdate(float deltaTime) override
		{
			auto& entities = this->GetEntities();
			for (auto& entity : entities)
			{
				this->GetEntityManagerService().DestroyEnity(entity);
			}
		}


		void RenderCollisionEvent(Collision::CollisionEventComponent* collisionInfo)
		{
			auto& view = this->GetViewService();

			m_GraphicsContext->SetDrawColor(MAGENTA_COLOR);

			vec2 worldPosition;
			Point point;

			for (size_t i = 0; i < collisionInfo->m_contactInformation->pointCount; i++)
			{
				worldPosition = collisionInfo->m_contactInformation->m_contactPoints[i];
				point = view.ConvertToScreenPoint(worldPosition);
				m_GraphicsContext->DrawCircle(point.x, point.y, 4);
			}
		}


		void OnEntityAdded(ECS::EntityID entityId) override
		{
			int x = 1;
		}

		void OnEntityRemoved(ECS::EntityID entityId) override
		{
			int x = 0;
		}
	};
}