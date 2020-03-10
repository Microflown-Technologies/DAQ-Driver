#include "CallbackHandler.h"


void CallbackHandler::runOnce(std::function<void ()> callback)
{
    if(m_invokeThreadID!= std::this_thread::get_id()) std::lock_guard<std::mutex> m_callbackFunctionMutexLocker(m_callbackFunctionsMutex);
    std::shared_ptr<std::function<void(void)>> newCallback (new std::function<void(void)>(callback));
    m_runOnceFunctions.push_back(newCallback);
}

std::shared_ptr<std::function<void (void)> > CallbackHandler::addCallback(std::function<void (void)> callback)
{
    // Copy callback to shared pointer
    std::shared_ptr<std::function<void(void)>> newCallback (new std::function<void(void)>(callback));
    // Add callback and return copied function shared pointer
    addCallback(newCallback);
    return newCallback;
}

void CallbackHandler::addCallback(std::shared_ptr<std::function<void (void)> > callback) {
    // Lock m_callbackFunctionsMutex and insert into m_callbackFunctions
    if(m_invokeThreadID!= std::this_thread::get_id()) std::lock_guard<std::mutex> m_callbackFunctionMutexLocker(m_callbackFunctionsMutex);
    m_callbackFunctions.push_back(callback);
}

bool CallbackHandler::removeCallback(std::shared_ptr<std::function<void (void)> > callback)
{
    //Find callback inside m_callbackFunctions and remove if found
    if(m_invokeThreadID!= std::this_thread::get_id()) std::lock_guard<std::mutex> m_callbackFunctionMutexLocker(m_callbackFunctionsMutex);
    for(auto callbackIterator = m_callbackFunctions.begin(); callbackIterator != m_callbackFunctions.end(); callbackIterator++) {
        if(*callbackIterator == callback) {
            m_callbackFunctions.erase(callbackIterator);
            return true;
        }
    }
    //Return false if nothing is removed/found
    return false;
}

void CallbackHandler::invokeCallbacks() {
    m_invokeThreadID = std::this_thread::get_id();
    std::lock_guard<std::mutex> m_callbackFunctionMutexLocker(m_callbackFunctionsMutex);
    for(auto callback: m_callbackFunctions) {
        (*callback)();
    }
    for(auto callback: m_runOnceFunctions) {
        (*callback)();
    }
    m_runOnceFunctions.clear();
}
