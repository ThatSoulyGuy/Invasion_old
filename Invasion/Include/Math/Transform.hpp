#pragma once

#include "ECS/Component.hpp"
#include "Math/Vector3.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::ECS;
using namespace Invasion::Util;

namespace Invasion::Math
{
	class Transform : public Component
	{

	public:

		Transform(const Transform&) = delete;
		Transform& operator=(const Transform&) = delete;

		void Translate(const Vector3f& translation)
		{
			localPosition += translation;
		}

		void Rotate(const Vector3f& rotation)
		{
			localRotation += rotation;
		}

		void Scale(const Vector3f& scale)
		{
			localScale += scale;
		}

		Vector3f GetForward() const
		{
			return GetWorldMatrix().r[2];
		}

		Vector3f GetRight() const
		{
			return GetWorldMatrix().r[0];
		}

		Vector3f GetUp() const
		{
			return GetWorldMatrix().r[1];
		}

		Vector3f GetWorldPosition() const
		{
			Matrix modelMatrix = GetWorldMatrix();
			
			return Vector3f(modelMatrix.r[3].m128_f32[0], modelMatrix.r[3].m128_f32[1], modelMatrix.r[3].m128_f32[2]);
		}

		Vector3f GetWorldRotation() const
		{
			Matrix modelMatrix = GetWorldMatrix(); 
			 
			float pitch, yaw, roll; 

			yaw = atan2(modelMatrix.r[1].m128_f32[2], modelMatrix.r[2].m128_f32[2]); 
			pitch = atan2(-modelMatrix.r[0].m128_f32[2], sqrt(pow(modelMatrix.r[0].m128_f32[0], 2) + pow(modelMatrix.r[0].m128_f32[1], 2)));
			roll = atan2(modelMatrix.r[0].m128_f32[1], modelMatrix.r[0].m128_f32[0]); 

			return Vector3f(pitch, yaw, roll);
		}

		Vector3f GetWorldScale() const
		{
			Matrix modelMatrix = GetWorldMatrix();

			float scaleX = 
				sqrt(modelMatrix.r[0].m128_f32[0] * modelMatrix.r[0].m128_f32[0] +
					 modelMatrix.r[0].m128_f32[1] * modelMatrix.r[0].m128_f32[1] +
					 modelMatrix.r[0].m128_f32[2] * modelMatrix.r[0].m128_f32[2]);

			float scaleY = 
				sqrt(modelMatrix.r[1].m128_f32[0] * modelMatrix.r[1].m128_f32[0] +
					 modelMatrix.r[1].m128_f32[1] * modelMatrix.r[1].m128_f32[1] +
					 modelMatrix.r[1].m128_f32[2] * modelMatrix.r[1].m128_f32[2]);

			float scaleZ = 
				sqrt(modelMatrix.r[2].m128_f32[0] * modelMatrix.r[2].m128_f32[0] +
					 modelMatrix.r[2].m128_f32[1] * modelMatrix.r[2].m128_f32[1] +
					 modelMatrix.r[2].m128_f32[2] * modelMatrix.r[2].m128_f32[2]);

			return Vector3f(scaleX, scaleY, scaleZ);
		}

		Vector3f GetLocalPosition() const { return localPosition; }
		Vector3f GetLocalRotation() const { return localRotation; }
		Vector3f GetLocalScale() const { return localScale; }

		void SetLocalPosition(const Vector3f& position) { localPosition = position; }
		void SetLocalRotation(const Vector3f& rotation) { localRotation = rotation; }
		void SetLocalScale(const Vector3f& scale) { localScale = scale; }

		Matrix GetWorldMatrix() const
		{
			Matrix translation = DirectX::XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);
			Matrix rotation = DirectX::XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z);
			Matrix scale = DirectX::XMMatrixScaling(localScale.x, localScale.y, localScale.z);

			Matrix localTransform = scale * rotation * translation;

			if (parent)
				return localTransform * parent->GetWorldMatrix();
			else
				return localTransform;
		}

		static Shared<Transform> Create(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
		{
			class Enabled : public Transform { };
			Shared<Transform> result = std::make_shared<Enabled>();

			result->SetLocalPosition(position);
			result->SetLocalRotation(rotation);
			result->SetLocalScale(scale);

			return std::move(result);
		}
		
		Shared<Transform> parent;

	private:

		Transform() = default;

		Vector3f localPosition;
		Vector3f localRotation;
		Vector3f localScale;
	};
}