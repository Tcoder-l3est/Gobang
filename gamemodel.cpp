#include"gamemodel.h"

void GameModel::startGame(GameType Type)
{
    gameType = Type;
    gameMapVec.clear();
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
        vector<int> lineBoard;
        for(int j=0;j<BOARD_GRAD_SIZE;j++)
               lineBoard.emplace_back(0);
        gameMapVec.emplace_back(lineBoard);
    }

    // 如果是PVE
    if(gameType == PVE){
        scoreMapVec.clear();
        for(int i=0;i<BOARD_GRAD_SIZE;i++){
            vector<int> linescores;
            for(int j=0;j<BOARD_GRAD_SIZE;j++){
                linescores.emplace_back(0);

            }
            scoreMapVec.emplace_back(linescores);
        }
    }
    else if(gameType == PVP){
        scoreMapVec.clear();
        for(int i=0;i<BOARD_GRAD_SIZE;i++){
            vector<int> linescores;
            for(int j=0;j<BOARD_GRAD_SIZE;j++){
                linescores.emplace_back(0);

            }
            scoreMapVec.emplace_back(linescores);
        }
    }
    else if(gameType == EVE){
        scoreMapVec.clear();
        for(int i=0;i<BOARD_GRAD_SIZE;i++){
            vector<int> linescores;
            for(int j=0;j<BOARD_GRAD_SIZE;j++){
                linescores.emplace_back(0);

            }
            scoreMapVec.emplace_back(linescores);
        }
    }

    // 轮到黑棋下为true 白为false
    playerFlag = true;
}

void GameModel::UpdateGameMap(int row,int col){
    if(playerFlag){
        gameMapVec[row][col] = 1;
    }else{
        gameMapVec[row][col] = -1;
    }
    //换手
    playerFlag = !playerFlag;
}

void GameModel::actionByPerson(int row, int col){
    UpdateGameMap(row,col);
}

bool GameModel::isWin(int row,int col){
    // 水平、竖直、左右对角线方向是否有五个子
    for(int i=0;i<5;i++){
        // 水平
        if(row>0 && row<BOARD_GRAD_SIZE &&
           col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+1] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+2] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+3] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+4]){
            return true;
        }
        //先判断垂直方向是否有5子
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col>0 && col<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col] == gameMapVec[row-i+1][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+2][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+3][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+4][col]){
            return true;
        }
        //判断"/"方向是否有5子，左下->右上
        if(
           row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col+i-4>0 && col+i<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+1][col+i-1] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+2][col+i-2] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+3][col+i-3] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+4][col+i-4]){
            return true;
        }
        //判断"\"方向是否有5子，右下->左上
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+1][col-i+1] &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+2][col-i+2] &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+3][col-i+3] &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+4][col-i+4]){
            return true;
        }
    }
    return false;
}

bool GameModel::isDead(){
    // 棋盘满了
    for(int i=1;i<gameMapVec.size()-1;i++){
        for(int j=1;j<gameMapVec[i].size()-1;j++){
            if(gameMapVec[i][j]==0){
                return false;
            }
        }
    }
    return true;
}

//采用评分的方法
void GameModel::calculateScore(){
    //统计玩家或者电脑的落子
    int personNum = 0; //玩家连子个数
    int botNum = 0;   //机器连子个数
    int emptyNum = 0;   //各方向空白位的个数

    scoreMapVec.clear();
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
        std::vector<int> lineScores;
        for(int j=0;j<BOARD_GRAD_SIZE;j++){
            lineScores.push_back(0);
        }
        scoreMapVec.push_back(lineScores);
    }
    //计分
    /*计分算法：
     * 遍历每一个格子，判断哪些是没下过的，以该点为中心，判断周围的八个点向外延伸的四格里面，
     * 有多少个是黑子、白子、空白，以此作为依据来评分。算法是以守为主，体现在守的加分>攻的分数
     */
    for(int row=0;row<BOARD_GRAD_SIZE;row++){
        for(int col=0;col<BOARD_GRAD_SIZE;col++){
            //空白點算
            if(row>0 && col>0 && gameMapVec[row][col]==0){
                //遍历周围八个方向
                for(int y=-1;y<=1;y++){
                    for(int x=-1;x<=1;x++){
                        //重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        //原坐标不算
                        if(!(y==0 && x==0)){
                            //每个方向延申四个

                            //对玩家黑子评分(正反两个方向)
                            for(int i=1;i<=4;i++){
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                   col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row+i*y][col+i*x]==1){ //真人玩家的子
                                    personNum++;
                                }else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                         col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row+i*y][col+i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出界，或有白子
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++){
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                   col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row-i*y][col-i*x]==1){ //真人玩家的子
                                    personNum++;
                                }else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                         col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row-i*y][col-i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出界，或有白子
                                    break;
                                }
                            }
                            if(personNum == 1){                 //防止2个
                                scoreMapVec[row][col]+=10;
                            }else if(personNum == 2){           //防止3个
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=30;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=50;
                                if(this->gameType == EVE){
                                     scoreMapVec[row][col]-=20;
                                 }
                            }else if(personNum == 3){           //防止4个
                                //量变空位不一样，优先级不一样
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=80;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=200;
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]-=60;
                                }
                            }else if(personNum == 4){           //防止5个
                                scoreMapVec[row][col]+=10000;
                            }

                            emptyNum = 0;

                            //对机器自己评分
                            for(int i=1;i<=4;i++){
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                   col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row+i*y][col+i*x]==-1){ //AI的子
                                    botNum++;
                                }else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                         col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row+i*y][col+i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出界
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++){
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                   col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row-i*y][col-i*x]==-1){ //AI的子
                                    botNum++;
                                }else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                         col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row-i*y][col-i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出界
                                    break;
                                }
                            }
                            if(botNum == 0){
                                scoreMapVec[row][col]+=5;  //练成1个
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=20;
                                }
                            }else if(botNum == 1){
                                scoreMapVec[row][col]+=20; //练成2个
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=40;
                                }

                            }else if(botNum == 2){         //练成3个
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=25;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=50;
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=60;
                                }

                            }else if(botNum == 3){         //练成4个
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=55;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=150;
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=400;
                                }
                            }else if(botNum >= 4){         //练成5个
                                scoreMapVec[row][col]+=20000;
                            }


                        }
                    }
                }
            }
        }
    }

}

void GameModel::actionByAI(int &clickRow,int &clickCol){
    //根据当前棋盘的状态进行计分
    calculateScore();

    //找出分最大的位置进行下棋
    int maxScore = 0;
    std::vector<std::pair<int,int>> maxPoints;
    for(int row = 1;row<BOARD_GRAD_SIZE;row++){
        for(int col = 1;col<BOARD_GRAD_SIZE;col++){
            //必须是没下过的地方
            if(gameMapVec[row][col] == 0){
                if(scoreMapVec[row][col]>maxScore){     //找最大的数和坐标
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row,col));
                }else if(scoreMapVec[row][col] == maxScore){   //存最大值，便于随机数
                    maxPoints.push_back(std::make_pair(row,col));
                }
            }
        }
    }
    //如果多个评分相同的则随机落子
    srand((unsigned)time(0));
    int index = rand()%maxPoints.size();
    std::pair<int,int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first;
    clickCol = pointPair.second;
    UpdateGameMap(clickRow,clickCol);
}
