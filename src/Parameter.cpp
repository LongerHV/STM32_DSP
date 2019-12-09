#include "Parameter.h"
#include "stdio.h"

Parameter::Parameter(const char *name, const char *unit, int32_t min, int32_t max, int32_t step, int32_t initial) {
    this->SetName(name);
    this->SetUnit(unit);
    this->min = min;
    this->max = max;
    this->step = step;
    this->value = initial;
    this->UpdateValRepr();
}

void Parameter::SetName(const char *name){
    uint8_t i;
    for (i = 0; name[i] != '\000' && i < 9; i++) {
        this->name[i] = name[i];
    }
    for (; i < 9; i++) {
        this->name[i] = ' ';
    }
    this->name[9] = '\000';
}

void Parameter::SetUnit(const char *unit) {
    for(uint8_t i = 0; i < 3; i++) {
        this->unit[i] = unit[i];
    }
}

char *Parameter::GetName() {
    return &this->name[0];
}

char *Parameter::GetValRepr() {
    return &this->val_repr[0];
}

int32_t Parameter::GetValue() {
    return this->value;
}

int32_t Parameter::GetMin() {
    return this->min;
}

int32_t Parameter::GetMax() {
    return this->max;
}

int32_t Parameter::GetStep() {
    return this->step;
}

void Parameter::SetValue(int32_t value) {
    this->value = value;
}

void Parameter::UpdateValRepr() {
    int a, b = 0;
    char prefix = '\000';
    if(this->unit[0] == 's'){
        if (this->value >= 1000){
            a = this->value == 1000;
            b = (this->value % 1000) * 0.1;
        } else {
            a = this->value;
            prefix = 'm';
        }
    } else if (this->unit[0] == 'H' && this->unit[1] == 'z') {
        if (this->name[0] == 'F'){
            if (this->value < 1000){
                a = this->value;
            } else {
                a = this->value * 0.001;
                b = this->value % 1000 * 0.01;
                prefix = 'k';
            }
        } else {
            a = this->value * 0.001;
            b = (this->value % 1000) * 0.01;
        }
    } else if (this->unit[0] == '%') {
        a = this->value;
    } else if (this->unit[0] == ' ') {
        a = this->value;
    } else {
        a = this->value * 0.001;
        b = (this->value % 1000) * 0.01;
    }

    if(b){
        if(prefix){
            snprintf(this->val_repr, 6, "%d.%d%c%s  ", a, b, prefix, this->unit);
        } else {
            snprintf(this->val_repr, 6, "%d.%d%s   ", a, b, this->unit);
        }
    } else {
        if(prefix) {
            snprintf(this->val_repr, 6, "%d%c%s    ", a, prefix, this->unit);
        } else {
            snprintf(this->val_repr, 6, "%d%s     ", a, this->unit);
        }
    }
}
