#pragma once

#include "ECS/System.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/ComponentManagerUser.h"

#include "ECS/Render/SpriteComponent.h"
#include "ECS/Common/TransformComponent.h"

namespace FlatFormer {

	class SpriteRenderSystem :
		public ECS::System<SpriteRenderSystem>,
		public Service::FFE_SERV_USER_View,
		public Service::FFE_SERV_USER_ComponentManager
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		SpriteRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
			this->m_SystemFilter.SetItemByType<SpriteComponent>(true);
			this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);
		}


		~SpriteRenderSystem()
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
				auto sprite = componentManager.GetComponent<SpriteComponent>(entity);
				auto transform = componentManager.GetComponent<ECS::TransformComponent>(entity);
				RenderSprite(sprite, transform);
			}
		}


		void PostUpdate(float deltaTime) override
		{
		}


		void RenderSprite(SpriteComponent* spriteComponent, ECS::TransformComponent* transform)
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;

			auto imagePayload = std::static_pointer_cast<Asset::ImageAssetPayloadData>(spriteComponent->sprite.spriteHandle->GetPayload());
			
			Point spriteSize = { spriteComponent->sprite.width * camera->GetScale(), spriteComponent->sprite.height * camera->GetScale() };

			vec2 worldPoint = transform->m_transform.GetWorldPosition();
			Point destPoint = view.ConvertToScreenPoint(worldPoint);

			Rect destRect = { destPoint.x - spriteSize.x / 2,
							  destPoint.y - spriteSize.y / 2,
							  spriteSize.x,
							  spriteSize.y };

			Point rotationCenter = { spriteSize.x / 2,
									 spriteSize.y / 2 };

			m_GraphicsContext->DrawImage(imagePayload, destRect, transform->m_transform.GetWorldRotation(), rotationCenter, spriteComponent->flipType, spriteComponent->color);
		}

	};
}