#include <stdafx.h>
#include <Interface/IComponent.h>
#include <Interface/ComponentBase.h>
#include <Resources/MaterialData.h>
#include <Renderer/Shader.h>

export module Material;

namespace Component {
	class Material : public ComponentBase {
	public:

		static std::string componentName;
		bool enabled{ false };

		Material(Entity* entity);
		void Init(std::string name, std::string shader);

		//Regist to reflect
		static void Bind();
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

	ReflectBindComponent(Material);

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
		ImGui::Text(componentName.c_str());
		ImGui::Spacing();
	}

	std::shared_ptr<Shader> Material::getShader() {
		return shader;
	}

	void Material::SetShader(const std::string name) {
		shader = Shader::instances[name];
	}
}