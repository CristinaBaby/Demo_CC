
#ifndef CandlePosition_h
#define CandlePosition_h

#include "cocos2d.h"

USING_NS_CC;

class CandleData {
    
    
public:
    Vec2 firePos;
    float holderHeight;
    
    static CandleData getCandleData(int type){
        CandleData data;
        data.firePos = Vec2::ZERO;
        data.holderHeight = 0;
        switch (type) {
            case 0:
            {
                data.firePos = Vec2(14,147);
                data.holderHeight = 30;
            }
                break;
            case 1:
            {
                data.firePos = Vec2(58,120);
                data.holderHeight = 31;
            }
                break;
            case 2:
            {
                data.firePos = Vec2(40,109);
                data.holderHeight = 10;
            }
                break;
            case 3:
            {
                data.firePos = Vec2(26,180);
                data.holderHeight = 31;
            }
                break;
                
            default:
                break;
        }
        return data;
    }
};
class CandlePosition
{
public:
    int maxCol;
    int minCol;
    int maxRow;
    
    float marginX;
    float marginY;
    Vec2 orignPos; // min col对应row的第一个坐标位置（从左往右）
    
    class CandlePos{
    public:
        int index;
        Vec2 pos;
    };
    void init(int col,int row,float mgX,float mgY,Vec2 po){
        minCol = col;
        maxRow = row;
        maxCol = col+row/2;
        marginX = mgX;
        marginY = mgY;
        orignPos = po;
    }
    CandlePos containPosition(Vec2 worldPos){
        Vec2 pos = Vec2(orignPos, worldPos);
        int midRow = maxRow/2;
        Rect rect = Rect(-marginX*midRow/2, 0, marginX*maxCol, marginY*maxRow);
        CandlePos candlePos;
        candlePos.index = -1;
        candlePos.pos = Vec2::ZERO;
        
        if (!rect.containsPoint(pos)) {
            return candlePos;
        }
        int count = 0;
        for (int i = 0; i<midRow; i++) {
            for (int j=0; j<minCol+i; j++) {
                Rect rect = Rect(-marginX/2*i+j*marginX, marginY*i, marginX, marginY);
                if (rect.containsPoint(pos)) {
                    candlePos.index = count;
                    candlePos.pos = Vec2(rect.getMidX()+orignPos.x,rect.getMidY()+orignPos.y);
                    break;
                }
                count++;
            }
        }
        for (int i = midRow; i>=0; i--) {
            for (int j=0; j<minCol+i; j++) {
                Rect rect = Rect(-marginX/2*i+j*marginX, marginY*(maxRow-i-1), marginX, marginY);
                if (rect.containsPoint(pos)) {
                    candlePos.index = count;
                    candlePos.pos = Vec2(rect.getMidX()+orignPos.x,rect.getMidY()+orignPos.y);
                    break;
                }
                count++;
            }
        }
        return candlePos;
    }
    Vec2 getPosition(int index){
        int midRow = maxRow/2;
        int count = 0;
        for (int i = 0; i<midRow; i++) {
            for (int j=0; j<minCol+i; j++) {
                if (count == index) {
                    Rect rect = Rect(-marginX/2*i+j*marginX, marginY*i, marginX, marginY);
                    Vec2 pos = Vec2(rect.getMidX()+orignPos.x,rect.getMidY()+orignPos.y);
                    return pos;
                }
                count++;
            }
        }
        for (int i = midRow; i>=0; i--) {
            for (int j=0; j<minCol+i; j++) {
                if (count == index) {
                    Rect rect = Rect(-marginX/2*i+j*marginX, marginY*(maxRow-i-1), marginX, marginY);
                    Vec2 pos = Vec2(rect.getMidX()+orignPos.x,rect.getMidY()+orignPos.y);
                    return pos;
                }
                count++;
            }
        }
        
        return Vec2::ZERO;
    }
    std::vector<Vec2> getPointVector(){
        std::vector<Vec2> pointVector;
        int midRow = maxRow/2;
        for (int i = 0; i<midRow; i++) {
            for (int j=0; j<minCol+i; j++) {
                Rect rect = Rect(-marginX/2*i+j*marginX, marginY*i, marginX, marginY);
                Vec2 pos = Vec2(rect.getMidX()+orignPos.x,rect.getMidY()+orignPos.y);
                pointVector.push_back(pos);
            }
        }
        for (int i = midRow; i>=0; i--) {
            for (int j=0; j<minCol+i; j++) {
                Rect rect = Rect(-marginX/2*i+j*marginX, marginY*(maxRow-i-1), marginX, marginY);
                Vec2 pos = Vec2(rect.getMidX()+orignPos.x,rect.getMidY()+orignPos.y);
                pointVector.push_back(pos);
            }
        }
        return pointVector;
    }
};
#endif /* CandlePosition_h */
