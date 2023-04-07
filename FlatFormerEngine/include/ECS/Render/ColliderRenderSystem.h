#pragma once



#include "ECS/System.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/ComponentManagerUser.h"

#include "ECS/Collision/Colliders/ColliderComponent.h"
#include "ECS/Collision/Colliders/CircleCollider.h"
#include "ECS/Collision/Colliders/PolygonCollider.h"

namespace FlatFormer {

	class ColliderRenderSystem :
		public ECS::System<ColliderRenderSystem>,
		public Service::FFE_SERV_USER_View,
		public Service::FFE_SERV_USER_ComponentManager
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		ColliderRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
			this->m_SystemFilter.SetItemByType<Collision::ColliderComponent>(true);
			this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);
		}


		~ColliderRenderSystem()
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
				auto collider = componentManager.GetComponent<Collision::ColliderComponent>(entity);
				RenderCollider(collider);
			}
		}


		void PostUpdate(float deltaTime) override
		{
		}


		void RenderCollider(Collision::ColliderComponent* collider)
		{
			m_GraphicsContext->SetDrawColor(COLLIDER_COLOR);

			if (collider->m_collider->GetType() == Collision::circle)
			{
				RenderCircleCollider((Collision::CircleCollider*)collider->m_collider);
			}
			else
			{
				RenderPolygonCollider((Collision::PolygonCollider*)collider->m_collider);
			}
		}


		void RenderCircleCollider(Collision::CircleCollider* collider)
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;

			vec2 worldPosition = collider->GetWorldPosition();
			Point circleCenter = view.ConvertToScreenPoint(worldPosition);

			m_GraphicsContext->DrawCircle(circleCenter.x, circleCenter.y, collider->GetRadius() * camera->GetScale());
			m_GraphicsContext->DrawLine(circleCenter.x, circleCenter.y, circleCenter.x + collider->GetRadius()* camera->GetScale(), circleCenter.y);
			DrawCross(worldPosition);
		}


		void RenderPolygonCollider(Collision::PolygonCollider* collider)
		{
			auto& view = this->GetViewService();

			std::vector<Point> points(collider->GetVerticesCount());
			vec2 worldVertex;

			for (size_t i = 0; i < points.size(); i++)
			{
				worldVertex = collider->GetWorldVertex(i);
				points[i] = view.ConvertToScreenPoint(worldVertex);
			}

			for (size_t i = 0; i < points.size() - 1; i++)
			{
				m_GraphicsContext->DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
			}

			m_GraphicsContext->DrawLine(points[0].x, points[0].y, points[points.size() - 1].x, points[points.size() - 1].y);

			vec2 worldCenter = collider->GetCentroid();
			DrawCross(worldCenter);
		}


		void DrawCross(vec2& point)
		{
			auto& view = this->GetViewService();

			auto screenPoint = view.ConvertToScreenPoint(point);
			m_GraphicsContext->DrawLine(screenPoint.x, screenPoint.y, screenPoint.x, screenPoint.y + 2);
			m_GraphicsContext->DrawLine(screenPoint.x, screenPoint.y, screenPoint.x + 2, screenPoint.y);
			m_GraphicsContext->DrawLine(screenPoint.x, screenPoint.y, screenPoint.x, screenPoint.y - 2);
			m_GraphicsContext->DrawLine(screenPoint.x, screenPoint.y, screenPoint.x - 2, screenPoint.y);
		}
	};
}