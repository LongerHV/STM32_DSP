#include "Biquad.h"

Biquad::Biquad(const char *name) {
    this->SetName(name);

    this->filter_instance_L = new arm_biquad_cascade_df2T_instance_f32;
    this->filter_instance_R = new arm_biquad_cascade_df2T_instance_f32;

    // arm_biquad_cascade_df1_init_f32(this->filter_instance_L, 1, new float32_t[5], new float32_t[4]);
    // arm_biquad_cascade_df1_init_f32(this->filter_instance_R, 1, new float32_t[5], new float32_t[4]);

    this->filter_instance_L->numStages = 1;
    this->filter_instance_R->numStages = 1;
    this->filter_instance_L->pCoeffs = new float32_t[5];
    this->filter_instance_R->pCoeffs = new float32_t[5];
    this->filter_instance_L->pState = new float32_t[2];
    this->filter_instance_R->pState = new float32_t[2];
    arm_fill_f32(0.0, this->filter_instance_L->pState, 2);
    arm_fill_f32(0.0, this->filter_instance_R->pState, 2);

    this->number_of_parameters = 4;
    this->current_parameter = 0;
    this->Fs = 48000;
    this->type = 0;

    this->parameters[0] = new Parameter("Type", " ", 0, 6, 1, 0);
    this->parameters[1] = new Parameter("Frequency", "Hz", 100, 20000, 100, 10000);
    this->parameters[2] = new Parameter("Q", "", 0, 5000, 100, 1000);
    this->parameters[3] = new Parameter("Peak gain", "", 1000, 2000, 100, 1000);
    for (uint8_t i = 4; i < 10; i++) parameters[i] = NULL;

    this->UpdateParameters();
    this->RecalculateCoeffitients();
}

Biquad::~Biquad() {
}

void Biquad::RecalculateCoeffitients() {
    uint8_t type = (uint8_t)this->type;
    float32_t K = arm_tan(PI * this->Fc / this->Fs);
    float32_t V;
    float32_t norm;
    float32_t a0, a1, a2, b1, b2;

    if (type > HIGHSHELF)
        type = HIGHSHELF;
    if (type == PEAK || type == LOWSHELF || type == HIGHSHELF)
        V = pow(10, fabs(peak_gain / 20));

    switch (type) {
        case LOWPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;

        case HIGHPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;

        case BANDPASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K / Q * norm;
            a1 = 0;
            a2 = -a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
        
        case NOTCH:
            norm = 1 / (1 + K / Q + K * K);
            a0 = (1 + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = a0;
            b1 = a1;
            b2 = (1 - K / Q + K * K) * norm;
            break;
        
        case PEAK:
            if (peak_gain >= 0) {    // boost
                norm = 1 / (1 + 1/Q * K + K * K);
                a0 = (1 + V/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - V/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - 1/Q * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + V/Q * K + K * K);
                a0 = (1 + 1/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - 1/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - V/Q * K + K * K) * norm;
            }
            break;
        case LOWSHELF:
            if (peak_gain >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
                a1 = 2 * (V * K * K - 1) * norm;
                a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (V * K * K - 1) * norm;
                b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
            }
            break;
        case HIGHSHELF:
            if (peak_gain >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (V + sqrt(2*V) * K + K * K) * norm;
                a1 = 2 * (K * K - V) * norm;
                a2 = (V - sqrt(2*V) * K + K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (V + sqrt(2*V) * K + K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (K * K - V) * norm;
                b2 = (V - sqrt(2*V) * K + K * K) * norm;
            }
            break;

        default:
            break;
    }
    this->filter_instance_L->pCoeffs[0] = a0;
    this->filter_instance_L->pCoeffs[1] = b1;
    this->filter_instance_L->pCoeffs[2] = b2;
    this->filter_instance_L->pCoeffs[3] = a1;
    this->filter_instance_L->pCoeffs[4] = a2;

    this->filter_instance_R->pCoeffs[0] = a0;
    this->filter_instance_R->pCoeffs[1] = b1;
    this->filter_instance_R->pCoeffs[2] = b2;
    this->filter_instance_R->pCoeffs[3] = a1;
    this->filter_instance_R->pCoeffs[4] = a2;
}

void Biquad::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size) {
    arm_biquad_cascade_df2T_f32(this->filter_instance_L, pData_left, pData_left, block_size);
    arm_biquad_cascade_df2T_f32(this->filter_instance_R, pData_right, pData_right, block_size);
}

void Biquad::UpdateParameters(){
    this->type = this->parameters[0]->GetValue();
    this->Fc = this->parameters[1]->GetValue();
    this->Q = this->parameters[2]->GetValue() * 0.001;
    this->peak_gain = this->parameters[3]->GetValue() * 0.001;

    this->RecalculateCoeffitients();
}