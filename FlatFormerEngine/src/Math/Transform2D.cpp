#include "Math/Transform2D.h"

namespace FlatFormer {

	Transform2D::Transform2D(std::string name) :
		root(this),
		parent(nullptr),
		localPosition(vec2::Zero()),
		worldPosition(localPosition),
		localRotation(0.0f),
		worldRotation(localRotation),
		localScale(vec2::One()),
		worldScale(localScale),
		hasChanged(false),
		childs()
	{
		FFE_DEFINE_LOGGER(name.c_str())
	}


	Transform2D::~Transform2D()
	{
	}


	void Transform2D::AttachChild(Transform2D* child)
	{
		if (child->parent == this || child == this)
			return;

		child->DetachSelf();

		/// since child goes into local space, new local parameters need to be computed
		///
		/// note: getting world parameters occurs through methods,
		/// because these characteristics may not have been relevant at the time the binding was called,
		/// so they will be updated if necessary
		child->localPosition = child->GetWorldPosition() - this->GetWorldPosition();
		child->localRotation = this->GetLocalRotation() - child->GetLocalRotation();
		child->localScale	 = child->GetLocalScale() * (vec2::One() / this->GetWorldScale());

		child->parent = this;
		child->root = this->root;

		childs.push_back(child);
	}


	void Transform2D::AttachSelf(Transform2D* transform)
	{
		transform->AttachChild(this);
	}


	void Transform2D::DetachChild(Transform2D* child)
	{
		if (child->parent == this)
		{
			/// since child goes into global space, its local parameters need to be changed.
			/// note: getting world parameters occurs through methods,
			/// because these parameters may not have been relevant at the time of the unbind call,
			/// so they will be updated if necessary
			child->localPosition = child->GetWorldPosition();
			child->localRotation = child->GetWorldRotation();
			child->localScale	 = child->GetWorldScale();

			/// in order for the parameters to be updated correctly,
			/// you need to unbind from the parent after the transformations,
			/// because retrieving child's world parameters can be followed by a call to the old parent
			child->parent = nullptr;;
			child->root = child;

			auto iterator = std::find(childs.begin(), childs.end(), child);
			childs.erase(iterator);
		}
	}


	void Transform2D::DetachSelf()
	{
		if (parent != nullptr)
		{
			parent->DetachChild(this);
		}
	}


	void Transform2D::DetachAllChilds()
	{
		while (!childs.empty())
		{
			Transform2D* child = childs.front();
			DetachChild(child);
		}
	}


	bool Transform2D::IsChildOf(Transform2D* transform)
	{
		return this->parent == transform;
	}


	Transform2D* Transform2D::GetChild(int childIndex)
	{
		if (childIndex >= 0 && childIndex < childs.size())
			return childs[childIndex];

		return nullptr;
	}


	void Transform2D::Translate(vec2 increment)
	{
		localPosition += increment;
		hasChanged = true;
		SetChildsFlags();
	}


	void Transform2D::TranslateAt(vec2 newPosition)
	{
		localPosition = newPosition;
		hasChanged = true;
		SetChildsFlags();
	}


	void Transform2D::Rotate(float increment)
	{
		localRotation += increment;
		hasChanged = true;
		SetChildsFlags();
	}


	void Transform2D::RotateAt(float newRotation)
	{
		localRotation = newRotation;
		hasChanged = true;
		SetChildsFlags();
	}

	void Transform2D::Scale(vec2 increment)
	{
		localScale += increment;
		hasChanged = true;
		SetChildsFlags();
	}


	void Transform2D::ScaleAt(vec2 newScale)
	{
		localScale = newScale;
		hasChanged = true;
		SetChildsFlags();
	}


	void Transform2D::SetChildsFlags()
	{
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->hasChanged = true;
			childs[i]->SetChildsFlags();
		}
	}


	// Returns the world position of a node
	// note: uses recursive node traversal to recalculate a transform
	// if it is out of date since it was last accessed
	vec2 Transform2D::GetWorldPosition()
	{
		if (hasChanged)
		{
			RecalculateTransformation();
		}
		return worldPosition;
	}


	// Returns the world rotation of a node
	// note: uses recursive node traversal to recalculate a transform
	// if it is out of date since it was last accessed
	float Transform2D::GetWorldRotation()
	{
		if (hasChanged)
		{
			RecalculateTransformation();
		}
		return worldRotation;
	}


	// Returns the world scale of a node
	// note: uses recursive node traversal to recalculate a transform
	// if it is out of date since it was last accessed
	vec2 Transform2D::GetWorldScale()
	{
		if (hasChanged)
		{
			RecalculateTransformation();
		}
		return worldScale;
	}

	void Transform2D::RecalculateTransformation()
	{
		if (IsRoot())
		{
			worldPosition = localPosition;
			worldRotation = localRotation;
			worldScale = localScale;
			hasChanged = false;
			return;
		}
		
		/// recursive hierarchy update
		worldRotation = localRotation + parent->GetWorldRotation();

		worldScale = localScale * parent->GetWorldScale();

		/// the order is important: 1 - scale, 2 - rotation, 3 - translation
		worldPosition = parent->worldPosition + vec2::Rotate(parent->worldScale * localPosition, -ToRadians(parent->worldRotation));

		/// clear the flag becuase transformation is in actual state
		hasChanged = false;
	}


	void Transform2D::ResetPosition()
	{
		TranslateAt(vec2::Zero());
	}


	void Transform2D::ResetRotation()
	{
		RotateAt(0.0f);
	}


	void Transform2D::ResetScale()
	{
		ScaleAt(vec2(1, 1));
	}
	

	vec2 Transform2D::ConvertToLocal(vec2& worldPositionToConvert, float scaleCoeff)
	{
		return vec2::Rotate( scaleCoeff * (worldPositionToConvert - GetWorldPosition()), ToRadians(GetWorldRotation()));
	}


	vec2 Transform2D::ConvertToWorld(const vec2& localPositionToConvert)
	{
		return GetWorldPosition() + vec2::Rotate(GetWorldScale() * localPositionToConvert, -ToRadians(GetWorldRotation()));
	}


	vec2 Transform2D::TransformDirection(vec2 localPositionToConvert)
	{
		return vec2::Rotate(localPositionToConvert, -ToRadians(GetWorldRotation()));
	}


	vec2 Transform2D::TransformVector(vec2 localPositionToConvert)
	{
		return vec2::Rotate(GetWorldScale() * localPositionToConvert, -ToRadians(GetWorldRotation()));
	}


	vec2 Transform2D::TransformPoint(vec2 localPositionToConvert)
	{
		return GetWorldPosition() + vec2::Rotate(GetWorldScale() * localPositionToConvert, -ToRadians(GetWorldRotation()));
	}


	vec2 Transform2D::InverseTransformDirection(vec2 worldPositionToConvert)
	{
		return TransformDirection(worldPositionToConvert - GetWorldPosition());
	}


	vec2 Transform2D::InverseTransformVector(vec2 worldPositionToConvert)
	{
		return TransformVector(worldPositionToConvert - GetWorldPosition());
	}


	vec2 Transform2D::InverseTransformPoint(vec2 worldPositionToConvert)
	{
		return TransformPoint(worldPositionToConvert - GetWorldPosition());
	}
}
