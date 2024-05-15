#pragma once

namespace PLX {

    class List;
    class Object;

    bool pArray(List*& tokens, Object*& value);
    bool pBinding(List*& tokens, Object*& value);
    bool pBindingList(int minCount, List*& tokens, Object*& value);
    bool pList(List*& tokens, Object*& value);
    bool pQueue(List*& tokens, Object*& value);
    bool pHashTable(List*& tokens, Object*& value);
    bool pData(List*& tokens, Object*& value);
    bool pDataWithoutBinding(List*& tokens, Object*& value);

}
