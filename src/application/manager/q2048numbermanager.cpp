#include "q2048numbermanager.h"
#include <QDebug>

Q2048NumberManager::Q2048NumberManager(QObject *parent):
    Q2048ManagerBase(parent)
{

}

void Q2048NumberManager::startInit()
{
    qsrand(time(NULL));

    int count1 = qrand() % (m_boxMax * m_boxMax - 1) ;
    int count2 = qrand() % (m_boxMax * m_boxMax -1) ;

    for(int i = 0 ; i < m_boxMax;i++){
        QVector<int> k;
        for(int j = 0 ; j < m_boxMax;j++){

            if( i * m_boxMax + j == count1 ||  i * m_boxMax + j == count2)
            {
                k << (qrand() % 2 == 0 ? 2 : 4);
            }
            else{
                k << -1;
            }
        }
        m_data << k;
    }

    sigSendData(m_data);
}

void Q2048NumberManager::goRight()
{
    QVector<QVector<int>> dataDes;
    for(int i = 0 ; i < m_boxMax;i++){

        QVector<int> k0 =  m_data.at(i);
        QVector<int> k;

        //(1)去掉-1的部分
        for(int i = 0;i<k0.count();i++){
            int num = k0.at(i);

            if(num != -1){
                 k.push_back(num);
            }
        }

        //(2)同向组合
        QVector<int> kDes;
        bool step = false;
        bool firstIs = false;

        for(int j = k.count()-1; j > 0;j--){
            if(step){
                step = false;
                continue;
            }

            int code = k.at(j);

            if(k.at(j - 1) == code){
                step = true;

                if(j == 1){
                    firstIs = true;
                }

                kDes.push_front(code * 2);
            }
            else if(code != -1){
                kDes.push_front(code);
            }
        }

        if(!firstIs && k.count() > 0){
            kDes.push_front(k.at(0));
        }

        //补充-1
        while(kDes.count() < m_boxMax){
            kDes.push_front(-1);
        }

        dataDes << kDes;
     }

    m_data.swap(dataDes);
}

void Q2048NumberManager::goLeft()
{
    QVector<QVector<int>> dataDes;
    for(int i = 0 ; i < m_boxMax;i++){

        QVector<int> k0 =  m_data.at(i);
        QVector<int> k;

        //(1)去掉-1的部分
        for(int i = 0;i<k0.count();i++){
            int num = k0.at(i);

            if(num != -1){
                 k.push_back(num);
            }
        }

        //(2)同向组合
        QVector<int> kDes;
        bool step = false;
        bool firstIs = false;

        for(int j = 0; j < k.count()-1;j++){
            if(step){
                step = false;
                continue;
            }
            int code = k.at(j);

            if(k.at(j + 1) == code){
                step = true;
                if(j == 1){
                    firstIs = true;
                }
                kDes.push_back(code * 2);
            }
            else if(code != -1){
                kDes.push_back(code);
            }
        }

        if(!firstIs && k.count() > 0){
            kDes.push_back(k.at(k.count()- 1));
        }

        while(kDes.count() < m_boxMax){
            kDes.push_back(-1);
        }

        dataDes << kDes;
     }

    m_data.swap(dataDes);
}

void Q2048NumberManager::goUp()
{
    QVector<QVector<int>> data;
    transYToX(m_data,data);

    //left
    QVector<QVector<int>> dataDes;
    for(int i = 0 ; i < m_boxMax;i++){

        QVector<int> k0 =  data.at(i);
        QVector<int> k;

        //(1)去掉-1的部分
        for(int i = 0;i<k0.count();i++){
            int num = k0.at(i);

            if(num != -1){
                 k.push_back(num);
            }
        }

        //(2)同向组合
        QVector<int> kDes;
        bool step = false;
        bool firstIs = false;

        for(int j = 0; j < k.count()-1;j++){
            if(step){
                step = false;
                continue;
            }
            int code = k.at(j);

            if(k.at(j + 1) == code){
                step = true;
                if(j == 1){
                    firstIs = true;
                }
                kDes.push_back(code * 2);
            }
            else if(code != -1){
                kDes.push_back(code);
            }
        }

        if(!firstIs && k.count() > 0){
            kDes.push_back(k.at(k.count()- 1));
        }

        while(kDes.count() < m_boxMax){
            kDes.push_back(-1);
        }

        dataDes << kDes;
     }

    //left_end

    QVector<QVector<int>> dataDes2;
    transXToY(dataDes,dataDes2);

    m_data.swap(dataDes2);
}

void Q2048NumberManager::goDown()
{
    QVector<QVector<int>> data;
    transYToX(m_data,data);

    //right
    QVector<QVector<int>> dataDes;
    for(int i = 0 ; i < m_boxMax;i++){

        QVector<int> k0 =  data.at(i);
        QVector<int> k;

        //(1)去掉-1的部分
        for(int i = 0;i<k0.count();i++){
            int num = k0.at(i);

            if(num != -1){
                 k.push_back(num);
            }
        }

        //(2)同向组合
        QVector<int> kDes;
        bool step = false;
        bool firstIs = false;

        for(int j = k.count()-1; j > 0;j--){
            if(step){
                step = false;
                continue;
            }

            int code = k.at(j);

            if(k.at(j - 1) == code){
                step = true;

                if(j == 1){
                    firstIs = true;
                }

                kDes.push_front(code * 2);
            }
            else if(code != -1){
                kDes.push_front(code);
            }
        }

        if(!firstIs && k.count() > 0){
            kDes.push_front(k.at(0));
        }

        //补充-1
        while(kDes.count() < m_boxMax){
            kDes.push_front(-1);
        }

        dataDes << kDes;
     }


    //right_end

    QVector<QVector<int>> dataDes2;
    transXToY(dataDes,dataDes2);

    m_data.swap(dataDes2);
}

bool Q2048NumberManager::isOver()
{
    bool res = true;

    //检测是否存在 -1
    for(int i = 0 ; i< m_boxMax;i++){
        for(int  j= 0 ; j< m_boxMax;j++){
            if(m_data.at(i).at(j) == -1){
               return false;
            }
        }
    }

    //检测周围是否存在相同
    for(int i = 0 ; i< 4;i++){
        for(int  j= 0 ; j< m_boxMax - 1;j++){
            if(i == 0 || i ==4){
                int y = i==0?0:m_boxMax-1;
                if(m_data.at(y).at(j) == m_data.at(y).at(j + 1)){
                   return false;
                }
            }
            else{
                int x = i==1?0:m_boxMax-1;
                if(m_data.at(j).at(x) == m_data.at(j + 1).at(x)){
                   return false;
                }
            }
        }
    }

    //检测周围里面是否存在相同
    for(int i = 1 ; i< m_boxMax-1;i++){
        for(int  j= 1 ; j< m_boxMax-1;j++){
            if(m_data.at(i).at(j) == m_data.at(i+1).at(j)||
               m_data.at(i).at(j) == m_data.at(i-1).at(j)||
               m_data.at(i).at(j) == m_data.at(i).at(j+1)||
               m_data.at(i).at(j) == m_data.at(i).at(j-1)){
               return false;
            }
        }
    }

    return res;
}

void Q2048NumberManager::insertRand()
{
    QVector<QVector<int>> dataDes;
    QVector<int> list;
    QVector<int> listIndex;

    //找到为-1的值
    for(int i = 0 ; i< m_boxMax;i++){
        for(int  j= 0 ; j< m_boxMax;j++){
            if(m_data.at(i).at(j) == -1){
                list.push_back(i * m_boxMax + j);
            }
        }
    }

    //改变1-2个值
    int num = qrand() %2 == 0?1:2;;
    for(int i = 0;i<num;i++){
        listIndex << list.at(qrand() % (list.count()));
    }

    //新data
    for(int i = 0 ; i < m_boxMax;i++){
        QVector<int> L;
        for(int j = 0 ; j < m_boxMax;j++){
            for(int k = 0 ; k < listIndex.count();k++){
                if(listIndex.at(k) == i * m_boxMax + j){
                    L.push_back(qrand() % 2 == 0 ? 2 : 4);
                    break;
                }
                else{
                    L.push_back(m_data.at(i).at(j));
                    break;
                }

            }


        }
        dataDes << L;
    }

    m_data.swap(dataDes);
}

void Q2048NumberManager::doAction(Q2048ManagerBase::GameDir dir)
{
   if(m_isOver) return;

    switch (dir) {
        case g_Right:
        goRight();
    break;
        case g_Left:
        goLeft();
    break;
        case g_Up:
        goUp();
    break;
        case g_Down:
        goDown();
    break;
    }

    if(hasNull()){
        insertRand();  //随机插入1-2个
    }

    m_isOver = isOver();

    sigSendData(m_data,m_isOver);
}

bool Q2048NumberManager::hasNull()
{
    bool res = false;

    for(int i = 0 ; i< m_boxMax;i++){
        for(int  j= 0 ; j< m_boxMax;j++){
            if(m_data.at(i).at(j) == -1){
               res = true;
               break;
            }
        }
    }

    return res;
}

void Q2048NumberManager::transXToY(QVector<QVector<int> > &list1, QVector<QVector<int> > &list2)
{
    for(int i = 0 ; i < m_boxMax;i++){
        QVector<int> L;
        for(int  j= m_boxMax - 1 ; j>=0;j--){
            L.push_back(list1.at(j).at(i));
        }
        list2 << L;
    }
}

void Q2048NumberManager::transYToX(QVector<QVector<int> > &list1, QVector<QVector<int> > &list2)
{
    for(int i = m_boxMax - 1 ; i >= 0;i--){
        QVector<int> L;
        for(int  j= 0 ; j< m_boxMax;j++){
            L.push_back(list1.at(j).at(i));
        }
        list2 << L;
    }
}
