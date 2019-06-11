#include <ssss.h>
#include <Entropy.h>
#include <wordlist.h>

char buffer[12][12];

void setup() {
  // put your setup code here, to run once:
  /* Initialising the index array with random values from 0 - 2047 */
  uint16_t index[12];
  for(uint8_t i = 0; i < 12; i++) {
     index[i] = Entropy.random(2047);
  }

  for(int i = 0; i < 12; i++) {
      strcpy_P(buffer[i], (char*)pgm_read_word(&(string_table[index[i]])));
  }
  /* Buffer stores the mnemonic sentence where every index stores a word of the sentence */
  for(uint8_t i = 0; i < 12; i++) {
    Serial.println(buffer[i]);
  }

  /* Concatenating the words together in a sentence */
  String conc_string = "";
  for(int i = 0; i < 12; i++) {
    conc_string += (String)buffer[i];
        
    if(i == 11) {
      continue;
    }
    conc_string += ",";
              
  }
           
  /* payload is the total length of the concatenated string */       
  uint8_t payload = conc_string.length();
  //uint8_t *data = new uint8_t[payload];
  uint8_t data[payload];

  /* data array stores the ASCII values of the characters in the concatenated string */
  for(int i = 0; i < payload; i++) {
     data[i] = conc_string[i];
  }

  for(int i = 0; i < payload; i++) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
   
  /* n_shares is the number of shares generated */       
  uint8_t n_shares = 5;
  /* threshold is the minimum number of shares required to recreate the secret */
  uint8_t threshold = 2;

  /* **shares will store all the shares generated */
  uint8_t **shares = new uint8_t *[n_shares];
        
  for(int i = 0; i < n_shares; i++) {
    shares[i] = new uint8_t[payload + 2];
  }

  /* Generation of shares */
  create_shares(data, n_shares, threshold, shares, payload);

  /* **r_shares will store the shares used for recovery of secret */
   uint8_t **r_shares = new uint8_t *[2]; 

   r_shares[0] = shares[0];
   r_shares[1] = shares[1];


   /* Stores the ASCII values of the secret */
   uint8_t secret_array[payload];

   /* Recreation of secrets */
   extract_secret(r_shares, secret_array, 2 , payload);

   for(uint8_t i = 0; i < payload; i++) {
      Serial.print(secret_array[i]);
      Serial.print(" ");
   }

   /* final_secret restores the mnemonic phrase */
   String final_secret[12];
   uint8_t j = 0;
   for(uint8_t i = 0; i < 12; i++) {
   String temp = "";
   while(j < payload) {
      if(secret_array[j] == 44){
            final_secret[i] = temp;
            j++;
            break;
      }
        
     temp += char(secret_array[j]);
     j++;
   }        
   if(j == payload){
      final_secret[i] = temp;
   }
  }


  for(uint8_t i = 0; i < 12; i++) {
      Serial.println(final_secret[i]);
  }
  

  

}

void loop() {
  // put your main code here, to run repeatedly:

}
