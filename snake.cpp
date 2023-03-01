#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <windows.h>



class SNAKE
{

private:

/*
200ms
400ms
800ms
1600ms
3200ms
*/
//难度,移动速度
    int m_difficulty;
    int moveSpeed[6]={1,25,20,16,10,5};
    int foodTime[6]={1,3000,2000,1500,100,500};


//移动的场地,判断移动是否非法,是否有食物
    int moveSpace[50][100];

//蛇
    struct BODY
    {
        int x;
        int y;
    };
    BODY body[3000];
    const int maxIndex = 2500;
    int m_head;
    int m_tail;

    int m_dir;//移动方向
    //0 1   2   3   
    //上下左右
    int m_dirx[4]={-1,1,0,0};
    int m_diry[4]={0,0,-1,1};

    int m_windowX;
    int m_windowY;

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

        foodX=1;
        foodY=1;


//初始化蛇
        m_dir=3;
        m_head=1;
        m_tail=0;
        body[m_head]={windowx/2,windowy/2};
        body[m_tail]={windowx/2,windowy/2-1};
        drawSpace(windowx,windowy);
        drawSnake(body[0]);
        drawSnake(body[1]);
        
    }

    int gameStart()
    {
        int changeDir=m_dir;
        int flag=0;
        int remainingTime=0;

        int score=0;

        setFood();

        while(1)
        {
// printf("flag=%d\n",flag);
            flag++;
            flag%=100000;
            remainingTime++;
            if(remainingTime==foodTime[m_difficulty])
            {
                remainingTime=0;
                setFood();
            }


            if(_kbhit())
            {
                char midInput;
                while(_kbhit())
                {
                    midInput=_getch();
                }
                int input;

// printf("%c\n",midInput);



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

                int unableDir=(m_dir/2)*2;
                unableDir+=1-(unableDir^m_dir);
                if(input!=unableDir)
                {
                    changeDir=input;
                }

            }

            Sleep(10);
// Sleep(100);

            if(flag%moveSpeed[m_difficulty]==0)
            {


// puts("hello world!");
                m_dir=changeDir;

                // 0 -> normal
                // 1 -> fail
                // 2 -> grow

// puts("hello");
                int judge=moveSnakeHead();
// puts("world");

// printf("judge=%d\n",flag);
// system("pause");

                switch (judge)
                {
                    case 0:
                        moveSnakeTail();
                        break;
                    case 1:
                        return score;
                        break;
                    case 2:
                        setFood();
                        remainingTime=0;
                        score+=100;
                        drawScore(score);
                        break;
                }

            }

        }
    }


private:

    void drawScore(int score)
    {
        printf("\033[%d;%dH",1,1);
        printf("\033[%d;%dm%d\033[0m",91,40,score);

    }

    void setFood()
    {
        if(moveSpace[foodX][foodY]==2)
        {
            cleanFood();
        }
        int midX;
        int midY;
        do
        {
            midX=(rand()%(m_windowX-1))+1;
            midY=(rand()%(m_windowY-1))+1;
        }
        while(moveSpace[midX][midY]);
        foodX=midX;
        foodY=midY;
        drawFood();
    }

    void drawSpace(int windowx, int windowy)
    {

        
        printf("\033[%d;%dH",windowx+2,3);
        puts("\033[97m coded by abirdlikefish \033[0m");

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
/*
0 -> 正常
1 -> 非法
2 -> 食物
*/
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



    void draw(int windowx, int windowy,char penL,char penR,int colorF,int colorB)
    {
        printf("\033[%d;%dH",windowx,windowy*2);
        printf("\033[%d;%dm%c",colorF,colorB,penL);
        printf("\033[%d;%dm%c",colorF,colorB,penR);

        fflush(stdout);

    }
    void drawFood()
    {
        moveSpace[foodX][foodY]=2;
        draw(foodX,foodY,'(',')',31,101);
    }
    void cleanFood()
    {
        moveSpace[foodX][foodY]=0;
        draw(foodX,foodY,' ',' ',37,47);
    }


    void drawSnake(BODY body)
    {
        moveSpace[body.x][body.y]=1;
        draw(body.x, body.y, '[' ,']',32,102);
    }
    void cleanSnake(BODY body)
    {
        moveSpace[body.x][body.y]=0;
        draw(body.x, body.y,' ',' ',37,47);
    }
    int moveSnakeHead()
    {
        BODY midbody={body[m_head].x+m_dirx[m_dir],body[m_head].y+m_diry[m_dir]};
        m_head++;
        m_head%=maxIndex;
        body[m_head]=midbody;

        int returnData=moveSpace[midbody.x][midbody.y];

        drawSnake(body[m_head]);
        return returnData;
    }
    void moveSnakeTail()
    {
        cleanSnake(body[m_tail]);
        m_tail++;
        m_tail%=maxIndex;
    }
    
};


int main()
{
    system("mode con cols=200 lines=50");

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

        int difficulty;
        do
        {
            puts("please check the degree of difficulty");
            puts("1 is the easiest,5 is the most difficult");
            puts("input a number between 1 and 5");
            scanf("%d",&difficulty);
        }
        while(difficulty <1|| difficulty > 5);

        SNAKE snake(windowy+2,windowx+2,difficulty);
        int score=snake.gameStart();
        // gameStart();
        fputs("\033[0m",stdout);
        system("cls");
        printf("your score is \033[91m%d\033[0m !\n",score);

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