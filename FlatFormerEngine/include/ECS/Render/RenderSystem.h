#pragma once

#include "ECS/System.h"
#include "ECS/Render/CameraComponent.h"
#include "ECS/Collision/Colliders/ColliderComponent.h"
#include "ECS/Collision/Colliders/CircleCollider.h"
#include "ECS/Collision/Colliders/PolygonCollider.h"

#include "ECS/Collision/QuadTreeDebugComponent.h"
#include "ECS/Collision/CollisionEventComponent.h"

#include "Collision/QuadTree/QuadTree.h"
#include "Collision/FF_AABB.h"

#include "Engine/Service/ComponentManagerUser.h"

#include "Render/IGraphicsContext.h"

#include "Math/Transform2D.h"


#include "Event/IEventListener.h"
#include "Window/KeyCodes.h"
#include "Window/InputEvents.h"

namespace FlatFormer {

	class RenderSystem : 
		public ECS::System<RenderSystem>,
		public Event::IEventListener,
		public Service::FFE_SERV_USER_ComponentManager
	{
	public:
		IGraphicsContext* m_GraphicsContext;
		bool m_IsDebugDrawing;
		bool m_IsRenderCollision;
		bool m_IsRenderTree;

		Transform2D* cameraTransform;
		CameraComponent* cameraComponent;

	public:
		RenderSystem(IGraphicsContext* graphicsContext, bool useDebugDrawing = true);
		~RenderSystem();

		void PreUpdate(float deltaTime)		override;
		void Update(float deltaTime)		override;
		void PostUpdate(float deltaTime)	override;

		bool IsDebugDrawing() const { return m_IsDebugDrawing; }
		void SetDebugDrawing(bool debugDrawingState) { m_IsDebugDrawing = debugDrawingState; }

	private:
		void RenderGrid();
		void RenderOrigin();
		void RenderPointer();
		void RenderCollider(Collision::ColliderComponent* collider);
		void RenderCircleCollider(Collision::CircleCollider* collider);
		void RenderPolygonCollider(Collision::PolygonCollider* collider);
		void RenderCollisionInfo(Collision::CollisionEventComponent* collisionInfo);

		void RenderTreeInfo(Collision::QuadTreeDebugComponent* treeInfo);

		Point ConvertToScreenPoint(vec2& worldPosition);

		void TEST(vec2& point);

		void OnKeyPressed(const KeyDownEvent* event);
	};

}