#pragma once

#include "ECS/System.h"

#include "Render/IGraphicsContext.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/ComponentManagerUser.h"

#include "ECS/Render/TextComponent.h"
#include "ECS/Common/TransformComponent.h"

namespace FlatFormer {

	class TextRenderSystem :
		public ECS::System<TextRenderSystem>,
		public Service::FFE_SERV_USER_View,
		public Service::FFE_SERV_USER_ComponentManager
	{
	private:
		IGraphicsContext* m_GraphicsContext;

	public:
		TextRenderSystem(IGraphicsContext* graphicsContext) :
			m_GraphicsContext(graphicsContext)
		{
			this->m_SystemFilter.SetItemByType<TextComponent>(true);
			this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);
		}


		~TextRenderSystem()
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
				auto text = componentManager.GetComponent<TextComponent>(entity);
				auto transform = componentManager.GetComponent<ECS::TransformComponent>(entity);
				RenderText(text, transform);
			}
		}


		void PostUpdate(float deltaTime) override
		{
		}


		void RenderText(TextComponent* textComponent, ECS::TransformComponent* transform)
		{
			auto& view = this->GetViewService();
			auto& camera = view.m_CurrentCamera;

			auto textPayload = std::static_pointer_cast<Asset::FontAssetPayloadData>(textComponent->fontHandle->GetPayload());

			Point textSize = { textComponent->ptSize * textComponent->text.size() * camera->GetScale(), textComponent->ptSize * camera->GetScale() };

			vec2 worldPoint = transform->m_transform.GetWorldPosition();
			Point destPoint = view.ConvertToScreenPoint(worldPoint);

			Rect destRect = { destPoint.x - textSize.x / 2,
							  destPoint.y - textSize.y / 2,
							  textSize.x,
							  textSize.y };

			m_GraphicsContext->DrawFont(textPayload, destRect, textComponent->text, textComponent->color);
		}
	};
}