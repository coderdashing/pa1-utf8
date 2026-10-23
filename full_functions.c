#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

int8_t width_from_start_byte(char c) {
  if((c & 0x80) == 0x00) { return 1; } // ASCII = 1 byte
  if((int)(c & 0xE0) == 0xC0) { return 2; } // 2 byte case
  if((c & 0xF0) == 0xE0) { return 3; } // 2 byte case
  if((c & 0xF8) == 0xF0) { return 4; } // 3 byte case
  return -1;
}
int8_t is_ascii(char string[])
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
	if(string[0] == '\0'){
		return 0;
	}
    if ((string[0] & 0b10000000) == 0b00000000) return 1;
    if ((string[0] & 0b11100000) == 0b11000000) return 2;
    if ((string[0] & 0b11110000) == 0b11100000) return 3;
    if ((string[0] & 0b11111000) == 0b11110000) return 4;
    return -1; 
}
int32_t utf8_strlen(char str[]) {
    // TODO: Implement this function
	
	if(str[0] == '\0'){
		return 0;
	}

    int i = 0;
    int count = 0;
    int w = 0;
    while(str[i] != '\0' ){//add condition to check for str's length or out of bounds
        if(str[i] == 0xfe || str[i] == 0xff || str[i] == 0xc1 || str[i] == 0xc0 || (str[i] >= 0xf5 && str[i] <= 0xff) || str[i] == 0xa0 || str[i] == 0xa4){
			return -1;
		}
		w = width_from_start_byte(str[i]);
		if(w == -1){
			return -1;
		}
        if(w < 0){
            break;
        }
        i += w;

        count++;
        
    }
    return count;
}
void utf8_substring(const char str[], int start, int end,char result[]) {
    //Implement utf8_substring in the body of this function!
    if(start < 0){
		result[0] = '\0';
	}
	if(end < start){
		result[0] = '\0';
		return;
	}
	
	int i = 0;
    int count = 0;
    int w = 0;
    while((str[i] != 0) && (count < start) ){
        w = (int)width_from_start_byte(str[i]);
        
        i +=  (int)width_from_start_byte(str[i]);

        count++;
        
    }
	int mark = i;
    while((str[i] != 0) && (count < end)){
        
        w = (int)width_from_start_byte(str[i]);
        for (int j = 0; j < w; j++){
            
            result[i+j - mark] = str[i+j];
            
        }
        
        i += w;
        count++;
    }
	result[i-mark] = '\0';
    
}

int32_t codepoint_at(char str[], int32_t byte_index) {
    // TODO: Implement this function
    
	if(byte_index < 0){
        return -1;
    }
	if(byte_index >= strlen(str)){
		return -1;
	}
    int i = 0;
    while(i < byte_index){
        i += (int)width_from_start_byte(str[i]);
        if(i > byte_index){
            return -1;
        }
        
    }
    int width = (int)width_from_start_byte(str[i]);
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
        
        ind += (int)width_from_start_byte(str[ind]);
        count++;
    }
    if(count < index){
        return 0;
    }
    int32_t cp =  codepoint_at(newstr,ind);
    return (cp >= 0x1F400 && cp <= 0x1F43F) || (cp >= 0x1F980 && cp <= 0x1F9AE);
}
void next_utf8_char(char str[], int32_t cpi, char result[]){
	
	int32_t source = codepoint_at(str,cpi) + 1;
	if(source == 0){

		return;
	}
	//subject to change
	//possibilities:
	//ascii: 8 bits
	//2: 11 bits or 6 + 5 < 1000 0000 0000
	//3: 16 bits or 6 + 6 + 4 < 0x10000
	//
	//4: 21 bits or 6 + 6 + 6 + 3 0010 0000 0000 0000 0000 0000
	if((int) source < 128){
		result[0] = source;
		result[1] = '\0';
	}
	else if(source < 0x800){
		result[0] = ((source >> 6) & 0x1F) | 0xC0;
		result[1] = (source & 0x3F) | 0x80;
		result[2] = '\0';
	}
	else if(source < 0x10000){
		result[0] = ((source >> 12) & 0xF) | 0xE0;
		result[1] = ((source >> 6) & 0x3F) | 0x80;
		result[2] = (source & 0x3F) | 0x80;
		result[3] = '\0';
	}
	else if(source < 0x200000){
		result[0] = ((source >> 18) & 0x7) | 0xF0;
		result[1] = ((source >> 12) & 0x3F) | 0x80;
		result[2] = ((source >> 6) & 0x3F) | 0x80;
		result[3] = (source & 0x3F) | 0x80;
		result[4] = '\0';
	}
	else{
		result[0] = '\0';
	}
}
int main()
{
        // TODO: Read lines from stdin in a loop
        // Each line contains two strings separated by a space
        // Parse the two strings from each line
        // Call string_cap_match and print "match" or "miss"
        char buffer[201];
    char r1[100];
    char r2[100];
    int i1;
    int i2;
    int mainind;
    int ind2;
	mainind = 0;
	char *scan1;

	printf("Enter a UTF-8 encoded string: ");		
    scan1 = fgets(buffer,sizeof(buffer),stdin);

	if(scan1[strlen(scan1)-1] == '\n'){
		scan1[strlen(scan1)-1] = '\0';

	}
        i1 = 0;
        i2 = 0;
        ind2 = 0;

    printf("\nValid ASCII: ");
	if(is_ascii(scan1)){
		printf("true\n");
	}
	else{
		printf("false\n");
	}
	char temp[100];
	int i = 0;
	while(scan1[i] != '\0'){
		temp[i] = scan1[i];
		i++;
	}
	temp[i] = '\0';
	capitalize_ascii(temp);
	printf("Uppercased ASCII: \"");
	for(int j = 0; j < i-1; j++){
		printf("%c",temp[j]);
	}

    printf("%c\"\nLength in bytes: %d\n",temp[i-1],(int)strlen(scan1));
	printf("Number of code points: %d\n",(int)utf8_strlen(scan1));
    i = 0;
	int count = 0;
	int w = 0;
	printf("Bytes per code point: ");
    while(scan1[i] != '\0' ){//add condition to check for str's length or out of bounds
        w = width_from_start_byte(scan1[i]);
        if(w < 0){
            break;
        }
		printf("%d ",w);
        i += w;

    

    }
  	printf("\nSubstring of the first 6 code points: ");
	utf8_substring(scan1,0,6,temp);
	printf("%s\n",temp);
	printf("Code points as decimal numbers:");
	i = 0;
	w = 0;
	while(scan1[i] != '\0'){
		w = width_from_start_byte(scan1[i]);
		if(w < 0){
			break;
		}
		printf(" %d",codepoint_at(scan1,(int32_t)i));
		i += w;
		
	}
	printf("\nAnimal emojis: ");
    i = 0;
  
    w = 0;
	while(scan1[i] != '\0'){
        w = width_from_start_byte(scan1[i]);
        if(w < 0){
			break;
        }
        if(is_animal_emoji_at(scan1,(int32_t)count) == 1){
			for(int j = 0; j < w; j++){
				printf("%c",scan1[i+j]);
			}
		}
        i += w;
		count++;
    }
	printf("\nNext character of code point at index 3:");
	char res[4];
	if(count >= 4){
		next_utf8_char(scan1,3,res);
		printf("%s",res);
	}
	printf("\n");
	return 0;
};
