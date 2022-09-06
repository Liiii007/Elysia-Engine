<div align=center><img src="https://cdn.jsdelivr.net/gh/Liiii007/LiquidBlog@main/img/202208212313351.png#pic_center)"></div>

<br/>

<div align=center>
    <div>
        <font size=70>Elysia Engine</font>
    </div>
</div>
<br/>

这是一个手搓的游戏引擎，目前正在朝着Windows平台的DX12渲染器迈进中。俗话说，最好的学习方法就是自己造一个轮子。


## 特点
- 架构的扩展性经过持续重构稳步提升中，争取做到具有良好架构的引擎（渲染器）。
- 积极使用C++新特性来构建组件（目前已将大部分组件用module重构，编译速度++）
- 使用了ECS-like的接口，便于使用（尚未实现真正的缓存友好的ECS架构）


## 当前目标
- [x] 抽象出完善的光照系统与材质
- [ ] 抽象出贴图，与材质系统联动
- [ ] 实现ShadowMap&&PCCS
- [ ] 完善关卡的编辑系统
- [ ] 接入FMOD作为音频组件
- [ ] 接入lua脚本引擎
- [ ] PBR！
- [ ] 接入物理引擎


## 参考
- DX12的渲染器使用了很多《DirectX 12 3D游戏开发实践》一书中的样例代码，特别感谢Luna大叔
- JSON的操作使用了Rapidjson库
- 模型的导入使用了Assimp库
- 反射系统手搓，参考了太极图形的视频
- 图形API为微软的DirectX 12
- UI库使用了ImGui库
- 内置了个大伟丘的模型用来测试Mesh组件，感谢原神的模型
- 首页图来自米游社，暂时还找不到出处，如果侵犯了您的权益请务必联系我修改或者删除
