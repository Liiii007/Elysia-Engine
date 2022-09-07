# Component

## 组件列表
- Translation->位置信息 朝向
- Mesh->用以处理与保存模型数据，同时存有顶点/索引缓冲区等信息供渲染系统调用
- Material->与Shader联动，存有物体的材质信息（待扩展）
- Light->场景中的灯光定义，目前为平行光



## 使用 `AutoComponentGenerater.py` 脚本自动创建组件

运行后输入组件名字即可(暂时还没写)

## 手动创建组件

假设组件名为T

对于 `T.ixx` 模块文件
```c++
#include <stdafx.h>
import ECS;

//import depenency

export module T;
namespace Component {

	export class T final : public ComponentBase {
	public:
		T(Entity* entity) : ComponentBase(entity) {};
		static void Parse(Entity& entity, const rapidjson::Value& parm);
		virtual void DrawEditorUI() override;

		T* someFunction(){
			//code
			return this;//for continue call
		}

		Entity& ReturnParentEntity() {
			return *parentEntity;
		}

		//some data member
	};

	void T::Parse(Entity& entity, const rapidjson::Value& parm) {
		//Prepare data for component init

		//AppendComponent to entity
		entity.AppendComponent<T>()
			.GetComponent<T>();
	}

	//ImGui Relate
	void T::DrawEditorUI(){}
}
```
