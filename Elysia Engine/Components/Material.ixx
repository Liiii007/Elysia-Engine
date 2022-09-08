#include <stdafx.h>
#include <3rd/imgui/imgui.h>
export module Material;
import ECS;
import Shader;
import MaterialData;
import Definition;
import Log;

namespace Component {
	using namespace Resource;
	export class Material : public ComponentBase {
	public:
		
		bool enabled{ false };

		Material(Entity* entity);
		void Init(std::string name, std::string shader);

		//Parse init data from json file
		static void Parse(Entity& entity, const rapidjson::Value& parm);

		virtual void DrawEditorUI() override;

		std::shared_ptr<Shader> getShader();
		void SetShader(const std::string name);

		MaterialConstants& GetMatCB() {
			return data->materialConstants;
		}
		int GetCBIndex() {
			return data->matCBIndex;
		}

		void SetMaterialData(std::string name) {
			if (MaterialData::materialDatas.contains(name)) {
				data = MaterialData::materialDatas[name];
			}
		}

	private:
		std::shared_ptr<MaterialData> data;
		std::shared_ptr<Shader> shader;
	};

	Material::Material(Entity* entity) : ComponentBase(entity) {
		
	}

	void Material::Parse(Entity& entity, const rapidjson::Value& parm) {
		std::string shaderName = parm["Shader"].GetString();
		std::string materialName = parm["Name"].GetString();
		entity.AppendComponent<Material>()
			.GetComponent<Material>()
			->Init(materialName, shaderName);
	}

	void Material::Init(std::string name, std::string shader) {
		this->shader = Shader::instances[shader];

		if (MaterialData::materialDatas.contains(name)) {
			this->data = MaterialData::materialDatas[name];
		}
		else {
			Log::Error("MaterialData not found");
			//Fallback to init material;

			return;
		}

		enabled = true;
	}

	void Material::DrawEditorUI() {
		ImGui::Text("Material");
		ImGui::Spacing();
	}

	std::shared_ptr<Shader> Material::getShader() {
		return shader;
	}

	void Material::SetShader(const std::string name) {
		shader = Shader::instances[name];
	}

}