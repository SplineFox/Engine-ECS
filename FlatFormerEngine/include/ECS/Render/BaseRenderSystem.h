#pragma once

#include "ECS/System.h"

#include "ECS/Common/TransformComponent.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"

namespace FlatFormer {

	class BaseRenderSystem :
		public ECS::System<BaseRenderSystem>,
		public Service::FFE_SERV_USER_View
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		BaseRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
		}


		~BaseRenderSystem()
		{
		}


		void PreUpdate(float deltaTime) override
		{
		}


		void Update(float deltaTime) override
		{
			RenderGrid();
			RenderOrigin();
			RenderPointer();
		}


		void PostUpdate(float deltaTime) override
		{
		}


		void RenderGrid()
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;
			Transform2D* transform = view.m_CurrentTransform;

			float defaultCellSize = 100.f;

			/// calculate cell size in pixels
			float cellSize = roundf(defaultCellSize * camera->GetScale());
			if (cellSize < 10)
			{
				defaultCellSize *= 10.f;
				cellSize = roundf(defaultCellSize * camera->GetScale());
			}

			unsigned int cellsCountX = camera->width / (float)(cellSize);
			unsigned int cellsCountY = camera->height / (float)(cellSize);

			vec2 cameraWorldPosition = transform->GetWorldPosition();
			vec2 cameraGridWorldOffset = vec2(roundf(cameraWorldPosition.x / defaultCellSize),
				roundf(cameraWorldPosition.y / defaultCellSize)) * defaultCellSize;

			vec2 originGridWorldPosition = vec2((cellsCountX / 2) * -defaultCellSize,
				(cellsCountY / 2) * defaultCellSize);
			vec2 cameraGridWorldPosition = originGridWorldPosition + cameraGridWorldOffset;
			Point cornerScreenPoint = view.ConvertToScreenPoint(cameraGridWorldPosition);

			m_GraphicsContext->SetDrawColor(GRID_COLOR);
			for (int i = 0; i < cellsCountX + 1; i++)
			{
				m_GraphicsContext->DrawLine(cornerScreenPoint.x, 0, cornerScreenPoint.x, camera->height);
				cornerScreenPoint.x += cellSize;
			}
			for (int j = 0; j < cellsCountY + 1; j++)
			{
				m_GraphicsContext->DrawLine(0, cornerScreenPoint.y, camera->width, cornerScreenPoint.y);
				cornerScreenPoint.y += cellSize;
			}
		}


		void RenderOrigin()
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;

			vec2 originPosition = vec2::Zero();
			Point originPoint = view.ConvertToScreenPoint(originPosition);

			/// OX axis
			m_GraphicsContext->SetDrawColor(DARK_RED_COLOR);
			m_GraphicsContext->DrawLine(0, originPoint.y, camera->width - 5, originPoint.y);
			m_GraphicsContext->DrawLine(camera->width - 5, originPoint.y, camera->width - 5 - 10, originPoint.y - 10);
			m_GraphicsContext->DrawLine(camera->width - 5, originPoint.y, camera->width - 5 - 10, originPoint.y + 10);

			/// OY axis
			m_GraphicsContext->SetDrawColor(DARK_GREEN_COLOR);
			m_GraphicsContext->DrawLine(originPoint.x, 0 + 5, originPoint.x, camera->height);
			m_GraphicsContext->DrawLine(originPoint.x, 0 + 5, originPoint.x - 10, 0 + 5 + 10);
			m_GraphicsContext->DrawLine(originPoint.x, 0 + 5, originPoint.x + 10, 0 + 5 + 10);
		}


		void RenderPointer()
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;

			m_GraphicsContext->SetDrawColor(WHITE_COLOR);
			Point centerPoint = { camera->width / 2, camera->height / 2 };

			m_GraphicsContext->DrawLine(centerPoint.x - 5, centerPoint.y, centerPoint.x + 5, centerPoint.y);
			m_GraphicsContext->DrawLine(centerPoint.x, centerPoint.y - 5, centerPoint.x, centerPoint.y + 5);
		}
	};
}