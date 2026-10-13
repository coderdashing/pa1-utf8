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
