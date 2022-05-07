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

    // �����PVE
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

    // �ֵ�������Ϊtrue ��Ϊfalse
    playerFlag = true;
}

void GameModel::UpdateGameMap(int row,int col){
    if(playerFlag){
        gameMapVec[row][col] = 1;
    }else{
        gameMapVec[row][col] = -1;
    }
    //����
    playerFlag = !playerFlag;
}

void GameModel::actionByPerson(int row, int col){
    UpdateGameMap(row,col);
}

bool GameModel::isWin(int row,int col){
    // ˮƽ����ֱ�����ҶԽ��߷����Ƿ��������
    for(int i=0;i<5;i++){
        // ˮƽ
        if(row>0 && row<BOARD_GRAD_SIZE &&
           col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+1] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+2] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+3] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+4]){
            return true;
        }
        //���жϴ�ֱ�����Ƿ���5��
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col>0 && col<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col] == gameMapVec[row-i+1][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+2][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+3][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+4][col]){
            return true;
        }
        //�ж�"/"�����Ƿ���5�ӣ�����->����
        if(
           row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col+i-4>0 && col+i<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+1][col+i-1] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+2][col+i-2] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+3][col+i-3] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+4][col+i-4]){
            return true;
        }
        //�ж�"\"�����Ƿ���5�ӣ�����->����
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
    // ��������
    for(int i=1;i<gameMapVec.size()-1;i++){
        for(int j=1;j<gameMapVec[i].size()-1;j++){
            if(gameMapVec[i][j]==0){
                return false;
            }
        }
    }
    return true;
}

//�������ֵķ���
void GameModel::calculateScore(){
    //ͳ����һ��ߵ��Ե�����
    int personNum = 0; //������Ӹ���
    int botNum = 0;   //�������Ӹ���
    int emptyNum = 0;   //������հ�λ�ĸ���

    scoreMapVec.clear();
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
        std::vector<int> lineScores;
        for(int j=0;j<BOARD_GRAD_SIZE;j++){
            lineScores.push_back(0);
        }
        scoreMapVec.push_back(lineScores);
    }
    //�Ʒ�
    /*�Ʒ��㷨��
     * ����ÿһ�����ӣ��ж���Щ��û�¹��ģ��Ըõ�Ϊ���ģ��ж���Χ�İ˸�������������ĸ����棬
     * �ж��ٸ��Ǻ��ӡ����ӡ��հף��Դ���Ϊ���������֡��㷨������Ϊ�����������صļӷ�>���ķ���
     */
    for(int row=0;row<BOARD_GRAD_SIZE;row++){
        for(int col=0;col<BOARD_GRAD_SIZE;col++){
            //�հ��c��
            if(row>0 && col>0 && gameMapVec[row][col]==0){
                //������Χ�˸�����
                for(int y=-1;y<=1;y++){
                    for(int x=-1;x<=1;x++){
                        //����
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        //ԭ���겻��
                        if(!(y==0 && x==0)){
                            //ÿ�����������ĸ�

                            //����Һ�������(������������)
                            for(int i=1;i<=4;i++){
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                   col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row+i*y][col+i*x]==1){ //������ҵ���
                                    personNum++;
                                }else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                         col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row+i*y][col+i*x]==0){ //�հ�λ
                                    emptyNum++;
                                    break;
                                }else{ //���磬���а���
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++){
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                   col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row-i*y][col-i*x]==1){ //������ҵ���
                                    personNum++;
                                }else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                         col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row-i*y][col-i*x]==0){ //�հ�λ
                                    emptyNum++;
                                    break;
                                }else{ //���磬���а���
                                    break;
                                }
                            }
                            if(personNum == 1){                 //��ֹ2��
                                scoreMapVec[row][col]+=10;
                            }else if(personNum == 2){           //��ֹ3��
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=30;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=50;
                                if(this->gameType == EVE){
                                     scoreMapVec[row][col]-=20;
                                 }
                            }else if(personNum == 3){           //��ֹ4��
                                //�����λ��һ�������ȼ���һ��
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=80;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=200;
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]-=60;
                                }
                            }else if(personNum == 4){           //��ֹ5��
                                scoreMapVec[row][col]+=10000;
                            }

                            emptyNum = 0;

                            //�Ի����Լ�����
                            for(int i=1;i<=4;i++){
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                   col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row+i*y][col+i*x]==-1){ //AI����
                                    botNum++;
                                }else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                         col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row+i*y][col+i*x]==0){ //�հ�λ
                                    emptyNum++;
                                    break;
                                }else{ //����
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++){
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                   col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row-i*y][col-i*x]==-1){ //AI����
                                    botNum++;
                                }else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                         col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row-i*y][col-i*x]==0){ //�հ�λ
                                    emptyNum++;
                                    break;
                                }else{ //����
                                    break;
                                }
                            }
                            if(botNum == 0){
                                scoreMapVec[row][col]+=5;  //����1��
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=20;
                                }
                            }else if(botNum == 1){
                                scoreMapVec[row][col]+=20; //����2��
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=40;
                                }

                            }else if(botNum == 2){         //����3��
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=25;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=50;
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=60;
                                }

                            }else if(botNum == 3){         //����4��
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=55;
                                if(emptyNum == 2)
                                    scoreMapVec[row][col]+=150;
                                if(this->gameType == EVE){
                                    scoreMapVec[row][col]+=400;
                                }
                            }else if(botNum >= 4){         //����5��
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
    //���ݵ�ǰ���̵�״̬���мƷ�
    calculateScore();

    //�ҳ�������λ�ý�������
    int maxScore = 0;
    std::vector<std::pair<int,int>> maxPoints;
    for(int row = 1;row<BOARD_GRAD_SIZE;row++){
        for(int col = 1;col<BOARD_GRAD_SIZE;col++){
            //������û�¹��ĵط�
            if(gameMapVec[row][col] == 0){
                if(scoreMapVec[row][col]>maxScore){     //��������������
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row,col));
                }else if(scoreMapVec[row][col] == maxScore){   //�����ֵ�����������
                    maxPoints.push_back(std::make_pair(row,col));
                }
            }
        }
    }
    //������������ͬ�����������
    srand((unsigned)time(0));
    int index = rand()%maxPoints.size();
    std::pair<int,int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first;
    clickCol = pointPair.second;
    UpdateGameMap(clickRow,clickCol);
}
