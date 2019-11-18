#ifndef PARAMETER_HPP
#define PARAMETER_HPP


class Parameter{
    private:
        char name[11];
        uint8_t value;
        friend class Effect;

    public:
        Parameter();
        char* GetName();
        uint8_t GetValue();
        void IncrementValue();
        void DecrementValue();

};


#endif