#pragma once

#include "ECS/System.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/ComponentManagerUser.h"

#include "ECS/Render/AnimatorComponent.h"
#include "ECS/Common/TransformComponent.h"

namespace FlatFormer {

	class AnimationRenderSystem :
		public ECS::System<AnimationRenderSystem>,
		public Service::FFE_SERV_USER_View,
		public Service::FFE_SERV_USER_ComponentManager
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		AnimationRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
			this->m_SystemFilter.SetItemByType<AnimatorComponent>(true);
			this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);
		}


		~AnimationRenderSystem()
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
				auto animator = componentManager.GetComponent<AnimatorComponent>(entity);
				auto transform = componentManager.GetComponent<ECS::TransformComponent>(entity);

				animator->Update(deltaTime);

				RenderAnimationFrame(animator, transform);
			}
		}


		void PostUpdate(float deltaTime) override
		{
		}


		void RenderAnimationFrame(AnimatorComponent* animatorComponent, ECS::TransformComponent* transform)
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;

			auto frame = animatorComponent->GetCurrnetAnimation().GetCurrentFrame();
			auto imagePayload = std::static_pointer_cast<Asset::ImageAssetPayloadData>(frame.spriteHandle->GetPayload());

			Point spriteSize = { frame.width * camera->GetScale(), frame.height * camera->GetScale() };

			vec2 worldPoint = transform->m_transform.GetWorldPosition();
			Point destPoint = view.ConvertToScreenPoint(worldPoint);

			Rect destRect = { destPoint.x - spriteSize.x / 2,
							  destPoint.y - spriteSize.y / 2,
							  spriteSize.x,
							  spriteSize.y };

			Point rotationCenter = { spriteSize.x / 2,
									 spriteSize.y / 2 };

			m_GraphicsContext->DrawImage(imagePayload, destRect, transform->m_transform.GetWorldRotation(), rotationCenter, animatorComponent->flipType, animatorComponent->color);
		}

	};
}