//
//  GameObject.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__GameObject__
#define __MidasMiner__GameObject__

#include <stdio.h>
#include <string>
#include "Utils.h"

class SDL_Renderer;

class BaseGameObject
{
public:
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual bool update() = 0;
    virtual void load(const Point &pos, const std::string &textureId) = 0;
    virtual bool isIntersect(const Point &point) = 0;
    virtual void moveTo(const Point &toPos, const float &acceleration = 0.0f) = 0;
    virtual Point getPos() = 0;
    virtual void setAlpha(int value) = 0;
    virtual int getAlpha() = 0;
};

class GameObject : public BaseGameObject
{
protected:
    std::string m_textureId;
    Point m_pos;
    Point m_destination;
    float m_xVelocity = 0.0f;
    float m_yVelocity = 0.0f;
    float m_acceleration = 0.0f;
    int m_alpha = 255;
public:
    virtual void draw(SDL_Renderer* renderer) override;
    virtual bool update() override;
    virtual void load(const Point &pos, const std::string &textureId) override;
    virtual bool isIntersect(const Point &point) override;
    virtual void moveTo(const Point &toPos, const float &acceleration) override;
    virtual Point getPos() override {return m_pos;}
    virtual void setAlpha(int value) override {m_alpha = value;}
    virtual int getAlpha() override {return m_alpha;}
};

class GameObjectDecorator : public BaseGameObject
{
protected:
    std::unique_ptr<BaseGameObject> m_gameObject;
public:
    GameObjectDecorator(std::unique_ptr<BaseGameObject> obj):
        m_gameObject(std::move(obj)){}
    virtual void draw(SDL_Renderer* renderer) override;
    virtual bool update() override;
    virtual void load(const Point &pos, const std::string &textureId) override;
    virtual bool isIntersect(const Point &point) override;
    virtual void moveTo(const Point &toPos, const float &acceleration) override;
    virtual Point getPos() override;
    virtual void setAlpha(int value) override;
    virtual int getAlpha() override;
};

class SelectedDecorator : public GameObjectDecorator
{
public:
    SelectedDecorator(std::unique_ptr<BaseGameObject> obj):
        GameObjectDecorator(std::move(obj)) {}
    void draw(SDL_Renderer *renderer) override;
};

class HintDecorator : public GameObjectDecorator
{
private:    
    bool m_alphaIncrease = false;
public:
    HintDecorator(std::unique_ptr<BaseGameObject> obj):
        GameObjectDecorator(std::move(obj)) {}
    bool update() override;    
};



#endif /* defined(__MidasMiner__GameObject__) */
