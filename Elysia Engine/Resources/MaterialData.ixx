#include <stdafx.h>
import Definition;
import Log;
import JSONHandler;

export module MaterialData;
namespace Resource {
	export class MaterialData {
	public:

		static void Load(std::string name, std::string path) {
			materialDatas[name] = std::make_shared<MaterialData>(name, path);
		}

		static void Load(const std::filesystem::path& path) {
			rapidjson::Document d = JSONHandler::load(path);
			if (!d.HasMember("METADATA") && !(d["METADATA"]["Type"].GetString() != "Material")) {
				return;
			}

			std::string name = d["METADATA"]["Name"].GetString();

			materialDatas[name] = std::make_shared<MaterialData>(name, path);
		}

		static void Offload(std::string name) {
			if (materialDatas.contains(name)) {
				materialDatas.erase(name);
			}
		}

		MaterialData(std::string name, const std::filesystem::path& path) : name(name) {
			auto d = JSONHandler::load(path);

			const rapidjson::Value& parm = d["Parm"];
			if (parm.HasMember("diffuseAlbedo")) {
				materialConstants.diffuseAlbedo.x = parm["diffuseAlbedo"].GetArray()[0].GetFloat();
				materialConstants.diffuseAlbedo.y = parm["diffuseAlbedo"].GetArray()[1].GetFloat();
				materialConstants.diffuseAlbedo.z = parm["diffuseAlbedo"].GetArray()[2].GetFloat();
				materialConstants.diffuseAlbedo.w = parm["diffuseAlbedo"].GetArray()[3].GetFloat();
			}
			else {
				Log::Warning("Not contain diffuseAlbedo");
			}

			if (parm.HasMember("fresnelR0")) {
				materialConstants.fresnelR0.x = parm["fresnelR0"].GetArray()[0].GetFloat();
				materialConstants.fresnelR0.y = parm["fresnelR0"].GetArray()[1].GetFloat();
				materialConstants.fresnelR0.z = parm["fresnelR0"].GetArray()[2].GetFloat();
			}
			else {
				Log::Warning("Not contain fresnelR0");
			}

			if (parm.HasMember("roughness")) {
				materialConstants.roughness = parm["roughness"].GetFloat();
			}
			else {
				Log::Warning("Not contain roughness");
			}
		}

		MaterialConstants materialConstants{};

		std::string name;
		int matCBIndex;

		static std::unordered_map<std::string, std::shared_ptr<MaterialData>> materialDatas;
	};

	std::unordered_map<std::string, std::shared_ptr<MaterialData>> MaterialData::materialDatas{};
}