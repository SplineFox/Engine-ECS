#pragma once

#include "Engine/API.h"
#include <vector>
#include "Math/vec2.h"
#include "Math/FFMath.h"

namespace FlatFormer {

	class Transform2D
	{
	#pragma region MyRegion
		using ChildsVector = std::vector<Transform2D*>;
	#pragma endregion


	private:
		Transform2D* root;
		Transform2D* parent;

		vec2 localPosition;
		vec2 worldPosition;

		float localRotation;
		float worldRotation;

		vec2 localScale;
		vec2 worldScale;

		bool hasChanged; /// dirty flag

		ChildsVector childs;

		FFE_DECLARE_LOGGER

	public:
		Transform2D(std::string name = "");
		~Transform2D();

		void AttachChild(Transform2D* child);
		void AttachSelf(Transform2D* transform);
		void DetachChild(Transform2D* child);
		void DetachSelf();
		void DetachAllChilds();
		bool IsChildOf(Transform2D* transform);
		Transform2D* GetChild(int childIndex);

		void Translate(vec2 newPosition);
		void TranslateAt(vec2 newPosition);

		void Rotate(float newRotation);
		void RotateAt(float newRotation);

		void Scale(vec2 newScale);
		void ScaleAt(vec2 newScale);

		vec2 GetWorldPosition();
		float GetWorldRotation();
		vec2 GetWorldScale();

		void ResetPosition();
		void ResetRotation();
		void ResetScale();

		vec2 ConvertToLocal(vec2& worldPositionToConvert, float scaleCoeff);
		vec2 ConvertToWorld(const vec2& localPositionToConvert);

		vec2 TransformDirection(vec2 localPositionToConvert);
		vec2 TransformVector(vec2 localPositionToConvert);
		vec2 TransformPoint(vec2 localPositionToConvert);

		vec2 InverseTransformDirection(vec2 worldPositionToConvert);
		vec2 InverseTransformVector(vec2 worldPositionToConvert);
		vec2 InverseTransformPoint(vec2 worldPositionToConvert);

		inline Transform2D*	GetParent()		const { return parent; }
		inline Transform2D* GetRoot()		const { return root; }
		inline int			GetChildCount()	const { return childs.size(); }
		inline bool			IsRoot()		const { return (this == root); }
		inline bool			HasChanged()	const { return hasChanged; }

		inline vec2		GetLocalPosition()	const { return localPosition; }
		inline float	GetLocalRotation()	const { return localRotation; }
		inline vec2		GetLocalScale()		const { return localScale; }

		void Present()
		{
			auto localPosition = GetLocalPosition();
			auto worldPosition = GetWorldPosition();
			auto localRotation = GetLocalRotation();
			auto worldRotation = GetWorldRotation();
			auto localScale = GetLocalScale();
			auto worldScale = GetWorldScale();

			FFE_LOG_TRACE("")
			FFE_LOG_TRACE("")
			FFE_LOG_TRACE("Local-----------------")
			FFE_LOG_TRACE("T		{}, {}", localPosition.x, localPosition.y)
			FFE_LOG_TRACE("S		{}, {}", localScale.x, localScale.y)
			FFE_LOG_TRACE("R		{}", localRotation)
			FFE_LOG_TRACE("World-----------------")
			FFE_LOG_TRACE("T		{}, {}", worldPosition.x, worldPosition.y)
			FFE_LOG_TRACE("S		{}, {}", worldScale.x, worldScale.y)
			FFE_LOG_TRACE("R		{}", worldRotation)
		}

	private:
		void SetChildsFlags();
		void RecalculateTransformation();

	};

}
