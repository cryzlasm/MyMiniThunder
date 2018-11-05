
# 声明
<span data-type="color" style="color:rgb(51, 51, 51)">遵循开源许可证 [ </span><span data-type="color" style="color:rgb(34, 34, 34)"><span data-type="background" style="background-color:rgb(255, 255, 255)"><strong>BSD-3-Clause</strong></span></span><span data-type="color" style="color:rgb(34, 34, 34)"><span data-type="background" style="background-color:rgb(255, 255, 255)"> </span></span><span data-type="color" style="color:rgb(51, 51, 51)">] </span>
<span data-type="color" style="color:rgb(51, 51, 51)">项目基于&quot;迅雷开放下载引擎&quot; , 仅仅是增加一个界面显示工作, 一切解释权归&quot;迅雷公司&quot;所有.</span>


Ref:
[缓存:ThunderOpenSDK](https://github.com/cryzlasm/ThunderOpenSDK) : [https://github.com/cryzlasm/ThunderOpenSDK]

---

# 简介
<span data-type="color" style="color:rgb(51, 51, 51)">某一天, 我更新了我的WIN10系统, 更新之后, 我的 迅雷极速版/mini迅雷/迅雷5 等, 均会莫名的崩溃, 刚好在那个时间节点, 发现360极速浏览器的下载, 使用的事迅雷下载引擎, 激发了了解迅雷开放引擎的兴趣, 所以准备编写自己的下载器, 顺带练手(主要是练手)</span>

当前项目基于  [MiniThunder](https://github.com/intlinfo/MiniThunder) 开源项目的二次开发


![image | left](https://cdn.nlark.com/yuque/0/2018/png/172196/1537420815594-f9ba387a-d91b-4e2d-9fae-45c6c9c438e3.png "")




![image | left](https://cdn.nlark.com/yuque/0/2018/png/172196/1537420858505-f6fec75f-562d-4734-bce7-7e48597ede71.png "")



---

# 详细信息




![image | left](https://cdn.nlark.com/yuque/0/2018/png/172196/1540983972760-718201e9-19ba-4248-88e8-d4c18a4d57fb.png "")




---

# 更新日志

* v0.1
    * 基于__*MiniThunder *__项目, 转换原作者MFC项目到QT项目
    * [x] 完成UI
    * [x] 完成库文件的适配
    * [x] 完成单文件下载功能
    * [x] 完成常规任务链接下载 (http | https | ftp 等)
        * [ ] 暂停下载功能
        * [ ] 停止下载功能
        * [ ] 完成迅雷专有链接下载 (thunder:// 等)
        * [ ] 完成迅雷BT文件下载 (种子文件, ed2k://等)
             
            



---

# 项目遇到的问题

BT相关:
* [ ] 已经使用了BT下载API, 但无法下载BT文件, 暂未知问题所在
* [ ] 如何使用 `ED2K` 下载功能 
    可能以上问题的关键为 `XL_CreateBTTaskByThunder` 这个API的原因:
 此API 好像是拉起迅雷创建BT任务, 并非在开放引擎中创建BT任务, 
 暂未找到好的替代方案 


---

# 界面演示


![image.png | left | 696x318](https://cdn.nlark.com/yuque/0/2018/png/172196/1541409445962-72da9a4d-e36e-4700-b059-1a55e70d6349.png "")



![image.png | left | 696x318](https://cdn.nlark.com/yuque/0/2018/png/172196/1541409336154-bd4beeac-6413-4952-8b94-abe6b99b7bcf.png "")



![image.png | left | 696x318](https://cdn.nlark.com/yuque/0/2018/png/172196/1541409430400-dd3d1cd0-1112-4b35-81b6-6779c49069e6.png "")


---

# __License__
[BSD-3-Clause](https://raw.githubusercontent.com/cryzlasm/LICENSE/master/LICENSE%20-%20BSD%203)
 
