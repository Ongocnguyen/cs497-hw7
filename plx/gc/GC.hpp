#pragma once

#include <list>

#include <plx/gc/RegistrationType.hpp>
#include <plx/object/Object.hpp>

namespace PLX {

    class VM;

    class GC {
    public:

        GC();
        ~GC();

        void setRegistrationType(RegistrationType registrationType);
        void registerObject(Object* obj);
        void registerObject(Object* obj, RegistrationType registrationType);
        void addRoot(Object* obj);

        bool isGcNeeded();

        void collect();
        void mark();
        static void mark(std::vector<Object*>& objs);
        void sweep();

        int nPermanent();
        int nRoots();
        int nRegistered();

        void showPermanent();
        void showRoots();
        void showSpine();

    private:
        RegistrationType _registrationType {RegistrationType::REGULAR};

        int _nPermanent {0};
        std::list<Object*> _permanentObjects {};

        int _nRoots {0};
        std::list<Object*> _rootObjects {};

        int _nRegistered {0};
        size_t _sizeRegistered {0};
        int _nNew {0};
        size_t _sizeNew {0};
        std::list<Object*> _spine {};

        int _nLastReclaimed {0};
        size_t _sizeLastReclaimed {0};

        int _nThreshold {10000};
        size_t _sizeThreshold {100000};

        bool _isGcNeeded {false};

        void _deleteAllObjects();
        void _deleteList(std::list<Object*>& list);
        void _dispose(Object* obj);

    };

}
