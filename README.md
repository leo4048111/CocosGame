# CocosGame
Built for cocos game project

## （一）游戏启动方法
1. 进入游戏目录
2. 启动CocosGame.exe
3. 成功进入游戏
## （二）游玩模式选择和注意事项
### 单人模式
+ 菜单界面选择**Press To Start**进入设置名字界面
+ 设置名字界面输入玩家名字后选择**Confirm**进入游戏模式选择界面
+ 游戏模式选择界面选择**Single Player**开始游戏
### 多人模式
+ 菜单界面选择**Press To Start**进入设置名字界面
+ 设置名字界面输入玩家名字后选择**Confirm**进入游戏模式选择界面
+ 在游戏模式选择界面可以“**Create Room**”或者“**Join Room**”
+ 选择“**Create Room**”，玩家创建房间作为服务端进行游戏，点击界面“**+**”或“**-**”号可以添加/移除电脑玩家，上方标签显示当前加入的其它玩家姓名，如果无玩家加入则显示“**Wait for Joining**”，服务端随时可以点击“**Let's Roll**"来开始游戏。
+ 选择“**Join Room**”，玩家加入房间作为客户端进行游戏，在界面**IP地址输入框** 输入服务端IP地址，点击**connect**发出连接请求，如果连接成功上方标签1显示**Connected** ，如果加入房间成功上方标签2显示**Joined host+主机地址**, 如果房间已满或服务端游戏已开始则标签2显示Can't Join加入失败。加入房间后请等待服务端开始游戏。
### 郊游模式
+ 在主界面选择开启**Field Trip Mode**后，游戏场景中将不会自动生成敌人，需要手动通过输入/addTarget命令添加敌人。（对客户端该模式无效，服务端开启后对客户端同样生效）
### 游玩须知
+ 游戏中所有场景的切换间为了良好的视觉效果均设置有一定的延迟，因此点击开始游戏后场景没有立即切换是正常现象，如果反复不停多次连续点击开始游戏可能导致游戏卡死乃至崩溃。
+ 游戏如果使用笔记本游玩需要使用外接鼠标进行点击操作，如果使用触摸板点击可能没有任何效果。
## （三）游戏操作
### 基本按键操作
|按键|效果|
|---|---|
|W|向前移动|
|S|向后移动|
|A|向左移动|
|D|向右移动|
|R|更换子弹|
|F|快速切换至近战武器并进行近战攻击|
|鼠标左键|开火|
|鼠标移动|控制准心位置|
|Shift+任意移动键|快速翻滚移动（消耗体力值）|
|数字键1~7|切换武器栏1~7|
|enter回车键|打开文本输入框/发送文本输入框内聊天信息|
|ESC|打开游戏中设置菜单（有返回游戏选项和返回主菜单选项）|
### 作弊指令
1. 按Enter呼出文本输入框
2. 输入有效作弊指令，目前支持:  
 
|指令|效果|
|---|----|
|/invincible|无敌|
|/allWeapons|允许使用所有武器|
|/aimbot|子弹自动瞄准|
|/infiniteAmmo|无限弹药|
**注意:**输入作弊指令效果为切换作弊指令开关状态，如果原来作弊开启输入指令后作弊将会关闭
### 聊天系统
1. 按Enter呼出文本输入框
2. 输入聊天内容并发送，所有玩家的聊天信息将显示在上方聊天栏中，聊天栏自动隐藏。
### 掉落拾取系统
+ 击杀敌人后会随机掉落一种可拾取物品，包括武器和特殊效果道具，将人物移动到物品上可自动拾取，拾取未解锁的武器将解锁该武器对应的武器栏，拾取已解锁的武器将自动转换为相应的弹药。
### 游戏成为吃鸡者条件
+ 单人模式下，击杀敌人取得4000分即可成为吃鸡者
+ 多人模式下，玩家团队协作击杀敌人取得4000分后，所有存活的玩家全部为吃鸡者
+ 玩家间**不能**互相攻击