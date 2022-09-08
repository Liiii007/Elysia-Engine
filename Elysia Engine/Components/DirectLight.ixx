#include <stdafx.h>
export module DirectLight;
import ECS;
import Translation;
import ShadowMap;

namespace Component {
	export class DirectLight : public ComponentBase {

	public:
		std::string name;

		ShadowMap mShadowMap;

		virtual void DrawEditorUI();

		DirectLight(Entity* entity);

		//Parse init data from json file
		static void Parse(Entity& entity, const rapidjson::Value& parm);

		DirectLight* SetPosition(const DirectX::XMFLOAT3& position);
		DirectLight* SetTarget(const DirectX::XMFLOAT3& direction);
		DirectX::XMFLOAT3 GetPosition();
		DirectX::XMFLOAT3 GetDirection();
		DirectX::XMFLOAT3 GetColor();
		float GetPower();

		Entity& ReturnParentEntity();

	private:
		DirectX::XMFLOAT3 mPosition{};
		DirectX::XMFLOAT3 mTarget{};
		DirectX::XMFLOAT3 mLightColor{ 1,1,1 };
		float mLightPower{ 1 };
		//DirectX::XMFLOAT3 mDirection;
	};

	DirectLight::DirectLight(Entity* entity) : ComponentBase(entity) {
		
	}

	void DirectLight::Parse(Entity& entity, const rapidjson::Value& parm) {
		const DirectX::XMFLOAT3 target{
			parm[0].GetFloat(),
			parm[1].GetFloat(),
			parm[2].GetFloat(),
		};

		entity.AppendComponent<DirectLight>()
			.GetComponent<DirectLight>()
			->SetTarget(target);
	}

	void DirectLight::DrawEditorUI() {
		ImGui::Text("Light");
		ImGui::SliderFloat("Power", &mLightPower, 0, 10);
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorPicker3("##MyColor##6", &mLightColor.x, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		ImGui::Spacing();
	}

	DirectLight* DirectLight::SetPosition(const DirectX::XMFLOAT3& position) {
		mPosition = position;
		return this;
	}

	DirectLight* DirectLight::SetTarget(const DirectX::XMFLOAT3& target) {
		mTarget = target;
		return this;
	}

	Entity& DirectLight::ReturnParentEntity() {
		return *parentEntity;
	}

	DirectX::XMFLOAT3 DirectLight::GetPosition() {
		//XMVECTOR pos = parentEntity->translation.position;
		mPosition = parentEntity->GetComponent<Translation>()->position;
		return mPosition;
	}

	DirectX::XMFLOAT3 DirectLight::GetDirection() {
		DirectX::XMFLOAT3 direction{ mTarget.x - mPosition.x, mTarget.y - mPosition.y, mTarget.z - mPosition.z };
		return direction;
	}

	DirectX::XMFLOAT3 DirectLight::GetColor() {
		return mLightColor;
	}

	float DirectLight::GetPower() {
		return mLightPower;
	}
}