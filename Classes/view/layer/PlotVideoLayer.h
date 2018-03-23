//
//  PlotVideoLayer.h
//  life
//
//  Created by ff on 17/10/14.
//
//

#ifndef PlotVideoLayer_h
#define PlotVideoLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIVideoPlayer.h"

class PlotVideoLayer : public cocos2d::LayerColor {
public:
    PlotVideoLayer();
    ~PlotVideoLayer();
    
    bool init();
    void showVideo(int videoId, int missionId);
    
    void clickSkip();
    void videoPlayOverCallback();

    /**
     * 视频播放状态，只有在android和ios平台有效
     */
    void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);

    CREATE_FUNC(PlotVideoLayer);
private:
    cocos2d::Layer *skipLayer;
    cocos2d::experimental::ui::VideoPlayer *videoPlayer;
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* PlotVideoLayer_h */
