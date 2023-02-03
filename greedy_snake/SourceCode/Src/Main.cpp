//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"
#include<math.h>

//函数
void Creat_body(int n);//产生身体
void Creat_food(int food_num);//产生食物
void init(void);//初始化处理
void gameover(void);//游戏结束
void readhead(void);//读取变相时的数据
void readbody(void);//不断读取保存蛇身的信息


//定义身体结构体：
typedef struct body{
char *name;//精灵名字
float VelocityX;
float VelocityY;//精灵的速度
float PosX;//精灵的坐标
float PosY;
}BODY;

//定义常量
const float velociyt = 12.f;//移动速度
const float TurnPeriod = 0.3;//限制转向的频率，其值与速度大小有关


//定义变量
int Start = 0;//控制游戏的状态，0为未开始，1为游戏开始，2为游戏结束
int body_num=1;//身体长度编号
int food_num=1;//食物数量
char *foodname[100];//记录食物名字
BODY body[100];//定义身体结构体变量
BODY head;//定义蛇头结构体变量
float TurnLeftTime = 0.f;//蛇头离下次转向剩余时间并初始化
int counter=1;//测试计数



///////////////////////////////////////////////////////////////////////////////////////////
//
// 主函数入口
//
//////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// 初始化游戏引擎
	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : 在此使用API更改窗口标题
	dSetWindowTitle("Greedy Snake");

	// 引擎主循环，处理屏幕图像刷新等工作
	while( dEngineMainLoop() )
	{
		// 获取两次调用之间的时间差，传递给游戏逻辑处理
		float	fTimeDelta	=	dGetTimeDelta();
		TurnLeftTime -= fTimeDelta;//倒计时时间

		if(Start==0)//游戏等待开始
        {
            dSetSpriteVisible("start",true);
            dSetSpriteVisible("text",true);
        }

        if(Start==1)//游戏开始
        {
            //显示文本内容
            dSetTextValue("length",body_num);

            readbody();//不断读取每节蛇身信息

            //第一节身体跟随蛇头,误差不能太大，会导致重叠，也不能太小，会导致身体分离
                if(fabs(body[1].PosX-head.PosX)<=0.2&&fabs(body[1].PosY-head.PosY)<=0.2)//浮点数相等的表示方式
                {
                    body[1].VelocityX=head.VelocityX;
                    body[1].VelocityY=head.VelocityY;
                    dSetSpriteLinearVelocity(body[1].name,body[1].VelocityX,body[1].VelocityY);
                }

                //后面身体的跟随运动，要分情况讨论
                for(int i=2;i<=body_num;i++)//到达蛇头改变方向的地点
                {
                    if(body[i-1].VelocityX!=0&&body[i-1].VelocityY==0)//在x方向上运动
                    {
                        if(fabs(body[i].PosY-body[i-1].PosY)<=0.2)
                        {
                            body[i].VelocityX=body[i-1].VelocityX;
                            body[i].VelocityY=body[i-1].VelocityY;
                            dSetSpriteLinearVelocity(body[i].name,body[i].VelocityX,body[i].VelocityY);
                        }
                    }
                    if(body[i-1].VelocityY!=0&&body[i-1].VelocityX==0)//在x方向上运动
                    {
                        if(fabs(body[i].PosX-body[i-1].PosX)<=0.2)
                        {
                            body[i].VelocityX=body[i-1].VelocityX;
                            body[i].VelocityY=body[i-1].VelocityY;
                            dSetSpriteLinearVelocity(body[i].name,body[i].VelocityX,body[i].VelocityY);
                        }
                    }
                }

        }


		if(Start==2)
        {
            gameover();
        }
        // 执行游戏主循环
		GameMainLoop( fTimeDelta );
	};

	// 关闭游戏引擎
	dShutdownGameEngine();
	return 0;
}

//==========================================================================
//
// 引擎捕捉鼠标移动消息后，将调用到本函数
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// 引擎捕捉鼠标点击消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseClick(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// 引擎捕捉鼠标弹起消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// 引擎捕捉键盘按下消息后，将调用到本函数
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{
    if(iKey==KEY_SPACE&&Start==0)//仅当游戏未开始且按下空格执行
    {
        Start = 1;//记录游戏状态
        init();//初始化
    }

	// 可以在此添加游戏需要的响应函数
	//键盘控制蛇头运动，同时要保证蛇头不能直接掉头
	if(TurnLeftTime<=0.f)
	{
	    switch(iKey)
        {
        case KEY_W:
            if(dGetSpriteLinearVelocityY("head")>0)
            {
                dSetSpriteLinearVelocity("head",0,velociyt);
            }
            else
            {
                dSetSpriteRotation("head",270);
                dSetSpriteLinearVelocity("head",0,-velociyt);
            }
            break;
        case KEY_S:
            if(dGetSpriteLinearVelocityY("head")<0)
            {
                dSetSpriteLinearVelocity("head",0,-velociyt);
            }
            else
            {
                dSetSpriteLinearVelocity("head",0,velociyt);
                dSetSpriteRotation("head",90);
            }
            break;
        case KEY_A:
            if(dGetSpriteLinearVelocityX("head")>0)
            {
                dSetSpriteLinearVelocity("head",velociyt,0);
            }
            else
            {
                dSetSpriteLinearVelocity("head",-velociyt,0);
                dSetSpriteRotation("head",180);
            }
            break;
        case KEY_D:
            if(dGetSpriteLinearVelocityX("head")<0)
            {
                dSetSpriteLinearVelocity("head",-velociyt,0);
            }
            else
            {
                dSetSpriteLinearVelocity("head",velociyt,0);
                dSetSpriteRotation("head",0);
            }
            break;
        }
        TurnLeftTime=TurnPeriod;//重置时间

        readhead();//更新并保存蛇头的信息
    }

	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}
//==========================================================================
//
// 引擎捕捉键盘弹起消息后，将调用到本函数
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
//
void dOnKeyUp( const int iKey )
{
	// 可以在此添加游戏需要的响应函数
	OnKeyUp(iKey);
}

//===========================================================================
//
// 引擎捕捉到精灵与精灵碰撞之后，调用此函数
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵发送及接受碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// 可以在此添加游戏需要的响应函数
	if(strcmp("head",szSrcName)==0)//用于确定处理对象
	{

	    if(strcmp("food",szTarName)==0);
            {
                dDeleteSprite(foodname[food_num]);//删除被吃掉的食物精灵
                Creat_body(body_num);
                dPlaySound("eat",0,1);

                //跟随
                float x,y;//增加身体时的坐标
                if(body_num==1)//第一节身体
                {
                    x=dGetSpritePositionX("head");
                    y=dGetSpritePositionY("head");

                    if(dGetSpriteLinearVelocityX("head")>0&&dGetSpriteLinearVelocityY("head")==0)
                    {
                        dSetSpritePosition(body[1].name,x-4,y);
                        dSetSpriteLinearVelocityX(body[1].name,velociyt);
                    }

                    if(dGetSpriteLinearVelocityX("head")<0&&dGetSpriteLinearVelocityY("head")==0)
                    {
                        dSetSpritePosition(body[1].name,x+4,y);
                        dSetSpriteLinearVelocityX(body[1].name,-velociyt);
                    }
                    if(dGetSpriteLinearVelocityX("head")==0&&dGetSpriteLinearVelocityY("head")>0)
                    {
                        dSetSpritePosition(body[1].name,x,y-4);
                        dSetSpriteLinearVelocityY(body[1].name,velociyt);
                    }
                    if(dGetSpriteLinearVelocityX("head")==0&&dGetSpriteLinearVelocityY("head")<0)
                    {
                        dSetSpritePosition(body[1].name,x,y+4);
                        dSetSpriteLinearVelocityY(body[1].name,-velociyt);
                    }
                }
                else//第一节以后的身体
                {
                    x=dGetSpritePositionX(body[body_num-1].name);
                    y=dGetSpritePositionY(body[body_num-1].name);

                    if(dGetSpriteLinearVelocityX(body[body_num-1].name)>0&&dGetSpriteLinearVelocityY(body[body_num-1].name)==0)
                    {
                        dSetSpritePosition(body[body_num].name,x-4,y);
                        dSetSpriteLinearVelocityX(body[body_num].name,velociyt);
                    }

                    if(dGetSpriteLinearVelocityX(body[body_num-1].name)<0&&dGetSpriteLinearVelocityY(body[body_num-1].name)==0)
                    {
                        dSetSpritePosition(body[body_num].name,x+4,y);
                        dSetSpriteLinearVelocityX(body[body_num].name,-velociyt);
                    }
                    if(dGetSpriteLinearVelocityX(body[body_num-1].name)==0&&dGetSpriteLinearVelocityY(body[body_num-1].name)>0)
                    {
                        dSetSpritePosition(body[body_num].name,x,y-4);
                        dSetSpriteLinearVelocityY(body[body_num].name,velociyt);
                    }
                    if(dGetSpriteLinearVelocityX(body[body_num-1].name)==0&&dGetSpriteLinearVelocityY(body[body_num-1].name)<0)
                    {
                        dSetSpritePosition(body[body_num].name,x,y+4);
                        dSetSpriteLinearVelocityY(body[body_num].name,-velociyt);
                    }
                }
                body_num++;//身体长度产生并加一
                food_num++;//食物下标加一
                Creat_food(food_num);//产生食物
            }

                //蛇头与身体碰撞
                if(body_num > 3)//保证有足够的长度
                {
                    for(int i=3;i<body_num;i++)
                    {
                        if(stricmp(body[i].name,szTarName)==0)
                        {
                            Start=2;//游戏结束
                        }
                    }
                }
	}

	OnSpriteColSprite(szSrcName, szTarName);
}

//===========================================================================
//
// 引擎捕捉到精灵与世界边界碰撞之后，调用此函数.
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵的世界边界限制
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
     if(stricmp(szName,"head")==0&&iColSide==0)             //设置当蛇头碰到边界时结束游戏
    {
        Start=2;
    }
    if(stricmp(szName,"head")==0&&iColSide==1)
    {
        Start=2;
    }
    if(stricmp(szName,"head")==0&&iColSide==2)
    {
        Start=2;
    }
    if(stricmp(szName,"head")==0&&iColSide==3)
    {
        Start=2;
    }
	// 可以在此添加游戏需要的响应函数
	OnSpriteColWorldLimit(szName, iColSide);
}


void Creat_body(int n)//记录第几节身体
{
        body[n].name=dMakeSpriteName("body",n);//记录下标和名字
        dCloneSprite("body",body[n].name);

        if(n>2)//从第三节身体开始接受碰撞
        {
            dSetSpriteCollisionReceive(body[n].name,true);//设置可以接受物理碰撞
        }
}

void Creat_food(int food_num)//产生食物
{
    foodname[food_num]=dMakeSpriteName("food",food_num);
    dCloneSprite("food",foodname[food_num]);//复制食物并记录下标
    int x,y;
    x=dRandomRange(-45,45);
    y=dRandomRange(-32,32);
    dSetSpritePosition(foodname[food_num],x,y);
}

void init(void)//初始化处理
{
    Creat_food(food_num);//产生初始食物
    dSetSpriteVisible("start",false);
    dSetSpriteVisible("text",false);
    dPlaySound("music",1,1);//播放背景音乐
}

void gameover(void)//游戏结束
{
    dSetSpriteVisible("gameover",true);
    dPlaySound("fail",1,1);
}

void readhead(void)//读取蛇头变相时的数据
{
    head.PosX=dGetSpritePositionX("head");
    head.PosY=dGetSpritePositionY("head");
    head.VelocityX=dGetSpriteLinearVelocityX("head");
    head.VelocityY=dGetSpriteLinearVelocityY("head");
}

void readbody(void)//不断读取保存蛇身的信息
{
    for(int i=1;i<=body_num;i++)
    {
        body[i].PosX=dGetSpritePositionX(body[i].name);
        body[i].PosY=dGetSpritePositionY(body[i].name);
        body[i].VelocityX=dGetSpriteLinearVelocityX(body[i].name);
        body[i].VelocityY=dGetSpriteLinearVelocityY(body[i].name);
    }
}


