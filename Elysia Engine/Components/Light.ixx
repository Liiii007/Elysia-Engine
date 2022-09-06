#include <stdafx.h>
export module Light;
import ECS;

namespace Component {
	using namespace DirectX;
	export class Light : public ComponentBase {

	public:
		std::string name;

		virtual void DrawEditorUI();

		Light(Entity* entity);

		//Parse init data from json file
		static void Parse(Entity& entity, const rapidjson::Value& parm);

		Light* SetPosition(const XMFLOAT3& position);
		Light* SetTarget(const XMFLOAT3& direction);
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetDirection();
		XMFLOAT3 GetColor();
		float GetPower();

		Entity& ReturnParentEntity();

	private:
		XMFLOAT3 mPosition{};
		XMFLOAT3 mTarget{};
		XMFLOAT3 mLightColor{ 1,1,1 };
		float mLightPower{ 1 };
		//XMFLOAT3 mDirection;
	};

	Light::Light(Entity* entity) : ComponentBase(entity) {
		
	}

	void Light::Parse(Entity& entity, const rapidjson::Value& parm) {
		const XMFLOAT3 target{
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

	Light* Light::SetPosition(const XMFLOAT3& position) {
		mPosition = position;
		return this;
	}

	Light* Light::SetTarget(const XMFLOAT3& target) {
		mTarget = target;
		return this;
	}

	Entity& Light::ReturnParentEntity() {
		return *parentEntity;
	}

	XMFLOAT3 Light::GetPosition() {
		//XMVECTOR pos = parentEntity->translation.position;
		mPosition = parentEntity->translation.position;
		return mPosition;
	}

	XMFLOAT3 Light::GetDirection() {
		XMFLOAT3 direction{ mTarget.x - mPosition.x, mTarget.y - mPosition.y, mTarget.z - mPosition.z };
		return direction;
	}

	XMFLOAT3 Light::GetColor() {
		return mLightColor;
	}

	float Light::GetPower() {
		return mLightPower;
	}
}