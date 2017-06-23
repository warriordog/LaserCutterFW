#ifndef GCODE_H
#define GCODE_H

namespace gcode {
    struct Field {
        char letter;
        long iNum;
        float fNum;
    };
    
    struct FieldNode {
        Field* field = nullptr;
        FieldNode* next = nullptr;
    };
    
    class FieldList {
        private:
            FieldNode* head = nullptr;
        public:
            ~FieldList();
            FieldNode* getHead() {return head;}
            void add(Field* field);
    };
    
    void execute(Field* command, FieldList* args);
    bool isWorking();
    void update();
    void abortCurrentCommand();
}

#endif //GCODE_H
