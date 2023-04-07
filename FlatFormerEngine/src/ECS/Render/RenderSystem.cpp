#include "ECS/Render/RenderSystem.h"

#include "Window/IWindow.h"

namespace FlatFormer {

	RenderSystem::RenderSystem(IGraphicsContext* graphicsContext, bool useDebugDrawing) :
		m_GraphicsContext(graphicsContext),
		m_IsDebugDrawing(useDebugDrawing),
		m_IsRenderCollision(true),
		m_IsRenderTree(true)
	{
		cameraTransform = new Transform2D();
		cameraComponent = new CameraComponent();

		cameraComponent->width = 1280;
		cameraComponent->height = 640;

		Demo* demo = new Demo(cameraTransform, cameraComponent);
	}


	RenderSystem::~RenderSystem()
	{
	}


	void RenderSystem::PreUpdate(float deltaTime)
	{
		m_GraphicsContext->Clear(CLEAR_COLOR);
	}


	void RenderSystem::Update(float deltaTime)
	{
		RenderGrid();
		RenderOrigin();
		
		auto& componentManager = this->GetComponentManagerService();

		auto& colliders = componentManager.GetComponentsOfType<Collision::ColliderComponent>();
		for (size_t i = 0; i < colliders.size(); i++)
		{
			RenderCollider(colliders[i]);
		}

		if (m_IsRenderCollision)
		{
			auto& collisions = componentManager.GetComponentsOfType<Collision::CollisionEventComponent>();
			for (size_t i = 0; i < collisions.size(); i++)
			{
				RenderCollisionInfo(collisions[i]);
			}
		}

		if (m_IsRenderTree)
		{
			auto& trees = componentManager.GetComponentsOfType<Collision::QuadTreeDebugComponent>();
			for (size_t i = 0; i < trees.size(); i++)
			{
				RenderTreeInfo(trees[i]);
				
			}
		}

		RenderPointer();
	}


	void RenderSystem::PostUpdate(float deltaTime)
	{
		m_GraphicsContext->Present();
	}


	void RenderSystem::RenderGrid()
	{
		float defaultCellSize = 100.f;

		float cellSize = roundf(defaultCellSize * cameraComponent->GetScale());
		if (cellSize < 10)
		{
			defaultCellSize *= 10.f;
			cellSize = roundf(defaultCellSize * cameraComponent->GetScale());
		}

		unsigned int cellsCountX = cameraComponent->width / (float)(cellSize);
		unsigned int cellsCountY = cameraComponent->height / (float)(cellSize);

		vec2 cameraWorldPosition = cameraTransform->GetWorldPosition();
		vec2 cameraGridWorldOffset = vec2(roundf(cameraWorldPosition.x / defaultCellSize),
										  roundf(cameraWorldPosition.y / defaultCellSize)) * defaultCellSize;

		vec2 originGridWorldPosition = vec2((cellsCountX / 2) * -defaultCellSize,
											(cellsCountY / 2) * defaultCellSize);
		vec2 cameraGridWorldPosition = originGridWorldPosition + cameraGridWorldOffset;
		Point cornerScreenPoint = ConvertToScreenPoint(cameraGridWorldPosition);

		m_GraphicsContext->SetDrawColor(GRID_COLOR);
		for (int i = 0; i < cellsCountX; i++)
		{
			m_GraphicsContext->DrawLine(cornerScreenPoint.x, 0, cornerScreenPoint.x, cameraComponent->height);
			cornerScreenPoint.x += cellSize;
		}
		for (int j = 0; j < cellsCountY; j++)
		{
			m_GraphicsContext->DrawLine(0, cornerScreenPoint.y, cameraComponent->width, cornerScreenPoint.y);
			cornerScreenPoint.y += cellSize;
		}
	}


	void RenderSystem::RenderOrigin()
	{
		vec2 originPosition = vec2::Zero();
		Point originPoint = ConvertToScreenPoint(originPosition);

		/// OX axis
		m_GraphicsContext->SetDrawColor(DARK_RED_COLOR);
		m_GraphicsContext->DrawLine(0, originPoint.y, cameraComponent->width - 5, originPoint.y);
		m_GraphicsContext->DrawLine(cameraComponent->width - 5, originPoint.y, cameraComponent->width - 5 - 10, originPoint.y - 10);
		m_GraphicsContext->DrawLine(cameraComponent->width - 5, originPoint.y, cameraComponent->width - 5 - 10, originPoint.y + 10);
		
		/// OY axis
		m_GraphicsContext->SetDrawColor(DARK_GREEN_COLOR);
		m_GraphicsContext->DrawLine(originPoint.x, 0 + 5, originPoint.x, cameraComponent->height);
		m_GraphicsContext->DrawLine(originPoint.x, 0 + 5, originPoint.x - 10, 0 + 5 + 10);
		m_GraphicsContext->DrawLine(originPoint.x, 0 + 5, originPoint.x + 10, 0 + 5 + 10);
	}


	// Отрисовка перекретья камеры
	void RenderSystem::RenderPointer()
	{
		m_GraphicsContext->SetDrawColor(WHITE_COLOR);
		Point centerPoint = { cameraComponent->width / 2, cameraComponent->height / 2 };
		/// draw cmaera crosshair
		m_GraphicsContext->DrawLine(centerPoint.x - 5, centerPoint.y, centerPoint.x + 5, centerPoint.y);
		m_GraphicsContext->DrawLine(centerPoint.x, centerPoint.y - 5, centerPoint.x, centerPoint.y + 5);
	}


	void RenderSystem::RenderCollider(Collision::ColliderComponent* collider)
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


	void RenderSystem::RenderCircleCollider(Collision::CircleCollider* collider)
	{
		vec2 worldPosition = collider->GetWorldPosition();
		Point circleCenter = ConvertToScreenPoint(worldPosition);

		m_GraphicsContext->DrawCircle(circleCenter.x, circleCenter.y, collider->GetRadius() * cameraComponent->GetScale());
		m_GraphicsContext->DrawLine(circleCenter.x, circleCenter.y, circleCenter.x+ collider->GetRadius()* cameraComponent->GetScale(), circleCenter.y);
		TEST(worldPosition);


	}


	void RenderSystem::RenderPolygonCollider(Collision::PolygonCollider* collider)
	{
		std::vector<Point> points(collider->GetVerticesCount());
		vec2 worldVertex;

		for (size_t i = 0; i < points.size(); i++)
		{
			worldVertex = collider->GetWorldVertex(i);
			points[i] = ConvertToScreenPoint(worldVertex);
		}

		for (size_t i = 0; i < points.size() - 1; i++)
		{
			m_GraphicsContext->DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}

		m_GraphicsContext->DrawLine(points[0].x, points[0].y, points[points.size() - 1].x, points[points.size() - 1].y);

		vec2 worldCenter = collider->GetCentroid();
		TEST(worldCenter);
	}


	void RenderSystem::RenderCollisionInfo(Collision::CollisionEventComponent* collisionInfo)
	{
		m_GraphicsContext->SetDrawColor(MAGENTA_COLOR);

		vec2 worldPosition;
		Point point;

		for (size_t i = 0; i < collisionInfo->m_contactInformation->pointCount; i++)
		{
			worldPosition = collisionInfo->m_contactInformation->m_contactPoints[i];
			point = ConvertToScreenPoint(worldPosition);
			m_GraphicsContext->DrawCircle(point.x, point.y, 4);
		}
	}


	void RenderSystem::RenderTreeInfo(Collision::QuadTreeDebugComponent* treeInfo)
	{
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

			points[0] = ConvertToScreenPoint(worldPoints[0]);
			points[1] = ConvertToScreenPoint(worldPoints[1]);
			points[2] = ConvertToScreenPoint(worldPoints[2]);
			points[3] = ConvertToScreenPoint(worldPoints[3]);

			m_GraphicsContext->DrawLine(points[0].x, points[0].y, points[1].x, points[1].y);
			m_GraphicsContext->DrawLine(points[1].x, points[1].y, points[2].x, points[2].y);
			m_GraphicsContext->DrawLine(points[2].x, points[2].y, points[3].x, points[3].y);
			m_GraphicsContext->DrawLine(points[3].x, points[3].y, points[0].x, points[0].y);
		}
	}

	Point RenderSystem::ConvertToScreenPoint(vec2& worldPosition)
	{
		vec2 viewPosition = cameraTransform->ConvertToLocal(worldPosition, cameraComponent->GetScale());

		Point screenPoint = { cameraComponent->width / 2 + roundf(viewPosition.x),
							  cameraComponent->height / 2 - roundf(viewPosition.y) };

		return screenPoint;
	}
}