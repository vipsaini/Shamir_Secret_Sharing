#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Entropy.h>
#include "ssss.h"
#include "gf256.h"

//typedef unsigned char byte;

using namespace std;
/* threshold value stores the minimum no. of shares needed to recreate the secret */
static uint8_t threshold;

static const int prime = 256;

/*
    ------------------------------------------------------------------------
            Power() -> Computes a ^ x in galois field(256).
    ------------------------------------------------------------------------
*/

uint8_t Power(uint8_t a, uint8_t x)
{
    uint8_t result = 1;

    // Multiply the result in galois field(256) x times.
    for (int i = 0; i < x; i++)
    {
        result = mul(result, a);
    }

    return result;
}

void create_shares(uint8_t data[], uint8_t n, uint8_t k, uint8_t **shares, uint8_t payload)
{

    threshold = k;
    Entropy.initialize();

    uint8_t **coeff = new uint8_t *[k];
    for (int i = 0; i < k; i++)
    {
        coeff[i] = new uint8_t[payload];
    }

    for (int i = 0; i < payload; i++)
    {
        // lowest degree term is our secret.
        coeff[0][i] = data[i];
        for (int j = 1; j < k; j++)
        {
            // Initialising the coefficients with the random variables.
            coeff[j][i] = Entropy.randomByte();
        }
    }

    // Creating n shares.
    for (int i = 0; i < n; i++)
    {
        shares[i][0] = uint8_t(i + 1);
        for (int j = 0; j < payload; j++)
        {
            shares[i][j + 1] = coeff[0][j];
            for (int x = 1; x < k; x++)
            {

                uint8_t temp = Power(uint8_t(i + 1), x);
                shares[i][j + 1] = add(shares[i][j + 1], mul(coeff[x][j], temp));
            }
        }

        shares[i][payload + 1] = k;
    }

    // Deallocating the memory alloted to the **coeff array.
    for (int i = 0; i < k; i++)
    {
        delete coeff[i];
    }
}

bool has_sufficient_shares(int x)
{
    // Returns true if x is greater than or equal to threshold.
    return (x >= threshold);
}

/*
    ------------------------
    LAGRANGE's INTERPOLATION
    ------------------------
*/

void extract_secret(uint8_t **r_shares, uint8_t secret_array[], uint8_t n, uint8_t payload)
{
    uint8_t temp[payload][n][2];
    for (int i = 0; i < payload; i++)
    {
        for (int j = 0; j < n; j++)
        {
            temp[i][j][0] = uint8_t(r_shares[j][0]);
            temp[i][j][1] = uint8_t(r_shares[j][i + 1]);
        }
    }

    // uint8_t secret_array[33];
    uint8_t x = 0;
    for (int i = 0; i < payload; i++)
    {
        uint8_t secret = 0;
        for (int j = 0; j < n; j++)
        {
            uint8_t num = 1;
            uint8_t den = 1;

            for (int k = 0; k < n; k++)
            {
                if (j != k)
                {
                    //  num = (num * -temp[i][k][0]) % prime;
                    num = mul(num, sub(x, temp[i][k][0]));
                    //den = (den * (temp[i][j][0] - temp[i][k][0])) % prime;
                    den = mul(den, sub(temp[i][j][0], temp[i][k][0]));
                }
            }

            uint8_t value = temp[i][j][1];

            secret = add(secret, mul(value, div(num, den)));
        }
        secret = (secret + prime) % prime;
        secret_array[i] = secret;
    }

    // int middle_array[24];
    // back_to_original_array(byte_array, middle_array);
    // recover_phrase(middle_array, secrets);
    //recover_phrase(secret_array, secret);
}
