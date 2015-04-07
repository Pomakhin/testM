//
//  GenericSingleton.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__GenericSingleton__
#define __MidasMiner__GenericSingleton__

#include <stdio.h>
template <class T>
class Singleton
{
private:
    static T* m_instance;
public:
    static T* getInstance()
    {
        if (!m_instance)
            m_instance = new T();
        
        return m_instance;
    }    
    static void destroyInstance()
    {
        delete m_instance;
        m_instance = nullptr;
    }
};

template <class T> T* Singleton<T>::m_instance = nullptr;

#endif /* defined(__MidasMiner__GenericSingleton__) */
