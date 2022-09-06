#include <stdafx.h>
import ECS;


export module Translation;
namespace Component {

	export class Translation final : public ComponentBase {
	public:
		Translation(Entity* entity) : ComponentBase(entity) {};
		static void Parse(Entity& entity, const rapidjson::Value& parm);
		virtual void DrawEditorUI() override;

		//Translation Relate
		Translation* SetLocation(float x, float y, float z) {
			position = DirectX::XMFLOAT3{ x,y,z };
			return this;
		}
		Translation* SetLocation(const DirectX::XMFLOAT3& position) {
			this->position = position;
			return this;
		}
		Translation* SetRotation(float x, float y, float z) {
			rotation = DirectX::XMFLOAT3{ x,y,z };
			return this;
		}
		Translation* SetRotation(const DirectX::XMFLOAT3& rotation) {
			this->rotation = rotation;
			return this;
		}
		Translation* SetUniformScale(float scale) {
			this->scale = DirectX::XMFLOAT3{ scale,scale,scale };
			return this;
		}
		Translation* SetScale(float x, float y, float z) {
			this->scale = DirectX::XMFLOAT3{ x,y,z };
			return this;
		}
		Translation* SetScale(const DirectX::XMFLOAT3& scale) {
			this->scale = scale;
			return this;
		}
		DirectX::XMFLOAT3 GetLocation() {
			return position;
		}
		DirectX::XMFLOAT3 GetRotation() {
			return position;
		}
		DirectX::XMFLOAT3 GetScale() {
			return position;
		}

		Entity& ReturnParentEntity() {
			return *parentEntity;
		}

		//DataMember
		DirectX::XMFLOAT3 position{ 0,0,0 };
		DirectX::XMFLOAT3 rotation{ 0,0,0 };
		DirectX::XMFLOAT3 scale{ 1,1,1 };
	};

	//Parse function for json parse
	void Translation::Parse(Entity& entity, const rapidjson::Value& parm) {
		const rapidjson::Value& locationValue = parm["Location"];
		const rapidjson::Value& rotationValue = parm["Rotation"];
		const rapidjson::Value& scaleValue    = parm["Scale"];
		const DirectX::XMFLOAT3 location{
			locationValue[0].GetFloat(),
			locationValue[1].GetFloat(),
			locationValue[2].GetFloat()
		};

		const DirectX::XMFLOAT3 rotation{
			rotationValue[0].GetFloat(),
			rotationValue[1].GetFloat(),
			rotationValue[2].GetFloat()
		};

		const DirectX::XMFLOAT3 scale{
			scaleValue[0].GetFloat(),
			scaleValue[1].GetFloat(),
			scaleValue[2].GetFloat()
		};

		entity.AppendComponent<Translation>()
			.GetComponent<Translation>()
				->SetLocation(location)
				->SetRotation(rotation)
				->SetScale(scale);
	}

	//ImGui Relate
	void Translation::DrawEditorUI(){
		//Translation
		ImGui::DragFloat3("Location", &position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);
		ImGui::DragFloat3("Scale", &scale.x, 0.1f);
		ImGui::Spacing();
	}
}