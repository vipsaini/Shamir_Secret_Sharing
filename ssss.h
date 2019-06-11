#ifndef SSSS_H
#define SSSS_H
using namespace std;

/* 
    ------------------------------------------------------------------------
    create_shares() -> It is used to convert the secret byte array into shares
    array.
    Here,
    secrets[] => byte array containing the secret.
    n => number of shares array to be generated.
    k => minimum number of shares needed to recreate the secret.
    **shares =>  n-byte arrays storing n-shares.
    ------------------------------------------------------------------------
*/

void create_shares(uint8_t secrets[], uint8_t n, uint8_t k, uint8_t **shares, uint8_t payload);

/*
    ------------------------------------------------------------------------
    has_sufficient_shares() -> Checks whether the user has sufficient number 
    of shares or not.
    Here, 
    x => number of shares user has.
    ------------------------------------------------------------------------
*/
bool has_sufficient_shares(int x);

/*
    ------------------------------------------------------------------------
    extract_secret() -> Recreates the mnemonic recovery seed from the 
    recovery shares user has entered.
    Here, 
    x => number of shares user has.
    **r_shares => stores x recovery shares user has entered.
    secrets[] => will store the computed mnemonic recovery seed by the 
    function.
    ------------------------------------------------------------------------
*/
void extract_secret(uint8_t **r_shares, uint8_t secret_array[], uint8_t x, uint8_t payload);

#endif