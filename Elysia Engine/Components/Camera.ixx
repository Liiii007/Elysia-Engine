#include <stdafx.h>
import ECS;
import Log;

//Dependency
import Translation;

export module Camera;
namespace Component {
	export class Camera final : public ComponentBase {
	public:
		Camera(Entity* entity) : ComponentBase(entity) {

		};

		static void Parse(Entity& entity, const rapidjson::Value& parm);
		static Camera* activeCamera;
		static int activePriority;
		virtual void DrawEditorUI() override;

		std::string targetEntityName;
		DirectX::XMFLOAT3 targetDirection;
		DirectX::XMFLOAT3 up;
		int priority{ 0 };

		DirectX::XMMATRIX getViewMatrix() {
			auto targetEntity = Entity::GetEntity(targetEntityName);

			if (Entity::GetEntity(targetEntityName)) {
				targetDirection = targetEntity->GetComponent<Translation>()->GetLocation();
			}

			auto pos{ parentEntity->GetComponent<Translation>()->GetLocation() };

			// Build the view matrix.
			DirectX::XMVECTOR posV = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);

			DirectX::XMVECTOR targetV{};
	
			targetV = DirectX::XMVectorSet(
				targetDirection.x,
				targetDirection.y,
				targetDirection.z,
				1
			);

			DirectX::XMVECTOR upV = DirectX::XMVectorSet(up.x, up.y, up.z,0);

			return DirectX::XMMatrixLookAtLH(posV, targetV, upV);

		}

		Camera* SetTargetEntity(std::string name) {
			this->targetEntityName = name;
			return this;
		}
		Camera* SetDirection(DirectX::XMFLOAT3 direction) {
			targetDirection = direction;
			return this;
		}
		Camera* SetUp(DirectX::XMFLOAT3 up) {
			this->up = up;
			return this;
		}
		Camera* SetPriority(int p) {
			if (p > activePriority) {
				activePriority = p;
				activeCamera = this;
			}

			this->priority = p;
			return this;
		}
		DirectX::XMFLOAT3 GetDirection() {
			//target Entity exist->update targetDirection
			if (Entity::GetEntity(targetEntityName) != nullptr) { 
				auto targetLocation = Entity::GetEntity(targetEntityName)->GetComponent<Translation>()->GetLocation();
				auto pos{ parentEntity->GetComponent<Translation>()->GetLocation() };

				targetDirection.x = targetLocation.x - pos.x;
				targetDirection.y = targetLocation.y - pos.y;
				targetDirection.z = targetLocation.z - pos.z;
			}

			return targetDirection;
		}
		DirectX::XMFLOAT3 GetPosition() {
			if (!parentEntity->HasComponent<Translation>()) {
				Log::Error("No Translation Component");
				return DirectX::XMFLOAT3(0, 0, 0);
			}
			else {
				return parentEntity->GetComponent<Translation>()->GetLocation();
			}
			
		}

	};

	void Camera::Parse(Entity& entity, const rapidjson::Value& parm) {
		//Prepare data for component init
		DirectX::XMFLOAT3 targetDirection{};
		DirectX::XMFLOAT3 up{};
		std::string targetEntityName;
		int Priority{};

		if (parm.HasMember("TargetEntity") && strlen(parm["TargetEntity"].GetString()) > 0) {
			targetEntityName = parm["TargetEntity"].GetString();
		}

		if (parm.HasMember("TargetDirection")) {
			targetDirection.x = parm["TargetDirection"].GetArray()[0].GetFloat();
			targetDirection.y = parm["TargetDirection"].GetArray()[1].GetFloat();
			targetDirection.z = parm["TargetDirection"].GetArray()[2].GetFloat();
		}

		if (parm.HasMember("Up")) {
			up.x = parm["Up"].GetArray()[0].GetFloat();
			up.y = parm["Up"].GetArray()[1].GetFloat();
			up.z = parm["Up"].GetArray()[2].GetFloat();
		}

		if (parm.HasMember("Priority")) {
			Priority = parm["Priority"].GetInt();
		}

		//AppendComponent to entity
		entity.AppendComponent<Camera>()
			.GetComponent<Camera>()
			->SetTargetEntity(targetEntityName)
			->SetDirection(targetDirection)
			->SetUp(up)
			->SetPriority(Priority)
			->targetEntityName = targetEntityName;
	}

	//ImGui Relate
	void Camera::DrawEditorUI() {
		if (this == activeCamera) {
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Active Camera");
		}

		std::string Tar = "Target Entity: ";
		Tar += targetEntityName;
		if (Entity::GetEntity(targetEntityName) == nullptr) {
			Tar += "(Invaild)";
			ImGui::TextColored(ImVec4(1, 0, 0, 1), Tar.c_str());
		}
		else {
			ImGui::TextColored(ImVec4(1, 1, 1, 1), Tar.c_str());
		}

		ImGui::DragInt("Priority", &priority);
		SetPriority(priority);
	}

	Camera* Camera::activeCamera{ nullptr };
	int Camera::activePriority{ INT_MIN };
}
