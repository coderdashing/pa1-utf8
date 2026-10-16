#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

uint8_t is_ascii(char string[])
{
    // TODO: Implement is_ascii in the body of this function!
    int i = 0;
    while(string[i] != '\0') {
        if((int)(string[i] >> 7) == -1){
            return 0;
        }
        i++;
    }
    return 1;
}
int32_t capitalize_ascii(char str[])
{
    // TODO: Implement capitalize_ascii in the body of this function!
    int i = 0;
    int32_t total = 0;
    while(str[i] != '\0'){
        if((int)(str[i]) >= 97 && (int)(str[i]) <= 122){
            str[i] -= 32;
            total++;
        }
        i++;
    }
    return total;

}
int8_t codepoint_size(char string[]) {
    if ((string[0] & 0b10000000) == 0b00000000) return 1;
    if ((string[0] & 0b11100000) == 0b11000000) return 2;
    if ((string[0] & 0b11110000) == 0b11100000) return 3;
    if ((string[0] & 0b11111000) == 0b11110000) return 4;
    return -1; 
}
int32_t utf8_strlen(char str[]) {
    // TODO: Implement this function

    int i = 0;
    int count = 0;
    int w = 0;
    while(str[i] != '\0' ){//add condition to check for str's length or out of bounds
        w = width_from_start_byte(str[i]);
        if(w < 0){
            break;
        }
        i += width_from_start_byte(str[i]);

        count++;
        
    }
    return count;
}
void utf8_substring(const char str[], int start, int end) {
    //Implement utf8_substring in the body of this function!
        int i = 0;
    int count = 0;
    int w = 0;
    while(count < start ){
        w = width_from_start_byte(str[i]);
        
        i += width_from_start_byte(str[i]);

        count++;
        
    }
    while(count < end){
        
        w = width_from_start_byte(str[i]);
        for (int j = 0; j < w; j++){
            
            printf("%c",str[i+j]);
            
        }
        
        i += w;
        count++;
    }
    printf("\n");
}
int8_t width_from_start_byte(char c) {
  if((c & 0x80) == 0x00) { return 1; } // ASCII = 1 byte
  if((int)(c & 0xE0) == 0xC0) { return 2; } // 2 byte case
  if((c & 0xF0) == 0xE0) { return 3; } // 2 byte case
  if((c & 0xF8) == 0xF0) { return 4; } // 3 byte case
  return -1;
}

int32_t codepoint_at(char str[], int32_t byte_index) {
    // TODO: Implement this function
    if(byte_index < 0){
        return -1;
    }
    int i = 0;
    while(i < byte_index){
        i += width_from_start_byte(str[i]);
        if(i > byte_index){
            return -1;
        }
        
    }
    int width = width_from_start_byte(str[i]);
    if (width == 1) {
        return str[i] & 0x7F;
    }
    else if (width == 2) {
        return ((str[i] & 0x1F) << 6) |
               ((unsigned char)str[i+1] & 0x3F);
    }
    else if (width == 3) {
        return ((str[i] & 0x0F) << 12) |
               (((unsigned char)str[i+1] & 0x3F) << 6) |
               ((unsigned char)str[i+2] & 0x3F);
    }
    else if(width == 4){
        return ((str[i] & 0x07) << 18) |
               (((unsigned char)str[i+1] & 0x3F) << 12) |
               (((unsigned char)str[i+2] & 0x3F) << 6) |
               ((unsigned char)str[i+3] & 0x3F);
    }
    return -1;
}
int is_animal_emoji_at(const char str[], int index){
    
    int i = 0;
    while(str[i] != '\0'){
        i++;
    }
    char newstr[i];
    for (int j = 0; j < i; j++){
        newstr[j] = str[j];
    }
    
    int count = 0;
    
    int ind = 0;
    while(ind < i && count < index){
        
        ind += width_from_start_byte(str[ind]);
        count++;
    }
    if(count < index){
        return 0;
    }
    int32_t cp =  codepoint_at(newstr,ind);
    return (cp >= 0x1F400 && cp <= 0x1F43F) || (cp >= 0x1F980 && cp <= 0x1F9AE);
}

