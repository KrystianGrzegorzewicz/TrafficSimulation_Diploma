#include "behavior/longitudinal/FuzzyLongitudinalModel.h"



// Number of inputs to the fuzzy inference system
const int fis_gcI = 3;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 1;
// Number of rules to the fuzzy inference system
const int fis_gcR = 8;

FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];


// Loop routine runs over and over again forever:
/*void loop()
{
    // Read Input: input1
    g_fisInput[0] = analogRead(0);
    // Read Input: input2
    g_fisInput[1] = analogRead(1);
    // Read Input: input3
    g_fisInput[2] = analogRead(2);

    g_fisOutput[0] = 0;

    fis_evaluate();

    // Set output vlaue: output
    analogWrite(3, g_fisOutput[0]);

}*/

//***********************************************************************
// Support functions for Fuzzy Inference System                          
//***********************************************************************
// Gaussian Member Function
FIS_TYPE fis_gaussmf(FIS_TYPE x, FIS_TYPE* p)
{
    FIS_TYPE s = p[0], c = p[1];
    FIS_TYPE t = (x - c) / s;
    return exp(-(t * t) / 2);
}

FIS_TYPE fis_prod(FIS_TYPE a, FIS_TYPE b)
{
    return (a * b);
}

FIS_TYPE fis_probor(FIS_TYPE a, FIS_TYPE b)
{
    return (a + b - (a * b));
}

FIS_TYPE fis_sum(FIS_TYPE a, FIS_TYPE b)
{
    return (a + b);
}

FIS_TYPE fis_array_operation(FIS_TYPE* array, int size, _FIS_ARR_OP pfnOp)
{
    int i;
    FIS_TYPE ret = 0;

    if (size == 0) return ret;
    if (size == 1) return array[0];

    ret = array[0];
    for (i = 1; i < size; i++)
    {
        ret = (*pfnOp)(ret, array[i]);
    }

    return ret;
}


//***********************************************************************
// Data for Fuzzy Inference System                                       
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[] =
{
    fis_gaussmf
};

// Count of member function for each Input
int fis_gIMFCount[] = { 2, 2, 2 };

// Count of member function for each Output 
int fis_gOMFCount[] = { 8 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 42.4793040877466, -0.0020348331496122 };
FIS_TYPE fis_gMFI0Coeff2[] = { 42.507653835852, 99.960997651644 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2 };
FIS_TYPE fis_gMFI1Coeff1[] = { 6.36661688585403, -0.196258324243124 };
FIS_TYPE fis_gMFI1Coeff2[] = { 6.56908747235236, 14.623516733152 };
FIS_TYPE* fis_gMFI1Coeff[] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2 };
FIS_TYPE fis_gMFI2Coeff1[] = { 4.4673029765825, -5.37939717039347 };
FIS_TYPE fis_gMFI2Coeff2[] = { 4.44134496576034, 5.60729189271456 };
FIS_TYPE* fis_gMFI2Coeff[] = { fis_gMFI2Coeff1, fis_gMFI2Coeff2 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff, fis_gMFI1Coeff, fis_gMFI2Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { -0.790368452929576, -2.70131581204518, -6.3512213779997, 19.3242235402494 };
FIS_TYPE fis_gMFO0Coeff2[] = { 0.642470376698839, -0.275777619180442, 1.87958213662771, -12.8884789994241 };
FIS_TYPE fis_gMFO0Coeff3[] = { 2.08921844938907, -0.903248059946396, -0.118310339006397, -22.3020609340999 };
FIS_TYPE fis_gMFO0Coeff4[] = { -0.820868948077502, 1.27378125196083, -4.99612341105157, 9.71868662090781 };
FIS_TYPE fis_gMFO0Coeff5[] = { 0.467362333730111, -3.14516044404915, -9.31310069850738, -70.3602228658405 };
FIS_TYPE fis_gMFO0Coeff6[] = { -0.750500646902362, -0.676821738305917, -19.6632796939279, 94.4251333563198 };
FIS_TYPE fis_gMFO0Coeff7[] = { 1.64901444912593, -1.78132403051914, 8.48729038065619, -77.0917650181272 };
FIS_TYPE fis_gMFO0Coeff8[] = { -0.230256961320522, -5.47309823970646, 10.7878635476302, 62.4996074317181 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3, fis_gMFO0Coeff4, fis_gMFO0Coeff5, fis_gMFO0Coeff6, fis_gMFO0Coeff7, fis_gMFO0Coeff8 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 0 };
int fis_gMFI1[] = { 0, 0 };
int fis_gMFI2[] = { 0, 0 };
int* fis_gMFI[] = { fis_gMFI0, fis_gMFI1, fis_gMFI2 };

// Output membership function set

//int* fis_gMFO[] = {};

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1, 1, 1 };
int fis_gRI1[] = { 1, 1, 2 };
int fis_gRI2[] = { 1, 2, 1 };
int fis_gRI3[] = { 1, 2, 2 };
int fis_gRI4[] = { 2, 1, 1 };
int fis_gRI5[] = { 2, 1, 2 };
int fis_gRI6[] = { 2, 2, 1 };
int fis_gRI7[] = { 2, 2, 2 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7 };

// Rule Outputs
int fis_gRO0[] = { 1 };
int fis_gRO1[] = { 2 };
int fis_gRO2[] = { 3 };
int fis_gRO3[] = { 4 };
int fis_gRO4[] = { 5 };
int fis_gRO5[] = { 6 };
int fis_gRO6[] = { 7 };
int fis_gRO7[] = { 8 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 0.0100708, 0, -5.35685 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 100, 14.9253, 5.50003 };

// Output range Min
FIS_TYPE fis_gOMin[] = { -5.5 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 3.5 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System           
//***********************************************************************
// None for Sugeno

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate()
{
    FIS_TYPE fuzzyInput0[] = { 0, 0 };
    FIS_TYPE fuzzyInput1[] = { 0, 0 };
    FIS_TYPE fuzzyInput2[] = { 0, 0 };
    FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, fuzzyInput1, fuzzyInput2, };
    FIS_TYPE fuzzyOutput0[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    FIS_TYPE* fuzzyOutput[fis_gcO] = { fuzzyOutput0, };
    FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
    FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
    FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
    FIS_TYPE sW = 0;

    // Transforming input to fuzzy Input
    int i, j, r, o;
    for (i = 0; i < fis_gcI; ++i)
    {
        for (j = 0; j < fis_gIMFCount[i]; ++j)
        {
            fuzzyInput[i][j] =
                (fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
        }
    }

    int index = 0;
    for (r = 0; r < fis_gcR; ++r)
    {
        if (fis_gRType[r] == 1)
        {
            fuzzyFires[r] = 1;
            for (i = 0; i < fis_gcI; ++i)
            {
                index = fis_gRI[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_prod(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_prod(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_prod(fuzzyFires[r], 1);
            }
        }
        else
        {
            fuzzyFires[r] = 0;
            for (i = 0; i < fis_gcI; ++i)
            {
                index = fis_gRI[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_probor(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_probor(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_probor(fuzzyFires[r], 0);
            }
        }

        fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
        sW += fuzzyFires[r];
    }

    if (sW == 0)
    {
        for (o = 0; o < fis_gcO; ++o)
        {
            g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
        }
    }
    else
    {
        for (o = 0; o < fis_gcO; ++o)
        {
            FIS_TYPE sWI = 0.0;
            for (j = 0; j < fis_gOMFCount[o]; ++j)
            {
                fuzzyOutput[o][j] = fis_gMFOCoeff[o][j][fis_gcI];
                for (i = 0; i < fis_gcI; ++i)
                {
                    fuzzyOutput[o][j] += g_fisInput[i] * fis_gMFOCoeff[o][j][i];
                }
            }

            for (r = 0; r < fis_gcR; ++r)
            {
                index = fis_gRO[r][o] - 1;
                sWI += fuzzyFires[r] * fuzzyOutput[o][index];
            }

            g_fisOutput[o] = sWI / sW;
        }
    }
}

float FuzzyLongitudinalModel::computeAcceleration(
    const CarState& self,
    const PerceptionState& perception,
    float desiredSpeed,
    float maxAccel,
    float maxDecel
)
{
    g_fisInput[0] = perception.distanceToCarAhead;
    g_fisInput[1] = perception.relativeSpeed;
    g_fisInput[2] = perception.relativeAcceleration;

    g_fisOutput[0] = 0;

    fis_evaluate();

    return g_fisOutput[0];
}