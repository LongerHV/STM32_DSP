#include "Biquad.h"

Biquad::Biquad(cosnt char *name){
    this->SetName(name);

    this->filter_instance_L = new arm_biquad_casd_df1_inst_f32;
    this->filter_instance_R = new arm_biquad_casd_df1_inst_f32;

    this->filter_instance_L->numStages = 1;
    this->filter_instance_R->numStages = 1;
    this->filter_instance_L->pCoeffs = new float32_t[5];
    this->filter_instance_R->pCoeffs = new float32_t[5];
    this->filter_instance_L->pState = new float32_t[4];
    this->filter_instance_R->pState = new float32_t[4];

    this->number_of_parameters = 3;
    this->current_parameter = 0;
    this->Fs = 48000;
    this->type = 0;

    this->parameters[0] = new Parameter_uint32("Type", this->type, 100);
    this->parameters[1] = new Parameter_float32("Frequency", this->Fc, 20000);
    this->parameters[2] = new Parameter_float32("Q", this->Q, 5);
    this->parameters[3] = new Parameter_float32("Peek gain", this->peek_gain, 2);
    for (uint8_t i = 4; i < 10; i++) parameters[i] = NULL;
}

Biquad::~Biquad(){

}

void Biquad::RecalculateCoeffitients(){
    float32_t K = arm_tan(PI * this->Fc / this->Fs);
    float32_t V;
    float32_t a0, a1, a2, b1, b2;
    if(this->type > HIGHSHELF)
        this->type = HIGHSHELF;
    if(this->type == PEEK || this->type == LOWSHELF || this->type == HIGHSHELF)
        V = pow(10, fabs(this->peek_gain / 20));
    switch(this->type) {
        case LOWPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - l / Q + K * K) * norm;
            break;
        default:
            break;
    }
    this->filter_instance_L->pCoeffs[0] = a0;
    this->filter_instance_L->pCoeffs[1] = a1; 
    this->filter_instance_L->pCoeffs[2] = a2;
    this->filter_instance_L->pCoeffs[3] = b1;
    this->filter_instance_L->pCoeffs[4] = b2;

    this->filter_instance_R->pCoeffs[0] = a0;
    this->filter_instance_R->pCoeffs[1] = a1; 
    this->filter_instance_R->pCoeffs[2] = a2;
    this->filter_instance_R->pCoeffs[3] = b1;
    this->filter_instance_R->pCoeffs[4] = b2;
}

void Biquad::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size){
    arm_biquad_cascade_df1_f32(this->filter_instance_L, pData_left, pData_left, block_size);
    arm_biquad_cascade_df1_f32(this->filter_instance_R, pData_right, pData_right, block_size);
}
