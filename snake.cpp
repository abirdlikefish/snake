#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

class SNAKE
{

private:

//地图大小,难度,移动速度,食物持续时间
    int m_windowX;
    int m_windowY;
    int m_difficulty;
    int moveSpeed[6]={1,20,16,10,8,5};
    int foodTime[6]={1,3000,2000,1500,1000,500};


//移动的场地,判断移动是否非法,是否有食物
    int moveSpace[50][100];

//蛇
    struct BODY
    {
        int x;
        int y;
    };
    BODY body[3000];
    const int maxIndex = 2500;//循环数组
    int m_head;//头的编号
    int m_tail;//尾的编号


    // 0    1   2   3   
    // 上   下  左  右
    int m_dir;//移动方向
    int m_dirx[4]={-1,1,0,0};
    int m_diry[4]={0,0,-1,1};

    //食物的坐标
    int foodX;
    int foodY;


public:

//初始化
    SNAKE(int windowx, int windowy,int difficulty)
    {
        m_difficulty = difficulty;
        m_windowX = windowx;
        m_windowY = windowy;

        fputs("\033[?25l",stdout);  //隐藏光标
        system("cls");  //清空屏幕

        //初始化食物位置
        foodX=1;
        foodY=1;


        //初始化蛇
        m_dir=3;
        m_head=1;
        m_tail=0;
        body[m_head]={windowx/2,windowy/2};
        body[m_tail]={windowx/2,windowy/2-1};
        drawSnake(body[0]);
        drawSnake(body[1]);

        //绘制场地
        drawSpace(windowx,windowy);
        
    }

    //开始游戏
    int gameStart()
    {
        int changeDir=m_dir;//下次移动的方向,默认为当前方向
        int flag=0;//计时器
        int remainingTime=0;//食物持续时间
        int score=0;//分数

        setFood();//设置食物

        while(1)
        {
            //计时器计时
            flag++;
            flag%=100000;

            remainingTime++;//食物持续时间增加


            //时间到,重新设置食物
            if(remainingTime==foodTime[m_difficulty])
            {
                remainingTime=0;
                setFood();
            }


            //当有键盘输入
            if(_kbhit())
            {

                //读取最后一次输入并清空输入缓存
                char midInput;
                while(_kbhit())
                {
                    midInput=_getch();
                }

                //键盘输入转换为方向数字
                int input;
                switch(midInput)
                {
                    case 'w':
                    case 'W':
                        input=0;
                        break;
                    case 's':
                    case 'S':
                        input=1;
                        break;
                    case 'a':
                    case 'A':
                        input=2;
                        break;
                    case 'd':
                    case 'D':
                        input=3;
                        break;
                    default:
                        input=changeDir;
                        break;
                }

                //求当前方向的反方向
                int unableDir=(m_dir/2)*2;
                unableDir+=1-(unableDir^m_dir);

                //输入非当前反方向则记录这次方向变更
                if(input!=unableDir)
                {
                    changeDir=input;
                }

            }

            Sleep(10);//睡一会


            //蛇开始移动
            if(flag%moveSpeed[m_difficulty]==0)
            {
                m_dir=changeDir;//改变方向


                //判断移动方向有什么
                // 0 -> nothing
                // 1 -> illegal space
                // 2 -> food
                int judge=moveSnakeHead();

                switch (judge)
                {
                    case 0:
                        moveSnakeTail();//尾巴前移
                        break;
                    case 1:
                        return score;//结束,返回分数
                        break;
                    case 2:
                        //重设食物
                        setFood();  
                        remainingTime=0;
                        
                        //加分
                        score+=100;
                        drawScore(score);
                        
                        break;
                }

            }

        }
    }


private:

    //将分数显示在地图上方
    void drawScore(int score)
    {
        printf("\033[%d;%dH",1,1);
        printf("\033[%d;%dm%d\033[0m",91,40,score);
    }

    //设置食物
    void setFood()
    {
        //清空残余食物
        if(moveSpace[foodX][foodY]==2)
        {
            cleanFood();
        }

        //随机生成食物
        int midX;
        int midY;
        do
        {
            midX=(rand()%(m_windowX-1))+1;
            midY=(rand()%(m_windowY-1))+1;
        }
        while(moveSpace[midX][midY]);//当该区域为空时结束循环
        foodX=midX;
        foodY=midY;

        //绘制食物
        drawFood();
    }

    //初始化地图
    void drawSpace(int windowx, int windowy)
    {

        //绘制署名
        printf("\033[%d;%dH",windowx+2,3);
        puts("\033[97m coded by abirdlikefish \033[0m");

        //绘制地图
        for(int i=1;i<=windowx;i++)
        {
            for(int j=1;j<=windowy;j++)
            {
                if(i==1||j==1||i==windowx||j==windowy)
                {
                    draw(i,j,' ',' ',30,40);
                }
                else
                {
                    draw(i,j,' ',' ',37,47);
                }
            }
        }


// 设置地图属性
// 0 -> 正常
// 1 -> 非法
// 2 -> 食物
        memset(moveSpace,0,sizeof(moveSpace));
        for(int i=1;i<=windowx;i++)
        {
            moveSpace[i][1]=1;
            moveSpace[i][windowy]=1;
        }
        for(int i=1;i<=windowy;i++)
        {
            moveSpace[1][i]=1;
            moveSpace[windowx][i]=1;
        }

    }


    //绘制
    void draw(int windowx, int windowy,char penL,char penR,int colorF,int colorB)
    {
        printf("\033[%d;%dH",windowx,windowy*2);//移动光标
        printf("\033[%d;%dm%c",colorF,colorB,penL);
        printf("\033[%d;%dm%c",colorF,colorB,penR);
        fflush(stdout); //刷新输出

    }

    //在foodx,y处设置食物
    void drawFood()
    {
        moveSpace[foodX][foodY]=2;
        draw(foodX,foodY,'(',')',31,101);
    }

    //清除食物
    void cleanFood()
    {
        moveSpace[foodX][foodY]=0;
        draw(foodX,foodY,' ',' ',37,47);
    }

    //绘制蛇
    void drawSnake(BODY body)
    {
        moveSpace[body.x][body.y]=1;
        draw(body.x, body.y, '[' ,']',32,102);
    }

    //清除蛇
    void cleanSnake(BODY body)
    {
        moveSpace[body.x][body.y]=0;
        draw(body.x, body.y,' ',' ',37,47);
    }

    //蛇头移动,返回前面区域属性
    int moveSnakeHead()
    {
        //蛇头移动
        BODY midbody={body[m_head].x+m_dirx[m_dir],body[m_head].y+m_diry[m_dir]};
        m_head++;
        m_head%=maxIndex;
        body[m_head]=midbody;

        //记录移动到的区域的属性
        int returnData=moveSpace[midbody.x][midbody.y];

        //绘制
        drawSnake(body[m_head]);

        return returnData;
    }

    //移动尾巴
    void moveSnakeTail()
    {
        //清除尾巴
        cleanSnake(body[m_tail]);

        //设立新尾巴
        m_tail++;
        m_tail%=maxIndex;
    }
    
};


int main()
{
    //设置命令行大小
    system("mode con cols=200 lines=50");

    //接收地图大小
    int windowx,windowy;
    do
    {
        puts("please input the size of the space X * Y");
        puts("5<=x<=90\t5<=y<=40");
        scanf("%d%d",&windowx,&windowy);
    }
    while( !(5<=windowx&&windowx<=90&&5<=windowy&&windowy<=40) );


    while(1)
    {
        //接受难度选择
        int difficulty;
        do
        {
            puts("please check the degree of difficulty");
            puts("1 is the easiest,5 is the most difficult");
            puts("input a number between 1 and 5");
            scanf("%d",&difficulty);
        }
        while(difficulty <1|| difficulty > 5);

        //建立类,初始化
        SNAKE snake(windowy+2,windowx+2,difficulty);

        //开始游戏,记录分数
        int score=snake.gameStart();

        //游戏结束,输出分数
        fputs("\033[0m",stdout);
        system("cls");
        printf("your score is \033[91m%d\033[0m !\n",score);

        //询问是否重新开始
        char input; 
        do
        {
            puts("pause \' Y \' to restart the game or pause \' N \' to exit");
            input = getchar();
        }
        while(input!='Y'&&input!='N'&&input!='y'&&input!='n');

        if(input=='Y'||input=='y')
        {
            continue;
        }
        else
        {
            exit(0);
        }

    }
    return 0;
}
