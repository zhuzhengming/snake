//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"
#include<math.h>

//����
void Creat_body(int n);//��������
void Creat_food(int food_num);//����ʳ��
void init(void);//��ʼ������
void gameover(void);//��Ϸ����
void readhead(void);//��ȡ����ʱ������
void readbody(void);//���϶�ȡ�����������Ϣ


//��������ṹ�壺
typedef struct body{
char *name;//��������
float VelocityX;
float VelocityY;//������ٶ�
float PosX;//���������
float PosY;
}BODY;

//���峣��
const float velociyt = 12.f;//�ƶ��ٶ�
const float TurnPeriod = 0.3;//����ת���Ƶ�ʣ���ֵ���ٶȴ�С�й�


//�������
int Start = 0;//������Ϸ��״̬��0Ϊδ��ʼ��1Ϊ��Ϸ��ʼ��2Ϊ��Ϸ����
int body_num=1;//���峤�ȱ��
int food_num=1;//ʳ������
char *foodname[100];//��¼ʳ������
BODY body[100];//��������ṹ�����
BODY head;//������ͷ�ṹ�����
float TurnLeftTime = 0.f;//��ͷ���´�ת��ʣ��ʱ�䲢��ʼ��
int counter=1;//���Լ���



///////////////////////////////////////////////////////////////////////////////////////////
//
// ���������
//
//////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// ��ʼ����Ϸ����
	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : �ڴ�ʹ��API���Ĵ��ڱ���
	dSetWindowTitle("Greedy Snake");

	// ������ѭ����������Ļͼ��ˢ�µȹ���
	while( dEngineMainLoop() )
	{
		// ��ȡ���ε���֮���ʱ�����ݸ���Ϸ�߼�����
		float	fTimeDelta	=	dGetTimeDelta();
		TurnLeftTime -= fTimeDelta;//����ʱʱ��

		if(Start==0)//��Ϸ�ȴ���ʼ
        {
            dSetSpriteVisible("start",true);
            dSetSpriteVisible("text",true);
        }

        if(Start==1)//��Ϸ��ʼ
        {
            //��ʾ�ı�����
            dSetTextValue("length",body_num);

            readbody();//���϶�ȡÿ��������Ϣ

            //��һ�����������ͷ,����̫�󣬻ᵼ���ص���Ҳ����̫С���ᵼ���������
                if(fabs(body[1].PosX-head.PosX)<=0.2&&fabs(body[1].PosY-head.PosY)<=0.2)//��������ȵı�ʾ��ʽ
                {
                    body[1].VelocityX=head.VelocityX;
                    body[1].VelocityY=head.VelocityY;
                    dSetSpriteLinearVelocity(body[1].name,body[1].VelocityX,body[1].VelocityY);
                }

                //��������ĸ����˶���Ҫ���������
                for(int i=2;i<=body_num;i++)//������ͷ�ı䷽��ĵص�
                {
                    if(body[i-1].VelocityX!=0&&body[i-1].VelocityY==0)//��x�������˶�
                    {
                        if(fabs(body[i].PosY-body[i-1].PosY)<=0.2)
                        {
                            body[i].VelocityX=body[i-1].VelocityX;
                            body[i].VelocityY=body[i-1].VelocityY;
                            dSetSpriteLinearVelocity(body[i].name,body[i].VelocityX,body[i].VelocityY);
                        }
                    }
                    if(body[i-1].VelocityY!=0&&body[i-1].VelocityX==0)//��x�������˶�
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
        // ִ����Ϸ��ѭ��
		GameMainLoop( fTimeDelta );
	};

	// �ر���Ϸ����
	dShutdownGameEngine();
	return 0;
}

//==========================================================================
//
// ���沶׽����ƶ���Ϣ�󣬽����õ�������
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// ���沶׽�������Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseClick(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// ���沶׽��굯����Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// ���沶׽���̰�����Ϣ�󣬽����õ�������
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{
    if(iKey==KEY_SPACE&&Start==0)//������Ϸδ��ʼ�Ұ��¿ո�ִ��
    {
        Start = 1;//��¼��Ϸ״̬
        init();//��ʼ��
    }

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	//���̿�����ͷ�˶���ͬʱҪ��֤��ͷ����ֱ�ӵ�ͷ
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
        TurnLeftTime=TurnPeriod;//����ʱ��

        readhead();//���²�������ͷ����Ϣ
    }

	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}
//==========================================================================
//
// ���沶׽���̵�����Ϣ�󣬽����õ�������
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
//
void dOnKeyUp( const int iKey )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyUp(iKey);
}

//===========================================================================
//
// ���沶׽�������뾫����ײ֮�󣬵��ô˺���
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��鷢�ͼ�������ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	if(strcmp("head",szSrcName)==0)//����ȷ���������
	{

	    if(strcmp("food",szTarName)==0);
            {
                dDeleteSprite(foodname[food_num]);//ɾ�����Ե���ʳ�ﾫ��
                Creat_body(body_num);
                dPlaySound("eat",0,1);

                //����
                float x,y;//��������ʱ������
                if(body_num==1)//��һ������
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
                else//��һ���Ժ������
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
                body_num++;//���峤�Ȳ�������һ
                food_num++;//ʳ���±��һ
                Creat_food(food_num);//����ʳ��
            }

                //��ͷ��������ײ
                if(body_num > 3)//��֤���㹻�ĳ���
                {
                    for(int i=3;i<body_num;i++)
                    {
                        if(stricmp(body[i].name,szTarName)==0)
                        {
                            Start=2;//��Ϸ����
                        }
                    }
                }
	}

	OnSpriteColSprite(szSrcName, szTarName);
}

//===========================================================================
//
// ���沶׽������������߽���ײ֮�󣬵��ô˺���.
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��������߽�����
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
     if(stricmp(szName,"head")==0&&iColSide==0)             //���õ���ͷ�����߽�ʱ������Ϸ
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
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColWorldLimit(szName, iColSide);
}


void Creat_body(int n)//��¼�ڼ�������
{
        body[n].name=dMakeSpriteName("body",n);//��¼�±������
        dCloneSprite("body",body[n].name);

        if(n>2)//�ӵ��������忪ʼ������ײ
        {
            dSetSpriteCollisionReceive(body[n].name,true);//���ÿ��Խ���������ײ
        }
}

void Creat_food(int food_num)//����ʳ��
{
    foodname[food_num]=dMakeSpriteName("food",food_num);
    dCloneSprite("food",foodname[food_num]);//����ʳ�ﲢ��¼�±�
    int x,y;
    x=dRandomRange(-45,45);
    y=dRandomRange(-32,32);
    dSetSpritePosition(foodname[food_num],x,y);
}

void init(void)//��ʼ������
{
    Creat_food(food_num);//������ʼʳ��
    dSetSpriteVisible("start",false);
    dSetSpriteVisible("text",false);
    dPlaySound("music",1,1);//���ű�������
}

void gameover(void)//��Ϸ����
{
    dSetSpriteVisible("gameover",true);
    dPlaySound("fail",1,1);
}

void readhead(void)//��ȡ��ͷ����ʱ������
{
    head.PosX=dGetSpritePositionX("head");
    head.PosY=dGetSpritePositionY("head");
    head.VelocityX=dGetSpriteLinearVelocityX("head");
    head.VelocityY=dGetSpriteLinearVelocityY("head");
}

void readbody(void)//���϶�ȡ�����������Ϣ
{
    for(int i=1;i<=body_num;i++)
    {
        body[i].PosX=dGetSpritePositionX(body[i].name);
        body[i].PosY=dGetSpritePositionY(body[i].name);
        body[i].VelocityX=dGetSpriteLinearVelocityX(body[i].name);
        body[i].VelocityY=dGetSpriteLinearVelocityY(body[i].name);
    }
}


