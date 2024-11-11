#undef UNICODE
#undef _UNICODE
#include <easyx.h>
#include <iostream>
#define rows 15
#define cols 15

IMAGE bgp;                              //载入图片
int flag;                           //1为黑子，0为白子
int piece[rows][cols];                  //棋盘坐标
bool first;                             //第一手
int mod;                            //mod为1是玩家对战，mod为0是人机对战
bool md;                            //md控制起始页面的输出
ExMessage m;                             //声明一个鼠标的信息m
int row,col;
int over;                            // 判断是否结束
int scores[rows][cols] = {0};             // 分数表
int maxx, maxy;          //获取最大分数的坐标

struct MAXSCORE {
    int maxx, maxy;
};

//初始化棋盘落子情况，1为黑子，0为无子,-1为白子
void init_piece() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            piece[i][j] = 0;
        }
    }
    first = 1;
    flag = 1;
//绘制线条
    setlinecolor(BLACK);
    for (int x = 150; x <= 850; x += 50) line(x, 150, x, 850);
    for (int y = 150; y <= 850; y += 50) line(150, y, 850, y);
//绘制点
    setfillcolor(BLACK);
    fillcircle(6 * 50, 6 * 50, 5);
    fillcircle(6 * 50, 14 * 50, 5);
    fillcircle(10 * 50, 10 * 50, 5);
    fillcircle(14 * 50, 6 * 50, 5);
    fillcircle(14 * 50, 14 * 50, 5);

}

//绘制棋子
bool draw_piece(int flag,int row, int col) {
    if (flag == 1) {
        setfillcolor(BLACK);             //flag = 1时为黑子
    }
    else {                               //flag = 0时为白子
        setfillcolor(WHITE);
    }

    //当棋盘数组在（x, y）位置为空，则落子，并且黑子标记为1，白子标记为-1
    if (piece[row][col] == 0 && row >= 0 && row < rows && col >= 0 && col < cols) {
        fillcircle(row * 50 + 150, col * 50 + 150, 20);
        if(flag == 1) piece[row][col] = 1;
        else piece[row][col] = -1;
        return true;
    }
    else return false;
}

//五子连珠的判断
//判断行是否五子连珠
int check_numhang(int num, int row, int col) {
    int x = row, y = col;
    int count = 0;
    while (piece[row][col] == piece[x][y]) {
        x++;
        count++;
        if (count == num && piece[row][col] == 1) return 1;
        else if (count == num && piece[row][col] == -1) return -1;
        if (piece[row][col] != piece[x][y]) {
            x = x - num;
        }
    }
    return 0;
}
//判断列是否五子连珠
int check_numlie(int num, int row, int col) {
    int count = 0;
    int x = row, y = col;
    while (piece[row][col] == piece[x][y]) {
        y++;
        count++;
        if (count == num && piece[x][y] == 1) return 1;
        else if (count == num && piece[x][y] == -1) return -1;
        if (piece[row][col] != piece[x][y]) {
            y = y - num;
        }
    }
    return 0;
}
//判断左斜线是否五子连珠
int check_numlxie(int num, int row, int col) {
    int count = 0;
    int x = row, y = col;
    while (piece[row][col] == piece[x][y]) {
        x++;
        y++;
        count++;
        if (count == num && piece[x][y] == 1) return 1;
        else if (count == num && piece[x][y] == -1) return -1;
        if (piece[row][col] != piece[x][y]) {
            y = y - num;
            x = x - num;
        }
    }
    return 0;
}
//判断右斜线是否五子连珠
int check_numrxie(int num, int row, int col) {
    int count = 0;
    int x = row, y = col;
    while (piece[row][col] == piece[x][y]) {
        x++;
        y--;
        count++;
        if (count == num && piece[x][y] == 1) return 1;
        else if (count == num && piece[x][y] == -1) return -1;
        if (piece[row][col] != piece[x][y]) {
            y = y + num;
            x = x - num;
        }
    }
    return 0;
}

//判断输赢后进行下一局前清空棋盘
void clean() {
    flag = 1;
    first = 1;

    clearrectangle(0, 0, 1000, 1000);

    initgraph(1000, 1000);                     //图片的大小
    loadimage(&bgp, _T("./images/五子棋棋盘.PNG"));  //加载图片bg.jpg
    putimage(0, 0,1000, 1000, &bgp, 0, 0);  //显示图片的位置及大小
    init_piece();

    //设置结束文字属性
    settextcolor(BLACK);
    settextstyle(20, 20, _T("楷书"));
    setbkmode(TRANSPARENT);
    outtextxy(350, 50, "点击ESC退出游戏");
}
          
//分数表
int score(int self, int ai) {                     //玩家的连子数，ai的连子数
    if (self == 1) return 5;
    if (self == 2 && ai == 2) return 10;
    if (self == 2 && ai == 1) return 20;
    if (ai == 2 && self == 1) return 30;
    if (ai == 2 && self == 0) return 40;
    if (ai == 3 && self == 1) return 50;
    if (ai == 3 && self == 0) return 60;
    if (self == 3 && ai == 2) return 70;
    if (ai == 4 && self == 1) return 150;
    if (self == 3 && ai == 1) return 1000;
    if (self == 4 && ai == 2) return 5000;
    if (ai == 4 && self == 0) return 10000;
    if (self == 4 && ai == 1) return 20000;
    if (ai == 5 && self == 0) return 1000000;
    if (ai == 5 && self == 1) return 1000000;
    if (ai == 5 && self == 2) return 1000000;
    return 0;
}
//行的分数
int getscore_row(int x, int y, int pieces) {
    int ai = 1, self = 0;
    for (int i = x + 1; i <= rows; i++) {
        if (piece[i][y] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[i][y] == 0) break;
        else {
            if (pieces == -1) self++;
            else if (pieces == 1) ai++;
            break;
        }
    }
    for (int i = x - 1; i >= 0; i--) {
        if (piece[i][y] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[i][y] == 0) break;
        else {
            if (pieces == -1) self++;
            else if (pieces == 1) ai++;
            break;
        }
    }
    return score(self, ai);
}
//列的子数
int getscore_col(int x, int y, int pieces) {
    int ai = 1, self = 0;
    for (int i = y + 1; i <= cols; i++) {
        if (piece[x][i] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[x][i] == 0) break;
        else {
            if (pieces == -1) self++;
            else if (pieces == 1) ai++;
            break;
        }
    }
    for (int i = y - 1; i >= 0; i--) {
        if (piece[x][i] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[x][i] == 0) break;
        else {
            if (pieces == -1) self++;
            else if (pieces == 1) ai++;
            break;
        }
    }
    return score(self, ai);
}
//左斜的分数
int getscore_l(int x, int y, int pieces) {
    int ai = 1, self = 0;
    for (int i = x + 1, j = y + 1; i <= rows && j <= cols; i++, j++) {
        if (piece[i][j] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[i][j] == 0) break;
        else {
            if (pieces == 1) ai++;
            else if (pieces == -1) self++;
            break;
        }
    }
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
        if (piece[i][j] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[i][j] == 0) break;
        else {
            if (pieces == 1) ai++;
            else if (pieces == -1) self++;
            break;
        }
    }
    return score(self, ai);
}
//右斜的分数
int getscore_r(int x, int y, int pieces) {
    int ai = 1, self = 0;
    for (int i = x + 1, j = y - 1; i <= rows && j >= 0; i++, j--) {
        if (piece[i][j] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[i][j] == 0) break;
        else {
            if (pieces == 1) ai++;
            else if (pieces == -1) self++;
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j <= cols; i--, j++) {
        if (piece[i][j] == pieces) {
            if (pieces == -1) ai++;
            else if (pieces == 1) self++;
        }
        else if (piece[i][j] == 0) break;
        else {
            if (pieces == 1) ai++;
            else if (pieces == -1) self++;
            break;
        }
    }
    return score(self, ai);
 }
//总分
int getscore_max(int x, int y) {
    int rowb = getscore_row(x, y, 1);
    int roww = getscore_row(x, y, -1);
    int cb = getscore_col(x, y, 1);
    int cw = getscore_col(x, y, -1);
    int lb = getscore_l(x, y, 1);
    int lw = getscore_l (x, y, -1);
    int rb = getscore_r(x, y, 1);
    int rw = getscore_r(x, y, -1);

    if (rowb >= 30000 || roww >= 30000 || cb >= 30000 || cw >= 30000 || lb >= 30000 || lw >= 30000 || rb >= 30000 || rw >= 30000) return 1000000;
    
    return rowb + roww + cb + cw + lb + lw + rb + rw;
}

//遍历全图，寻找最大分数的坐标
MAXSCORE getscore() {
    int max = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int score = getscore_max(i, j);
            if (piece[i][j] == 0){
                if (score > max) {
                    max = score;
                    maxx = i, maxy = j;
                }
            }

            if (maxx == 0 && maxy == 0 && first == 1){
                maxx = rows / 2, maxy = cols / 2;
                first = 0;
                if (piece[maxx][maxy] != 0) maxx++;
            }
        }
    }
    MAXSCORE ms;
    ms.maxx = maxx;
    ms.maxy = maxy;
    return ms;
}

void AI(int col, int row) {
    MAXSCORE ms = getscore();
    if(draw_piece(0, ms.maxx, ms.maxy)) piece[maxx][maxy] = -1;
    if (check_numhang(5, ms.maxx, ms.maxy) == -1 || check_numlie(5, ms.maxx, ms.maxy) == -1 || check_numlxie(5, ms.maxx, ms.maxy) == -1 || check_numrxie(5, ms.maxx, ms.maxy) == -1) {
        settextcolor(BLACK);
        settextstyle(60, 60, _T("隶书"));
        setbkmode(TRANSPARENT);
        outtextxy(250, 500, "白子获胜");

        settextstyle(30, 30, _T("楷书"));

        outtextxy(200, 300, "点击Backspace重新开始游戏");
    }
}


int main() {
    //棋盘背景的设置
    initgraph(1000, 1000);                     //图片的大小
    loadimage(&bgp, _T("./images/五子棋棋盘.PNG"));  //加载图片bg.jpg
    putimage(0, 0, 1000, 1000, &bgp, 0, 0);  //显示图片的位置及大小
    //棋盘的初始化
    init_piece();
    md = 1;
    over = 0;
    //游戏循环
    while (true) {                              //时时获取鼠标的信息
        m = getmessage(EX_MOUSE | EX_KEY);
        row = (m.x - 150) / 50, col = (m.y - 150) / 50;

        //设置结束文字属性
        settextcolor(BLACK);
        settextstyle(30, 30, _T("楷书"));
        setbkmode(TRANSPARENT);
        if (md) {
            outtextxy(200, 300, "点击F1开始玩家对战");
            outtextxy(200, 500, "点击F2开始人机对战");

            if (md && m.vkcode == VK_F1) {
                md = 0;
                clean();
                mod = 1;
            }
            if (md && m.vkcode == VK_F2) {
                
                clean();
                mod = 0;
                md = 0;
            }
        }
        //按下鼠标左键，落子
        if (!md && over == 0 && m.lbutton) {
            if (draw_piece(flag, row, col)) {
                if (mod == 1) flag = (flag + 1) % 2;   //当成功落子后，更换下棋方
                
                if (check_numhang(5, row, col) == 1 || check_numlie(5, row, col) == 1 || check_numlxie(5, row, col) == 1 || check_numrxie(5, row, col) == 1) {

                    //设置结束文字属性
                    settextcolor(BLACK);
                    settextstyle(60, 60, _T("隶书"));

                    outtextxy(250, 500, "黑子获胜");

                    settextstyle(30, 30, _T("楷书"));
                    outtextxy(200, 300, "点击Backspace重新开始游戏");
                    over = 1;
                }
                else if (check_numhang(5, row, col) == -1 || check_numlie(5, row, col) == -1 || check_numlxie(5, row, col) == -1 || check_numrxie(5, row, col) == -1) {

                    //设置结束文字属性
                    settextcolor(BLACK);
                    settextstyle(60, 60, _T("隶书"));

                    outtextxy(250, 500, "白子获胜");

                    settextstyle(30, 30, _T("楷书"));
                    outtextxy(200, 300, "点击Backspace重新开始游戏");
                    over = 1;
                }

                if (mod == 0) {
                    AI(row, col);
                }
            }
        }
        if (m.vkcode == VK_BACK) {
            clean();
            md = 1;
            mod = 1;
            over = 0;
        }
        if (m.vkcode == VK_ESCAPE) {
            exit(0);
        }
    }
    return 0;
}
