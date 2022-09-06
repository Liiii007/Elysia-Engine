#include <stdafx.h>
export module Light;
import ECS;
import Translation;

namespace Component {
	export class Light : public ComponentBase {

	public:
		std::string name;

		virtual void DrawEditorUI();

		Light(Entity* entity);

		//Parse init data from json file
		static void Parse(Entity& entity, const rapidjson::Value& parm);

		Light* SetPosition(const DirectX::XMFLOAT3& position);
		Light* SetTarget(const DirectX::XMFLOAT3& direction);
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

	Light::Light(Entity* entity) : ComponentBase(entity) {
		
	}

	void Light::Parse(Entity& entity, const rapidjson::Value& parm) {
		const DirectX::XMFLOAT3 target{
			parm[0].GetFloat(),
			parm[1].GetFloat(),
			parm[2].GetFloat(),
		};

		entity.AppendComponent<Light>()
			.GetComponent<Light>()
			->SetTarget(target);
	}

	void Light::DrawEditorUI() {
		ImGui::Text("Light");
		ImGui::SliderFloat("Power", &mLightPower, 0, 10);
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorPicker3("##MyColor##6", &mLightColor.x, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		ImGui::Spacing();
	}

	Light* Light::SetPosition(const DirectX::XMFLOAT3& position) {
		mPosition = position;
		return this;
	}

	Light* Light::SetTarget(const DirectX::XMFLOAT3& target) {
		mTarget = target;
		return this;
	}

	Entity& Light::ReturnParentEntity() {
		return *parentEntity;
	}

	DirectX::XMFLOAT3 Light::GetPosition() {
		//XMVECTOR pos = parentEntity->translation.position;
		mPosition = parentEntity->GetComponent<Translation>()->position;
		return mPosition;
	}

	DirectX::XMFLOAT3 Light::GetDirection() {
		DirectX::XMFLOAT3 direction{ mTarget.x - mPosition.x, mTarget.y - mPosition.y, mTarget.z - mPosition.z };
		return direction;
	}

	DirectX::XMFLOAT3 Light::GetColor() {
		return mLightColor;
	}

	float Light::GetPower() {
		return mLightPower;
	}
}