<div align=center><img src="https://cdn.jsdelivr.net/gh/Liiii007/LiquidBlog@main/img/202208212313351.png#pic_center)"></div>

<div align=center>
    <div>
        <font size=30>Elysia Engine</font>
    </div>
</div>




<br/>
<br/>

这是一个手搓的游戏引擎，目前正在朝着Windows平台的DX12渲染器迈进中。俗话说，最好的学习方法就是自己造一个轮子。

PS:云玩家，但是爱莉厨！有谁会不喜欢爱莉呢！！！

<br/>

## 亮点
架构的扩展性还行，争取做到具有良好架构的引擎（渲染器）。

<br/>

## 开学前目标
- [ ] 抽象出完善的光照系统与材质
- [ ] 抽象出贴图，与材质系统联动
- [ ] 完善关卡的保存
- [ ] 接入FMOD作为音频组件
- [ ] 写出完备的输入系统
- [ ] 接入lua脚本引擎（也许做不完）

<br/>

## 参考
- DX12的渲染器使用了很多《DirectX 12 3D游戏开发实践》一书中的样例代码，特别感谢Luna大叔
- JSON的操作使用了Rapidjson库
- 模型的导入使用了Assimp库
- 反射系统部分手搓，部分使用rttr库
- 图形API为微软的DirectX 12
- 内置了个大伟丘的模型用来测试Mesh组件，感谢原神的模型
- 首页图来自米游社，暂时还找不到出处，如果侵犯了您的权益请务必联系我修改或者删除