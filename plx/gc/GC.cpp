#include <cassert>
#include <iostream>
#include <vector>

#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    GC::GC() {}

    GC::~GC() {
        _deleteAllObjects();
    }

    void GC::addRoot(Object* obj) {
        _rootObjects.push_back(obj);
        _nRoots++;
    }

    void GC::setRegistrationType(RegistrationType registrationType) {
        _registrationType = registrationType;
    }

    void GC::registerObject(Object* obj) {
        registerObject(obj, _registrationType);
    }

    void GC::registerObject(Object* obj, RegistrationType registrationType) {
        switch (registrationType) {
            case RegistrationType::REGULAR:
                {
                    _spine.push_back(obj);
                    size_t size = sizeof(*obj);
                    _nRegistered++;
                    _sizeRegistered += size;
                    _nNew++;
                    _sizeNew += size;
                    if (_nNew >= _nThreshold || _sizeNew >= _sizeThreshold) {
                        _isGcNeeded = true;
                    }
                }
                break;
            case RegistrationType::PERMANENT:
                // permanent objects are never marked or swept
                _permanentObjects.push_back(obj);
                _nPermanent++;
                break;
            // case RegistrationType::UNREGISTERED:
            //     break;
            default:
                assert(false);
                break;
        }
    }

    void GC::collect() {
        _nLastReclaimed = 0;
        _sizeLastReclaimed = 0;
        mark();
        sweep();
        _nNew = 0;
        _sizeNew = 0;
        _isGcNeeded = false;
    }

    bool GC::isGcNeeded() {
        return _isGcNeeded;
    }

    void GC::mark() {
       std::vector<Object*> objs;

    // Iterate over _rootObject list
    for (auto rootObj : _rootObjects) {
        // Call mark function on each object
        rootObj->mark(objs);
    }

    // Mark objects recursively
    while (!objs.empty()) {
        // Pop the last object from the vector
        Object* obj = objs.back();
        objs.pop_back();
        
        // Call mark function on the object
        obj->mark(objs);
    }
}

    void GC::mark(std::vector<Object*>& objs) {
        // Loop until objs vector becomes empty
    while (!objs.empty()) {
        // Pop the last object from the vector
        Object* obj = objs.back();
        objs.pop_back();
        
        // Call mark function on the object
        obj->mark(objs);
    }
}

    void GC::sweep() {
       // Iterate over _spine
    auto it = _spine.begin();
    while (it != _spine.end()) {
        Object* obj = *it;
        
        // If the object is marked, unmark it
        if (obj->isMarked()) {
            obj->setMark(false);
            ++it; // Move to the next object
        } else {
            // If the object is not marked, remove it from _spine and delete
            delete obj;
            it = _spine.erase(it);
        }
    }
}

    int GC::nPermanent() {
        return _nPermanent;
    }

    int GC::nRoots() {
        return _nRoots;
    }

    int GC::nRegistered() {
        return _nRegistered;
    }

    void GC::showPermanent() {
        int n = 0;
        for (Object* obj : _permanentObjects) {
            std::clog << n++ << ". " << obj << " :: " << obj->typeName() << "\n";
        }
    }

    void GC::showRoots() {
        int n = 0;
        for (Object* obj : _rootObjects) {
            std::clog << n++ << ". " << obj << " :: " << obj->typeName() << "\n";
        }
    }

    void GC::showSpine() {
        int n = 0;
        for (Object* obj : _spine) {
            std::clog << n++ << ". " << obj << " :: " << obj->typeName() << "\n";
        }
    }

    void GC::_deleteAllObjects() {
        _deleteList(_spine);
        _nRegistered = 0;
        _deleteList(_permanentObjects);
        _nPermanent = 0;
        _nRoots = 0;
    }

    void GC::_deleteList(std::list<Object*>& list) {
        while (!list.empty()) {
            Object* obj = list.front();
            list.pop_front();
            _dispose(obj);
        }
        assert(list.size() == 0);
    }

    void GC::_dispose(Object* obj) {
        _nLastReclaimed++;
        size_t size = sizeof(*obj);
        _sizeLastReclaimed += size;
        delete obj;
        _nRegistered--;
        _sizeRegistered -= size;
    }
}

