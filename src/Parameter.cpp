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
    for (i = 0; name[i] != '\000' && i < 10; i++) {
        this->name[i] = name[i];
    }
    for (; i < 10; i++) {
        this->name[i] = ' ';
    }
    this->name[10] = '\000';
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
    int a, b;
    uint8_t point;
    if(this->unit[0] == 's'){
        a = this->value == 1000;
        b = (this->value % 1000) * 0.1;
        point = 1;
    } else if (this->unit[0] == '%') {
        a = this->value;
        point = 0;
    } else if (this->unit[0] == ' ') {
        a = this->value;
        point = 0;
    } else if (this->unit[0] == 'm' && this->unit[1] == 's') {
        a = this->value;
        point = 0;
    } else if (this->unit[0] == 'H' && this->unit[1] == 'z') {
        if (this->max > 10){
            a = this->value;
            point = 0;
        } else {
            a = this->value * 0.001;
            b = (this->value % 1000) * 0.001;
            point = 1;
        }
    } else {
        a = this->value * 0.001;
        b = (this->value % 1000) * 0.001;
        point = 1;
    }

    if(point){
        snprintf(this->val_repr, 6, "%d.%d%s   ", a, b, this->unit);
    } else {
        snprintf(this->val_repr, 6, "%d%s     ", a, this->unit);
    }
}
